#ifndef _exprtrie_H
#define _exprtrie_H


namespace qtl {

template <class Traits>
struct ExprTrieCreator
{
    typedef typename Traits::TrieAgent  TrieAgent;
    typedef typename Traits::Token      Token;
    typedef typename Traits::Node       Node;   /* Actually it is "Node Ref" */

    enum Result
    {
        k_NoErr = 0,
        k_UnknownErr,
        k_UnknownToken,
        k_InitErr,
        k_TokenErr,
        k_NodeEnlargeErr,
        k_NodeStretchErr,
        k_NodeCreateErr,
        k_UnexpectedTerm,
        k_UnexpectedComma,
        k_UnexpectedLeftParenthesis,
        k_UnexpectedRightParenthesis,
        k_UnexpectedLeftSquareBracket,
        k_UnexpectedRightSquareBracket,
        k_UnexpectedBinaryOperator,
        k_UnexpectedHighOperator,
        k_UnexpectedEnding,
        k_NodeCreationErr,
        k_ExpectingRightParenthesis,
        k_TrieErr,
    };

    /*
     * This implementation allows full communication between
     * the trie and the token parser [:token:], which further
     * allows the parser to do the parsing with trie as a
     * reference or context.and even trie may have a reference
     * to the trie creator itself.
     * This may prove useful in heuristic parsing such as determining
     * whether a left parenthesis is possible to be followed 
     * by a type converter
     *
     *
     * Recommendation on extending to support C++:
     *  Identifying constructor, creation by new operator, type conversion
     *  is the resposibility of token parser
     *  The parser distinguish between constructor and type conversion by
     *  examining whether there is a right parenthesis at the tail of
     *  the token. A constructor is a function that returns a temporary
     *  value
     *  When a [:new:] operator occurs, the node to be created should be a 
     *  function similar to constructor with some variation extra information 
     *  distinguishing it from a constructor and needed by the operator
     *  mechanism
     * 
     */

    /*
     * Initialization
     */
    bool Init(TrieAgent &trie)
    {
        SepInfo sepInfo;

        m_pTrie = &trie;
        m_pTrie->Init(m_CmpEnd);    /* get the uncreated root */

        // clear stack
        while (!m_SepStack.empty())
        {
            m_SepStack.pop();
        }

        if (ProcessLeftBracket(TokenId(0), false, true) != k_NoErr)
        {
            return false;
        }

        m_bLastIsLeftBracket = false;

        return true;
    }

    /*
     * Deinitialization
     * It checks if the trie has succesfully been created
     */
    bool Deinit()
    {   /* return true if the trie is successfully created */
        bool rc = true;
        if (!m_pTrie->NodeCreated(m_CmpEnd))
        {   /* the trie is empty or missing the last term to 
            * fill the empty node stretched out */
            rc = false; 
        }
        else if (ProcessRightBracket(TokenId(0), true) != k_NoErr)
        {
            rc = false;
        }

        if (!m_SepStack.empty())
        {
            rc = false;
            while (!m_SepStack.empty())
            {
                m_SepStack.pop();
            }
        }
        return rc;
    }

    bool IsCompleted()
    {
        if (m_bLastIsLeftBracket) return false;
        if (!m_pTrie->NodeCreated(m_CmpEnd))
        {   /* the trie is empty or missing the last term to 
            * fill the empty node stretched out */
            return false;
        }

        if (m_SepStack.empty())
        {
            return false;   // an error
        }
        if (m_SepStack.size() > 1)
        {
            return false;
        }

        SepInfo &sepInfo = m_SepStack.top();
        return sepInfo.IsStub() && 
            sepInfo.brackets.size() == 1 && sepInfo.brackets.top().count = 1;
    }


    Result ParseOneStep(Token &token)
    {
        Result  rc = k_NoErr;
       
        if (m_bLastIsLeftBracket)
        {
            if (token.IsRightBracket())
            {   /* no arg func */
                rc = ProcessLeftBracket(m_LastTokenId, true);
                if (rc != k_NoErr)
                {
                    return rc;
                }
                rc = ProcessRightBracket(token.GetId());
                if (rc != k_NoErr)
                {
                    return rc;
                }
                return k_NoErr;
            }
            else
            {   /* may have arguments */
                rc = ProcessLeftBracket(m_LastTokenId, false);
                if (rc != k_NoErr)
                {
                    return rc;
                }
            }
            m_bLastIsLeftBracket = false;
        }

        if (token.IsTerm())
        {
            rc = ProcessTerm(token);
        }
        else if (token.IsLeftBracket())
        {
            m_LastTokenId = token.GetId();
            m_bLastIsLeftBracket = true;
        }
        else if (token.IsRightBracket())
        {
            rc = ProcessRightBracket(token.GetId());
        }
        else if (token.IsComma())
        {
            rc = ProcessComma();
        }
        else if (token.IsUnaryOperator())
        {
            rc = ProcessUnaryOperator(token);
        }
        else if (token.IsBinaryOperator())
        {
            rc = ProcessBinaryOperator(token);
        }
        else if (token.IsHighOrderOperator())
        {
            /*
            * About tertiary and higher:
            * It is assumed in this parser that tertiary
            * or higher order operator has no confusion
            * or obliqueness in combination.
            * It means any two high order operators can
            * not get twined with each other
            *
            * First level high order operator can take over
            * the operation from no lower priority binary
            * or higher operator
            *
            * The last stage of a high order operation
            * can be ended by a bracket/comma
            * or a no greater priority binary or higher operator
            *
            * And every time a subsequent operator of a
            * tertiary or high order operator occurs
            * a check is made to see if it happens at
            * the right position
            *
            */
            rc = ProcessHighOrderOperator(token);
        }
        else    /* error */
        {
            rc = k_UnknownToken;  /* unknown or unsupported token */
        }

        //printf("completed: %s\n", IsCompleted()? "true" : "false");
        return rc;
    }

    Result Parse(TrieAgent &trie, Token &token)
    {
        if (!Init(trie))
        {
            return k_InitErr;
        }

        for ( token.First(); token; token.Next())
        {
            Result  rc = ParseOneStep(token);
            if (rc != k_NoErr)
            {
                return rc;
            }
        }

        /* 
         * check the tree 
         * if successful
         */
        return Deinit()? k_NoErr : k_UnexpectedEnding;
    }

    const Node & GetCmpStart() const { return m_CmpStart; }
    const Node & GetCmpEnd() const { return m_CmpEnd; }

protected:
    struct SepInfo;
    typedef typename Traits::template StackWrap<SepInfo>::Stack SepStack;
    typedef typename Traits::Count      Count;
    typedef typename Traits::TokenId    TokenId;

    struct SepInfo
    {
        enum
        {
            k_None,     // undetermined
            k_Stub,     // '((.', one element with several parentheses on its left, 
                        // possible to be within a tuple or within simply multi-fold brackets
            k_Func,
            k_Tuple,
            k_Array,
            k_HighOp,
        } type;

        struct BracketInfo
        {
            Count       count;
            TokenId     tokenId;
            BracketInfo(Count c, const TokenId &id) : count(c), tokenId(id) {}
        };

        typedef typename Traits::template StackWrap<BracketInfo>::Stack     BracketStack;

        Count           fullness;
        Node            node;
        BracketStack    brackets;

        SepInfo()
            : fullness(0), type(k_None)
        {
        }

        void SetAsStub() { type = k_Stub; }
        void SetAsFunc() { type = k_Func; }
        void SetAsTuple() { type = k_Tuple; }
        void SetAsHighOp() { type = k_HighOp; }

        bool IsStub() const { return type == k_Stub; }
        bool IsFunc() const { return type == k_Func; }
        bool IsTuple() const { return type == k_Tuple; }
        bool IsHighOp() const { return type == k_HighOp; }

        void UpdateCmpStart(TrieAgent *pTrie, Node &nodeStart) 
        {
            if (IsStub())
            {
                nodeStart = node;
            }
            else    /* IsFunc() || IsTuple() || IsArray() || IsHighOp() */
            {
                pTrie->GetChild(node, fullness-1, nodeStart);
            }
        }

        bool AcceptComma() const 
        {
            return (IsFunc() || IsTuple() || HasBrackets());
        }

        void PushBracket(TokenId tokenId)
        {
            if (brackets.empty() || brackets.top().tokenId != tokenId)
            {
                brackets.push(BracketInfo(1, tokenId));
            }
            else if (brackets.top().tokenId == tokenId)
            {
                ++brackets.top().count;
            }
        }
        
        bool PopBracket(TokenId tokenId)
        {   /* Here we do the bracket matching */
            TokenId &id = tokenId;
            if (brackets.empty())
            {   // error
                return false;
            }
            if (brackets.top().tokenId != tokenId)
            {
                return false;
            }
            if (--brackets.top().count == 0)
            {
                brackets.pop();
            }
            return true;
        }

        void PopBracket()
        {
            if (brackets.empty())
            {   // error
                return;
            }
            if (--brackets.top().count == 0)
            {
                brackets.pop();
            }
        }

        bool HasBrackets() const
        {
            return !brackets.empty();
        }

        void Conclude(TrieAgent *pTrie, TokenId tokenId)
        {
            pTrie->CloseNode(node, fullness, tokenId);

            // recover the fullness to 1 in favor of the fact
            // that the node may revert to be a stub
            type = k_Stub;
            fullness = 1;   
        }

    };

protected:


    /*
     * Find according to the priority the appropriate position for 
     * the [:newNode:] to be inserted along the op-right-slop from 
     * [:m_CmpStart:] which is the first root node of a full subtrie
     *  when tracing along the rightmost side of the whole trie,
     *  or the uncreated node if unavailable
     *  termed 'first rightmost full-node'
     * to [:m_CmpEnd:] which is the first node of a complete-node
     *  (a term, the root of a complete subtrie or the uncreated)
     *  or the uncreated node if unavailable
     *  termed 'first rightmost complete-node'
     */

    void FindNodeAsLeft(Node &p, Node &newNode)
    {
        /*
         * About PassNode
         * For left associativity (common)
         *  prio(p) < prio(newNode)
         * For right associativity (e.g. assignments)
         *  if both p and newNode are assignments, return true
         *
         *
         * Note again that m_CmpEnd always points to the 
         * uncreated node if there is one. 
         */

        if (Traits::k_TrieFind)
        {
            m_pTrie->FindNodeAsLeft(p, newNode, m_CmpStart, m_CmpEnd);
        }
        else
        {
            p = m_CmpStart;
            while (p != m_CmpEnd && m_pTrie->PassNode(p, newNode))
            {
                m_pTrie->GoChild(p, m_pTrie->NumChildren(p) - 1);
            }
        }
    }

    /*
     * Do the rotation
     * Remove associated subtree with [:oriNode:] as root
     * and place [:newNode:] at the original branch
     * attach the tree to the leftmost of [:newNode:]
     *
     * Note:
     * Both the two nodes must have been created
     */
    Result ReplaceAndPutLeft(Node &oriNode, Node &newNode)
    {
        bool bRefreshSep = false;
        SepInfo &sepInfo = m_SepStack.top();
        if (oriNode == sepInfo.node)
        {   /* oriNode must be a stub so that it acts as a sep */
            if (!sepInfo.IsStub())
            {   // should not happen
                return k_UnknownErr;
            }
            bRefreshSep = true;
        }

        if (!m_pTrie->ReplaceAndPutLeft(oriNode, newNode))
        {
            return k_TrieErr;
        }

        if (bRefreshSep)
        {
            sepInfo.node = newNode;
            sepInfo.fullness = 1;
        }
        return k_NoErr;
    }

    /*
     * Process left term, conceptually including constants, 
     * function/variable/member names, etc.
     *
     * Note:
     * An aggressive trie manipulator within the calling of
     * [:CreatingAsTerm:] may further imply the details about
     * the node on itself or by invoking or enquiring other
     * modules and do the possible conversion on the node
     * since the node (reference) has already been attached to
     * the correct position in the trie.
     * 
     */
    Result ProcessTerm(Token &token)
    {
        bool bToUpdateStart = (m_CmpStart == m_CmpEnd);
        bool bToUpdateStub = false;
        if (bToUpdateStart)
        {
            // stub node to be created
            bToUpdateStub = (m_SepStack.top().node == m_CmpStart);
        }
        // may be more secure and may provide more 
        // degree of freedom of trie implentation

        if (m_pTrie->NodeCreated(m_CmpEnd))
        {
            return k_UnexpectedTerm;
        }
        if (!m_pTrie->CreateAsTerm(m_CmpEnd, token))
        {
            return k_NodeCreateErr;
        }
        if (bToUpdateStart)
        {
            m_CmpStart = m_CmpEnd;
            if (bToUpdateStub)
            {
                m_SepStack.top().node = m_CmpEnd;
            }
        }
        return k_NoErr;
    }

    /*
     * Process left parenthesis '('
     */
    Result ProcessLeftBracket(TokenId tokenId = 0, bool bNoArgFunc = false, bool bIniting = false)
    {
        Result rc = k_NoErr;

        // bind left parenthesis to the current node
        if (!m_pTrie->NodeCreated(m_CmpEnd))
        {   /* stub */
            if (!bIniting && m_SepStack.top().node == m_CmpEnd)
            {
                // e.g. "(" + '('

                // increment the bracket number
                m_SepStack.top().PushBracket(tokenId);
            }
            else
            {
                // e.g. "*" + '('
                //      ", " + '('
                //      ...

                SepInfo sepInfo;

                sepInfo.fullness = 0;

                sepInfo.node = m_CmpEnd;
                sepInfo.SetAsStub();
                sepInfo.PushBracket(tokenId);

                m_SepStack.push(sepInfo);

                m_CmpStart = m_CmpEnd;
            }
        }
        else
        {   /* 
             * functorial 
             *
             * always treat all the rest cases as funtorial
             */
            //assert(pToken != 0);

            Node p, funcNode;
            if (!m_pTrie->CreateAsFunc(funcNode, tokenId))
            {
                return k_NodeCreateErr;
            }

            // commonly func has a very high priority
            FindNodeAsLeft(p, funcNode);
           
            /* check validity which is not necessary
             * checking can be left to stage later than syntactical analysis
             */
            if (!m_pTrie->NodeForFunc(p))
            {
                return k_UnexpectedLeftParenthesis;
            }

            rc = ReplaceAndPutLeft(p, funcNode);
            if (rc != k_NoErr)
            {
                return rc;
            }

            SepInfo *pSepInfo = &m_SepStack.top();
            if (pSepInfo->node != funcNode)
            {   /* 
                 * the node that the func has just taken over 
                 * is not a stub 
                 */
                m_SepStack.push(SepInfo());
                pSepInfo = &m_SepStack.top();
            }

            /* 
             * There may be recommendations from type info, 
             * in that case, TrieAgent::CreateAsFunc() takes the responsiblity
             * of determining the function type by tracing through the left branch
             * of [:funcNode:], say (previously) [:m_CmpEnd:]
             */
            pSepInfo->fullness = 1;


            pSepInfo->node = funcNode;
            pSepInfo->SetAsFunc();  /* it may originally be stub */

            // to be created
            // slot 0 is for the name of the function
            if (!bNoArgFunc)
            {
                if (!m_pTrie->Stretch(funcNode, 1, m_CmpEnd))
                {
                    return k_NodeStretchErr;
                }
                m_CmpStart = m_CmpEnd;
            }
            else
            {   /* function with no argument */
                m_CmpStart = m_CmpEnd = funcNode;
                                        // make it legal when processing right parenthesis

                --pSepInfo->fullness;   // an increment on fullness will be performed
                                        // in the closing process which is sensible 
                                        // for function taking arguments

            }
        }

        /*
         * m_CmpStart == m_CmpEnd, uncreated,
         * either being a stub or the first argument of the function
         * or pointing to the function node when the function has no
         * argument
         */

        return k_NoErr;
    }

    /*
     * Process right parenthesis ')'
     */
    Result ProcessRightBracket(TokenId tokenId = 0, bool bEnding = false)
    {
        if (m_SepStack.empty())
        {
            return k_UnexpectedRightParenthesis;    // unexpected
        }

        SepInfo &sepInfo = m_SepStack.top();

        if (!m_pTrie->NodeCreated(m_CmpEnd))
        {   // A new term is being created
            return k_UnexpectedRightParenthesis;    // unexpected
        }

        // in fact, m_CmpEnd setting here doesn't concern
        // the succeeding processes at all (and they will
        // work out the m_CmpEnd themselves in need of it)
        // it only shows what is correct in conception.
        // 
        // However, m_CmpStart setting at the later part of this subroutine
        // is deadly crucial.

        m_CmpEnd = sepInfo.node;    // the node is closed
                                    // although there may be other
                                    // left brackets remaining on it
        
        if (sepInfo.IsFunc() || sepInfo.IsTuple())
        {
            sepInfo.fullness++;
            sepInfo.Conclude(m_pTrie, tokenId);  
                                        // [:Conclude:] close the node
                                        // which turns it to stub and 
                                        // if it truly is it will go thru 
                                        // the parentheses checking following
        }
        else if (sepInfo.HasBrackets())
        {   // stub
            sepInfo.PopBracket(tokenId);    // this function close the node
        }
        else
        {
            return k_UnexpectedRightParenthesis;
        }

        if (!sepInfo.HasBrackets())
        {
            m_SepStack.pop();
            if (m_SepStack.empty())
            {
                if (!bEnding)
                {
                    return k_UnknownErr;    /* should not happen */
                }
            }
            else
            {
                SepInfo &prevSep = m_SepStack.top();
                prevSep.UpdateCmpStart(m_pTrie, m_CmpStart);
            }
        }
        else
        {
            m_CmpStart = m_CmpEnd;
        }

        /*
         * both nodes are created
         * m_CmpStart is derived from previous separator or
         * is the closed func node if it still remains as a stub
         * m_CmpEnd is at the closed func node
         */

        return k_NoErr;
    }

    /*
     * Process comma ','
     */
    Result ProcessComma()
    {
        Result rc = k_NoErr;

        if (!m_pTrie->NodeCreated(m_CmpEnd))
        {   // A new term is to be created
            return k_UnexpectedRightParenthesis;    // unexpected
        }

        if (m_SepStack.empty())
        {
            return k_UnexpectedComma;   // unexpected
        }
        SepInfo &sepInfo = m_SepStack.top();

        if (!sepInfo.AcceptComma())
        {   
            /* 
             * sepInfo.node is functor or remains left brackets 
             * i.e. (IsFunc() || HasBracket() > 0)
             */
            return k_UnexpectedComma;   // unexpected
                                        // Notice that C# allows comma inside '[]'
        }

        if (sepInfo.IsFunc())
        {
            sepInfo.fullness++;
        }
        else
        {   /* tuple */
            if (!sepInfo.IsTuple())
            {   /* stub, to turn into a tuple */
                Node oldNode = sepInfo.node;
                Node tupleNode;

                /*
                 * [m_pTrie] can get the information of the tuple from 
                 * [: oldNode :]
                 */

                if (!m_pTrie->CreateAsTuple(tupleNode, oldNode))
                {   
                    return k_NodeCreationErr;
                }

                rc = ReplaceAndPutLeft(oldNode, tupleNode);
                if (rc != k_NoErr)
                {
                    return rc;
                }

                sepInfo.PopBracket();   // eliminate the parenthesis now serving as the beginning of the tuple 
                sepInfo.SetAsTuple();
                sepInfo.node = tupleNode;
                sepInfo.fullness = 1;
            }
            else
            {
                sepInfo.fullness++;
            }
        }

        if (!m_pTrie->Stretch(sepInfo.node, sepInfo.fullness, m_CmpEnd))
        {
            return k_NodeStretchErr;
        }
        m_CmpStart = m_CmpEnd;

        /*
         * m_CmpStart and m_CmpEnd are the same, pointing to the node to be created
         */

        return k_NoErr;
    }


    /*
     * Process unary operators
     */
    Result ProcessUnaryOperator(Token &token)
    {
        Result rc;
        Node p;
        Node newNode;

        if (!m_pTrie->CreateAsUnary(newNode, token))
        {
            return k_NodeCreationErr;
        }
        FindNodeAsLeft(p, newNode);
        
        bool bSwitchStart = (m_CmpStart == p);
        bool bUpdateEnd = (m_CmpEnd == p);
        rc = ReplaceAndPutLeft(p, newNode);
        if (rc != k_NoErr)
        {
            return rc;
        }
        
        if (bSwitchStart)
        {
            m_CmpStart = newNode;
        }
        if (bUpdateEnd)
        {
            m_pTrie->GetChild(newNode, 0, m_CmpEnd);
        }

        /*
         * m_CmpStart has been created, it is either the newly created
         * operator node when it turns to be the first rightmost full-node
         * ([:newNode:] takes over the position once held by m_CmpStart) or 
         * left unchanged which is impossible to be uncreated
         * m_CmpEnd is left unchanged and can either be created or uncreated
         */

        return k_NoErr;
    }

    /*
     * Process binary operators
     */
    Result ProcessBinaryOperator(Token &token)
    {
        Result  rc;
        Node    p;
        Node    newNode;

        if (!m_pTrie->NodeCreated(m_CmpEnd))
        {   // A new term is being created
            return k_UnexpectedBinaryOperator;  // unexpected
        }

        if (!m_pTrie->CreateAsBinary(newNode, token))
        {
            return k_NodeCreationErr;
        }

        FindNodeAsLeft(p, newNode);
        /*
         * p must be prior to or equal to m_CmpEnd, the leaf
         */
     
        bool bSwitchStart = (m_CmpStart == p);

        rc = ReplaceAndPutLeft(p, newNode);
        if (rc != k_NoErr)
        {
            return rc;
        }
        if (bSwitchStart)
        {
            m_CmpStart = newNode;
        }

        if (!m_pTrie->Stretch(newNode, 1, m_CmpEnd))
        {
            return k_NodeStretchErr;
        }

        /*
         * m_CmpStart has been created, it is either the newly created
         * operator node when it turns to be the first rightmost full-node
         * ([:newNode:] takes over the position once held by m_CmpStart) or 
         * left unchanged which is impossible to be uncreated
         *
         * m_CmpEnd is newly stretched uncreated node
         */

        return k_NoErr;
    }

    /*
     * Process high-order operators, including the tertiary
     */
    Result ProcessHighOrderOperator(Token &token)
    {
        if (!m_pTrie->NodeCreated(m_CmpEnd))
        {   // A new term is being created
            return k_UnexpectedHighOperator;    // unexpected
        }

        SepInfo &topInfo = m_SepStack.top();
        if (token.HighOpStage() == 0)
        {
            Node p;
            Node highNode;

            if (!m_pTrie->CreateAsHighOp(highNode, token))
            {
                return k_NodeCreationErr;
            }
            FindNodeAsLeft(p, highNode);
            Result rc = ReplaceAndPutLeft(p, highNode);
            if (rc != k_NoErr)
            {
                return rc;
            }

            SepInfo *pSepInfo = &m_SepStack.top();
            if (pSepInfo->node != highNode)
            {
                m_SepStack.push(SepInfo());
                pSepInfo = &m_SepStack.top();
                pSepInfo->node = highNode;
            }

            pSepInfo->SetAsHighOp();
            pSepInfo->fullness = 1;

            if (!m_pTrie->Stretch(topInfo.node, 1, m_CmpEnd))
            {
                return k_NodeStretchErr;
            }

            m_CmpStart = m_CmpEnd;    // to be created
        }
        else if (m_pTrie->SameHighOp(topInfo.node, token) 
            && topInfo.fullness == token.HighOpStage())
        {
            topInfo.fullness++;
            if (!m_pTrie->Stretch(topInfo.node, topInfo.fullness, m_CmpEnd))
            {
                return k_NodeStretchErr;
            }

            m_CmpStart = m_CmpEnd;

            if (topInfo.fullness == token.HighestStage())
            {   // last
                m_SepStack.pop();
                if (m_SepStack.empty())
                {
                    return k_UnknownErr;    /* should not happen */
                }
                SepInfo &prevSep = m_SepStack.top();
                prevSep.UpdateCmpStart(m_pTrie, m_CmpStart);
            }
        }
        else
        {
            return k_UnexpectedHighOperator;
        }

        /*
         * m_CmpStart and m_CmpEnd are the same, the node to be created
         * or m_CmpStart is at the previous separator when at the last
         * stage of the high-op
         */

        return k_NoErr;
    }

protected:
    TrieAgent   *m_pTrie;

    Node        m_CmpStart, m_CmpEnd;

    SepStack    m_SepStack;

    bool        m_bLastIsLeftBracket;
    TokenId     m_LastTokenId;

};  /* struct ExprTrieCreator */

}   /* using namespace qtl */

#endif  /* ifndef _exprtrie_H */
