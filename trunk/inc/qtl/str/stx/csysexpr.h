#ifndef _csysexpr_H
#define _csysexpr_H


#include <vector>
#include <string>
#include <cassert>
#include <stack>

namespace qtl {

struct CsysCommon
{
    enum NodeType
    {
        k_Raw = -1,     

        /*
         * comma has the lowest priority,
         * however it is not treated as a a node
         */

        k_Query = 0,    // ' ? : ', symbol '?' for token parsing
        
        /* all the following assigments right associativity */
        k_Asg,          // '='
        k_AsgBOr,       // '|='
        k_AsgBXor,      // '^='
        k_AsgBAnd,      // '&='
        k_AsgShl,       // '<<='
        k_AsgShr,       // '>>='
        k_AsgAdd,       // '+='
        k_AsgSub,       // '-='
        k_AsgMul,       // '*='
        k_AsgDiv,       // '/='
        k_AsgMod,       // '%='


        k_Or,           // '||'
        k_And,          // '&&'


        k_BOr,          // '|'

        k_BXor,         // '^'

        k_BAnd,         // '&'

        k_Eq,           // '=='
        k_Gt,           // '>'
        k_Lt,           // '<'
        k_NEq,          // '!='
        k_GtEq,         // '>='
        k_LtEq,         // '<='


        k_Shl,          // '<<'
        k_Shr,          // '>>'


        k_Add,          // '+'
        k_Sub,          // '-'

        k_Mul,          // '*'
        k_Div,          // '/'
        k_Mod,          // '%'


        k_Addr,         // '&'
        k_PVal,         // '*'
        k_IncL,         // '++i'
        k_DecL,         // '--i'
        k_BNot,         // '~'
        k_Not,          // '!'
        k_Conv,         // (int)

        k_IncR,         // 'i++'
        k_DecR,         // 'i--'
        k_FieldRef,     // '.'
        k_PtFieldRef,   // '->'
        k_Func,         // pow( , )                 // node only
        k_Array,        // a[]                      // node only


        // the following has nothing to do with priority
        k_Tuple,        // ( , , )                  // node only

        // token only
        k_LeftParenthesis,
        k_RightParenthesis,
        k_LeftSquareBracket,
        k_RightSquareBracket,
        k_Comma,
        k_Const,
        k_Name,
        k_Query2,       // symbol ':'
    };

    static const char *k_Symbol[];
    static const int k_Priority[];
};

const char *CsysCommon::k_Symbol[] =
{
    "?:",
    
    "=",
    "|=",
    "^=",
    "&=",
    "<<=",
    ">>=",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",

    "||",
    "&&",

    "|",

    "^",

    "&",

    "==",
    ">",
    "<",
    "!=",
    ">=",
    "<=",


    "<<",
    ">>",

    "+",
    "-",

    "*",
    "/",
    "%",

    "&",
    "*",
    "++L",
    "--L",
    "~",
    "!",
    "CONV",

    "R++",
    "R--",
    ".",
    "->",
    "FUNC",
    "ARRAY",

    "( , )",
};

const int CsysCommon::k_Priority[] =
{
    0,  // ' ? : '
    
    /* all the following assigments right associativity */
    1,  // '='
    1,  // '|='
    1,  // '^='
    1,  // '&='
    1,  // '<<='
    1,  // '>>='
    1,  // '+='
    1,  // '-='
    1,  // '*='
    1,  // '/='
    1,  // '%='

    
    2,  // '||'
    2,  // '&&'


    3,  // '|'

    4,  // '^'

    5,  // '&'

    6,  // '=='
    6,  // '>'
    6,  // '<'
    6,  // '!='
    6,  // '>='
    6,  // '<='


    7,  // '<<'
    7,  // '>>'

    8,  // '+'
    8,  // '-'

    9,  // '*'
    9,  // '/'
    9,  // '%'


    // unary
    10, // '&'
    10, // '*'
    10, // '++i'
    10, // '--i'
    10, // '~'
    10, // '!'
    10, // (int)

    11, // 'i++'
    11, // 'i--'
    11, // '.'
    11, // '->'
    11, // pow( , )
    11, // a[]
};



struct CsysTypeList
{
    typedef std::string     String;

    /* a demo, need optimization on the data structure  */
    std::vector<String>     initialsOfCInnerTypes;

    CsysTypeList()
    {
        initialsOfCInnerTypes.push_back("signed");
        initialsOfCInnerTypes.push_back("unsigned");
        initialsOfCInnerTypes.push_back("int");
        initialsOfCInnerTypes.push_back("long");
        initialsOfCInnerTypes.push_back("short");
        initialsOfCInnerTypes.push_back("float");
        initialsOfCInnerTypes.push_back("double");
        initialsOfCInnerTypes.push_back("void");
        initialsOfCInnerTypes.push_back("char");
        initialsOfCInnerTypes.push_back("struct");
    }

    bool Search(String &str)
    {
        int n = (int)initialsOfCInnerTypes.size();
        for (int i = 0; i < n; i++)
        {
            if (str == initialsOfCInnerTypes[i])
            {
                return true;
            }
        }
        return false;
    }
};


template <class ChrItr, class TypeList = CsysTypeList>
struct CsysToken : public CsysCommon
{
    typedef std::string     String;
    struct TokenInfo
    {
        typedef NodeType    Type;

        Type    type;
        String  name;

        TokenInfo(Type _type = k_Raw, const String &_name = "")
        {
            name = _name;
            type = _type;
        }
    };

    typedef int             TokenId;

    std::vector<TokenInfo>  tokens;
    int                     iToken;
    TypeList                *pTypeList;
    bool                    bExpectingTerm;

    CsysToken() : pTypeList(0), iToken(0), bExpectingTerm(true) {}

    NodeType GetType() { return tokens[iToken].type; }
    String GetName() { return tokens[iToken].name; }

    void Init()
    {
        iToken = 0;
        tokens.clear();
        bExpectingTerm = true;
    }

    static void SkipBlanks(ChrItr &pb, ChrItr pe)
    {
        while (pb!=pe && (*pb==' '||*pb=='\t'||*pb==0x0a||*pb==0x0d))
        {
            ++pb;
        }
    }

    static bool InAlphabet(char ch)
    {
        return (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z');
    }

    static bool IsDigit(char ch)
    {
        return (ch >= '0' && ch <= '9');
    }

    static bool GetName(String &res, ChrItr &pb, ChrItr pe)
    {
        res = "";
        if (!InAlphabet(*pb))
        {
            return false;
        }
        res += *pb;

        for (++pb; InAlphabet(*pb) || IsDigit(*pb); ++pb)
        {
            res += *pb;
        }

        return true;
    }

    static bool GetNumber(String &res, ChrItr &pb, ChrItr pe)
    {
        bool bInt = false;
        bool bPt = false;
        res = "";
        if (*pb=='0')
        {
            bInt = true;
            res += *pb;
            ++pb;
            if (pb==pe)
            {
                return true;
            }
            if (*pb=='x')
            {
                res += *pb;
                ++pb;
            }
        }

        for ( ; pb != pe; ++pb)
        {
            if (IsDigit(*pb))
            {
                res += *pb;
            }
            else if (*pb == '.')
            {
                if (bInt)
                {
                    res = "";
                    return false;
                }
            }
            else
            {
                break;
            }
        }

        for ( ; pb != pe; ++pb)
        {   // not check if the trailing letters are legal
            if (InAlphabet(*pb))
            {
                res += *pb;
            }
            else
            {
                break;
            }
        }
        
        // TODO: 
        // may further check if the number is followed by a 
        // dot which is not allowed, although some may argue
        // that it is not the duty of the lexical parser?

        return true;
    }

    static bool GetString(String &res, ChrItr pb, ChrItr pe)
    {
        res = "";
        if (*pb != '"')
        {
            return false;
        }
        res = "\"";
        for (++pb ; pb != pe; ++pb)
        {
            res += *pb;
            if (*pb == '"')
            {
                ++pb;
                return true;
            }
            else if (*pb == '\\')
            {
                ++pb;
                if (pb == pe)
                {
                    res = "";
                    return false;
                }
            }
        }
        res = "";
        return false;
    }

    bool IsTypeConv(String name, ChrItr &pb, ChrItr pe)
    {
        int nBracket = 0;

        name = "";
        if (*pb != '(')
        {
            return false;
        }
        ++nBracket;
        ++pb;

        SkipBlanks(pb, pe);

        ChrItr p = pb;
        if (!GetName(name, p, pe))
        {
            return false;
        }

        /* The first word of a possible converter must be a type name */
        if (!pTypeList->Search(name))
        {
            name = "";
            return false;
        }

        for ( ; p != pe && nBracket > 0; ++p)
        {
            name += *p;
            // only counting rather than checking the matching of bracket pair
            if (*p == '(' || *p == '[')
            {
                ++nBracket;
            }
            else if (*p == ')' || *p == ']')
            {
                --nBracket;
            }
        }

        if (nBracket > 0)
        {
            return false;
        }

        pb = p;
        return true;
    }

    bool ParseLex(ChrItr &pb, ChrItr pe)
    {
        TokenInfo   newToken;
        SkipBlanks(pb, pe);

        if (*pb == '(')
        {
            ChrItr p = pb;
            if (IsTypeConv(newToken.name, p, pe))
            {   /* conv */
                newToken.type = k_Conv;
                pb = p;
            }
            else
            {
                newToken.name = "";
                newToken.type = k_LeftParenthesis;
                ++pb;
            }
            bExpectingTerm = true;
        }
        else if (*pb == ')')
        {
            newToken.type = k_RightParenthesis;
            bExpectingTerm = false;
            ++pb;
        }
        else if (InAlphabet(*pb))
        {
            // For int(a)
            // even in C it is handled as a function
            // although at the backend of the compiler 
            // it will be interpreted as a type converter
            if (!GetName(newToken.name, pb, pe))
            {
                return false;
            }
            newToken.type = k_Name;
            bExpectingTerm = false;
        }
        else if (IsDigit(*pb))
        {
            if (!GetNumber(newToken.name, pb, pe))
            {
                return false;
            }
            newToken.type = k_Const;
            bExpectingTerm = false;
        }
        else if (*pb == '"')
        {   // string
            if (!GetString(newToken.name, pb, pe))
            {
                return false;
            }
            newToken.type = k_Const;
            bExpectingTerm = false;
        }
        else if (*pb == '=')
        {
            newToken.type = k_Asg;
            bExpectingTerm = true;
            ++pb;
        }
        else if (*pb == '+')
        {
            ++pb;
            if (*pb=='+')
            {   // inc
                if (bExpectingTerm)
                {   // left
                    newToken.type = k_IncL;
                    bExpectingTerm = true;
                }
                else
                {   // right
                    newToken.type = k_IncR;
                    bExpectingTerm = false;
                }
                ++pb;
            }
            else if (*pb == '=')
            {
                newToken.type = k_AsgAdd;
                bExpectingTerm = true;
                ++pb;
            }
            else
            {   // binary
                newToken.type = k_Add;
                bExpectingTerm = true;
            }
        }
        else if (*pb == '-')
        {
            ++pb;
            if (*pb=='-')
            {   // inc
                if (bExpectingTerm)
                {   // left
                    newToken.type = k_DecL;
                    bExpectingTerm = true;
                }
                else
                {   // right
                    newToken.type = k_DecR;
                    bExpectingTerm = false;
                }
                ++pb;
            }
            else if (*pb == '=')
            {
                newToken.type = k_AsgSub;
                bExpectingTerm = true;
                ++pb;
            }
            else if (*pb == '>')
            {
                newToken.type = k_PtFieldRef;
                bExpectingTerm = true;
                ++pb;
            }
            else
            {   // binary
                newToken.type = k_Sub;
                bExpectingTerm = true;
            }
        }
        else if (*pb == '*')
        {
            ++pb;
            if (*pb == '=')
            {
                newToken.type = k_AsgMul;
                ++pb;
            }
            else if (!bExpectingTerm)
            {   // multiplication
                newToken.type = k_Mul;
            }
            else
            {   // access
                newToken.type = k_PVal;
            }
            bExpectingTerm = true;
       }
        else if (*pb == '/')
        {
            ++pb;
            if (*pb == '=')
            {
                newToken.type = k_AsgDiv;
                ++pb;
            }
            else
            {
                newToken.type = k_Div;
            }
            bExpectingTerm = true;
        }
        else if (*pb == '%')
        {
            ++pb;
            if (*pb == '=')
            {
                newToken.type = k_AsgMod;
                ++pb;
            }
            else
            {
                newToken.type = k_Mod;
            }
            bExpectingTerm = true;
        }
        else if (*pb == '&')
        {
            ++pb;
            if (*pb == '=')
            {
                newToken.type = k_AsgBAnd;
                ++pb;
            }
            else if (*pb == '&')
            {
                newToken.type = k_And;
                ++pb;
            }
            else if (!bExpectingTerm)
            {   // bitwise and
                newToken.type = k_BAnd;
            }
            else
            {   // addr
                newToken.type = k_Addr;
            }
            bExpectingTerm = true;
        }
        else if (*pb == '|')
        {
            ++pb;
            if (*pb == '=')
            {
                newToken.type = k_AsgBOr;
                ++pb;
            }
            else if (*pb == '|')
            {
                newToken.type = k_Or;
                ++pb;
            }
            else
            {   // bitwise or
                newToken.type = k_BOr;
            }
            bExpectingTerm = true;
        }
        else if (*pb == '^')
        {
            ++pb;
            if (*pb == '=')
            {
                newToken.type = k_AsgBXor;
                ++pb;
            }
            else
            {   // bitwise xor
                newToken.type = k_BXor;
            }
            bExpectingTerm = true;
        }
        else if (*pb == '<')
        {
            ++pb;
            if (*pb == '=')
            {
                newToken.type = k_LtEq;
            }
            else if (*pb == '<')
            {
                ++pb;
                if (*pb == '=')
                {
                    newToken.type = k_AsgShl;
                    ++pb;
                }
                else
                {
                    newToken.type = k_Shl;
                }
            }
            else
            {
                newToken.type = k_Lt;
            }
            /*
             * Note: (Expansion)
             * To deal with template with arguments using this notation
             * which can be quite complicated (however array and function
             * are not allowed) one should most probably initiate a 
             * different instance of the parsing (either barely lexical or
             * syntactical), it is for two reasons:
             * 1. The arguments within the brackets are no longer
             *    conventional;
             * 2. The constant expression is quite limited, which can be
             *    evaluated at compile time easily.
             */
            bExpectingTerm = true;
        }
        else if (*pb == '>')
        {
            ++pb;
            if (*pb == '=')
            {
                newToken.type = k_GtEq;
            }
            else if (*pb == '>')
            {
                ++pb;
                if (*pb == '=')
                {
                    newToken.type = k_AsgShr;
                    ++pb;
                }
                else
                {
                    newToken.type = k_Shr;
                }
            }
            else
            {
                newToken.type = k_Gt;
            }
            bExpectingTerm = true;
        }
        else if (*pb == '!')
        {
            ++pb;
            if (*pb == '=')
            {
                newToken.type = k_NEq;
                ++pb;
            }
            else
            {
                newToken.type = k_Not;
            }
            bExpectingTerm = true;
        }
        else if (*pb == '~')
        {
            newToken.type = k_BNot;
            bExpectingTerm = true;
            ++pb;
        }
        else if (*pb == '.')
        {
            newToken.type = k_FieldRef;
            bExpectingTerm = true;
            ++pb;
        }
        else if (*pb == ',')
        {
            newToken.type = k_Comma;
            bExpectingTerm = true;
            ++pb;
        }
        else if (*pb == '[')
        {
            newToken.type = k_LeftSquareBracket;
            bExpectingTerm = true;
            ++pb;
        }
        else if (*pb == ']')
        {
            newToken.type = k_RightSquareBracket;
            bExpectingTerm = false;
            ++pb;
        }
        else
        {   // unknown token
            return false;
        }

        tokens.push_back(newToken);
        return true;
    }

    operator bool()
    {
        return iToken < (int)tokens.size();
    }

    void First()
    {
        iToken = 0;
    }

    void Next()
    {
        iToken++;
    }

    TokenId GetId()
    {   /* only used for brackets */

        if (tokens[iToken].type == k_LeftParenthesis || tokens[iToken].type == k_RightParenthesis)
        {
            return 1;   // parenthesis
        }
        else
        {
            return 2;   // square bracket
        }
    }

    bool IsTerm()
    {
        return tokens[iToken].type == k_Const || tokens[iToken].type == k_Name;
    }

    bool IsLeftBracket()
    {
        return IsLeftParenthesis() || IsLeftSquareBracket();
    }

    bool IsRightBracket()
    {
        return IsRightParenthesis() || IsRightSquareBracket();
    }

    bool IsLeftParenthesis()
    {
        return tokens[iToken].type == k_LeftParenthesis;
    }

    bool IsRightParenthesis()
    {
        return tokens[iToken].type == k_RightParenthesis;
    }

    bool IsComma()
    {
        return tokens[iToken].type == k_Comma;
    }

    bool IsLeftSquareBracket()
    {
        return tokens[iToken].type == k_LeftSquareBracket;
    }

    bool IsRightSquareBracket()
    {
        return tokens[iToken].type == k_RightSquareBracket;
    }

    bool IsUnaryOperator()
    {
        return tokens[iToken].type >= k_Addr && tokens[iToken].type <= k_DecR;
    }

    bool IsBinaryOperator()
    {
        return tokens[iToken].type >= k_Asg && tokens[iToken].type <= k_Mod
            || tokens[iToken].type == k_FieldRef || tokens[iToken].type == k_PtFieldRef;
    }

    bool IsHighOrderOperator()
    {
        return tokens[iToken].type == k_Query;
    }

    int HighOpStage()
    {
        if (tokens[iToken].type == k_Query)
        {
            return 0;
        }
        else if (tokens[iToken].type == k_Query2)
        {
            return 1;
        }
        return -1;
    }

    int HighestStage()
    {
        return 3;   /* only case in C */
    }

    void GenerateSample001()
    {
        tokens.clear();
        // sample expression:
        // (a<<2) + p->f(3,a)*(c=7, c+d.i) / (int)(++a)[32](a, b)
        tokens.push_back(TokenInfo(k_LeftParenthesis));
        tokens.push_back(TokenInfo(k_Name, "a"));
        tokens.push_back(TokenInfo(k_Shl));
        tokens.push_back(TokenInfo(k_Const, "2"));
        tokens.push_back(TokenInfo(k_RightParenthesis));
        tokens.push_back(TokenInfo(k_Add));
        tokens.push_back(TokenInfo(k_Name, "p"));
        tokens.push_back(TokenInfo(k_PtFieldRef));
        tokens.push_back(TokenInfo(k_Name, "f"));
        tokens.push_back(TokenInfo(k_LeftParenthesis));
        tokens.push_back(TokenInfo(k_Const, "3"));
        tokens.push_back(TokenInfo(k_Comma));
        tokens.push_back(TokenInfo(k_Name, "a"));
        tokens.push_back(TokenInfo(k_RightParenthesis));
        tokens.push_back(TokenInfo(k_Mul));
        tokens.push_back(TokenInfo(k_LeftParenthesis));
        tokens.push_back(TokenInfo(k_Name, "c"));
        tokens.push_back(TokenInfo(k_Asg));
        tokens.push_back(TokenInfo(k_Const, "7"));
        tokens.push_back(TokenInfo(k_Comma));
        tokens.push_back(TokenInfo(k_Name, "c"));
        tokens.push_back(TokenInfo(k_Add));
        tokens.push_back(TokenInfo(k_Name, "d"));
        tokens.push_back(TokenInfo(k_FieldRef));
        tokens.push_back(TokenInfo(k_Name, "i"));
        tokens.push_back(TokenInfo(k_RightParenthesis));
        tokens.push_back(TokenInfo(k_Div));
        tokens.push_back(TokenInfo(k_Conv, "int"));
        tokens.push_back(TokenInfo(k_LeftParenthesis));
        tokens.push_back(TokenInfo(k_IncL));
        tokens.push_back(TokenInfo(k_Name, "a"));
        tokens.push_back(TokenInfo(k_RightParenthesis));
        tokens.push_back(TokenInfo(k_LeftSquareBracket));
        tokens.push_back(TokenInfo(k_Const, "32"));
        tokens.push_back(TokenInfo(k_RightSquareBracket));
        tokens.push_back(TokenInfo(k_LeftParenthesis));
        tokens.push_back(TokenInfo(k_Name, "a"));
        tokens.push_back(TokenInfo(k_Comma));
        tokens.push_back(TokenInfo(k_Name, "b"));
        tokens.push_back(TokenInfo(k_RightParenthesis));

        iToken = 0;
    }

};


template <class Token>
struct CsysExprTrie : public CsysCommon
{
    typedef std::string     String;

    struct Node
    {
        typedef NodeType    Type;

        typedef std::vector<Node*>  NodeVec;
        NodeVec subnodes;
        Type    type;
        String  name;      /* the node of current implementation is totally based on string */

        Node() : type(k_Raw) {}

        int GetPrio() { return CsysCommon::k_Priority[type]; }
        bool IsAssign() { return type>=k_Asg && type<=k_AsgShr; }
    };

    struct NodeRef
    {
        Node    *parent;
        int     iChild;
        Node    *cache;

        NodeRef() : parent(0), iChild(0), cache(0) {}

        bool operator== (const NodeRef &that) const
        {
            //return parent==that.parent && iChild==that.iChild;
            return cache == that.cache;
        }

        bool operator!= (const NodeRef &that) const
        {
            return !(operator==(that));
        }
    };

    CsysExprTrie() : root(0)
    {
    }

    ~CsysExprTrie()
    {
    }

    void Init(NodeRef &node)
    {
        Deinit();

        node.cache = 0;
        node.parent = 0;
        node.iChild = 0;
        root = 0;
    }

    void Deinit()
    {
        if (root)
        {
            DisposeNode(root);
            root = 0;
        }
    }

    bool NodeCreated(const NodeRef &node)
    {
        return node.cache != 0;
    }

    bool NodeForFunc(const NodeRef &node)
    {
        return true;
    }

    bool NodeForArray(const NodeRef &node)
    {
        return true;
    }
    
    void GetChild(NodeRef &cur, int i, NodeRef &child)
    {
        child.cache = cur.cache->subnodes[i];
        child.parent = cur.cache;
        child.iChild = i;
    }

    void GoChild(NodeRef &cur, int i)
    {
        cur.parent = cur.cache;
        cur.cache = cur.cache->subnodes[i];
        cur.iChild = i;
    }

    int NumChildren(NodeRef &cur)
    {
        return (int)cur.cache->subnodes.size();
    }

    void CloseNode(NodeRef &node, int fullness, typename Token::TokenId tokenId)
    {
        node.cache->subnodes.resize(fullness);
    }

    bool Stretch(NodeRef &node, int i, NodeRef &out)
    {
        if ( i >= (int)node.cache->subnodes.size() )
        {
            node.cache->subnodes.resize(i + 1);
        }
        out.parent = node.cache;
        out.iChild = i;
        out.cache = out.parent->subnodes[i] = 0;
        return true;
    }

    bool PassNode(NodeRef &node, NodeRef &newNode)
    {
        if (node.cache->GetPrio() < newNode.cache->GetPrio())
        {
            return true;
        }
        if (node.cache->IsAssign() && newNode.cache->IsAssign())
        {
            return true;
        }
        return false;
    }

    void FindNodeAsLeft(NodeRef &node, NodeRef &newNode,
        NodeRef &cmpBegin, NodeRef &cmpEnd)
    {
        Node    *p = cmpBegin.cache;
        Node    *pParent = cmpBegin.parent;
        int     iChild = cmpBegin.iChild;

        while (1)
        {
            if (p == cmpEnd.cache)
            {
                break;
            }
            if (p->GetPrio() >= newNode.cache->GetPrio()
                && !(p->IsAssign() && newNode.cache->IsAssign()))
            {
                break;
            }
            pParent = p;
            iChild = (int)p->subnodes.size() - 1;
            p = p->subnodes[iChild];
        }
        node.cache = p;
        node.iChild = iChild;
        node.parent = pParent;
    }

    bool ReplaceAndPutLeft(NodeRef &left, NodeRef &newNode)
    {
        /* both */
        newNode.parent = left.parent;
        newNode.iChild = left.iChild;
        if (newNode.parent)
        {
            newNode.parent->subnodes[newNode.iChild] = newNode.cache;
        }
        else
        {
            root = newNode.cache;
        }
        newNode.cache->subnodes[0] = left.cache;

        left.parent = newNode.cache;
        left.iChild = 0;
        return true;
    }

    bool CreateAsTerm(NodeRef &node, Token &token)
    {
        Node *p = new Node();
        p->name = token.GetName();
        p->type = token.GetType();  // name or const
        node.cache = p;
        if (node.parent)
        {
            node.parent->subnodes[node.iChild] = p;
        }
        else
        {
            root = p;
        }
        return true;
    }
    
    bool CreateAsFunc(NodeRef &node, typename Token::TokenId tokenId)
    {
        Node *p = node.cache = new Node();
        p->subnodes.resize(1);
        if (tokenId == 1)
            p->type = k_Func;
        else
            p->type = k_Array;
        return true;
    }

    bool CreateAsTuple(NodeRef &node, NodeRef &oldNode)
    {
        Node *p = node.cache = new Node();
        p->subnodes.resize(1);
        p->type = k_Tuple;
        return true;
    }

    bool CreateAsUnary(NodeRef &node, Token &token)
    {
        Node *p = node.cache = new Node();
        p->subnodes.resize(1);
        p->type = token.GetType();
        return true;
    }

    bool CreateAsBinary(NodeRef &node, Token &token)
    {
        Node *p = node.cache = new Node();
        p->subnodes.resize(2);
        p->type = token.GetType();
        return true;
    }

    bool CreateAsHighOp(NodeRef &node, Token &token)
    {
        Node *p = node.cache = new Node();
        p->subnodes.resize(3);  // always 3
        p->type = token.GetType();
        return true;
    }

    bool SameHighOp(NodeRef &node, Token &token)
    {
        return (node.cache->type == k_Query && (token.GetType() == k_Query || token.GetType() == k_Query2));
    }

    void Print()
    {
        int depth = 0;
        if (!root)
        {
            return;
        }
        PrintSubtrie(root, 0, true);
        printf("\n");
    }

protected:
    void DisposeNode(Node *node)
    {
        size_t nSubNodes = node->subnodes.size();
        for (size_t i = 0; i < nSubNodes; i++)
        {
            if (node->subnodes[i])
            {
                DisposeNode(node->subnodes[i]);
                node->subnodes[i] = 0;
            }
        }
        delete node;
    }


    void PrintSubtrie(Node *p, int depth, bool cont)
    {
        const char *k_BlankCol = "        ";
        const int k_ColWidth = (int)strlen(k_BlankCol);
        int i, len;
        int numSymbol = sizeof(CsysCommon::k_Symbol)/sizeof(CsysCommon::k_Symbol[0]);

        if (!cont)
        {   // new line
            printf("\n");
            for (i = 0; i < depth; i++)
            {
                printf("%s", k_BlankCol);
            }
        }

        if (p)
        {
            if (p->type < numSymbol)
            {
                const char *pszSymbol = CsysCommon::k_Symbol[p->type];
                len = (int)strlen(pszSymbol);
                printf("%s", pszSymbol);
            }
            else
            {
                len = (int)p->name.length();
                if (len > k_ColWidth - 1)
                {
                    p->name = p->name.substr(0, k_ColWidth - 1);
                    len = k_ColWidth - 1;
                }
                printf("%s", p->name.c_str());
            }

            for (i = len; i < k_ColWidth; i++)
            {
                printf(" ");
            }
        }

        for (i = 0; i < (int)p->subnodes.size(); i++)
        {
            PrintSubtrie(p->subnodes[i], depth + 1, i == 0);
        }
    }

protected:
    Node    *root;

};


template <class ChrItr, class TypeList = CsysTypeList>
struct CsysExprTraits
{
    typedef CsysToken<ChrItr, TypeList> Token;
    typedef CsysExprTrie<Token>         TrieAgent;
    typedef typename TrieAgent::NodeRef Node;
    typedef unsigned long               Count;
    typedef typename Token::TokenId     TokenId;

    template <class Sep>
    struct StackWrap
    {
        typedef std::stack<Sep>     Stack;
    };

    static const bool k_TrieFind = true;
};

}   /* using namespace qtl */

#endif  /* _csysexpr_H */