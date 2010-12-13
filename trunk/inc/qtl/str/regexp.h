/**
 * === String Library ===
 *  Unit Name:  regexp
 *  Author:     Lincoln Yu
 *  Purpose:    An enhanced regular expression parser.
 * == History ==        
 *  2008-03-26: Created.
 *  2008-04-01: Roughly debugged.
 *  2008-04-19: Bug fixed, the tag tree should use tag pointer as vector type instead.
 *              Bug fixed, replacer string generation.
 *  2008-04-29: '<'s followed by non-numeric elements are not regarded as shortcut leading characters.
 *              Allowing more special characters acting as common ones on occasions that they are impossible 
 *              to function.
 * == Comments ==
 *
 */

#ifndef _regexp_H
#define _regexp_H

#include "../all/predef.h"

#include <cassert>
#include <vector>
#include <stack>
#include <string>
#include <map>
#if defined(DEBUG_REGEXP) && DEBUG_REGEXP
#   include <queue>
#endif

#include "regexp_tag.h"


_QTL_BEGIN


/* class RegExpAutomata -- common automata for RegExp */

template <class PtrnItr, class TextItr, class Traits>
class RegExpAutomata
{
public:
    typedef unsigned long   Uint32;
    typedef unsigned int    Uint;
    typedef std::string     String;

    /* CountSet 
     *
     * in the form as {1,3-5,10-16}, pattern iteration
     */
    typedef Uint32  Count;
    struct CountRange { Count lo, hi; };
    struct CountSet
    {
        typedef std::vector<CountRange>     CountVec;

        CountVec    vec;

        void Clear ();
        void Add (CountRange &range);

        static Count Infinity () { return 0xffffffff; }
    };

    typedef typename CountSet::CountVec::iterator           CountVecItr;
    typedef typename CountSet::CountVec::reverse_iterator   CountVecRitr;


    /* CharSet
     *
     * charset is restrained within square brackets
     */
    typedef unsigned char   Char;
    struct CharSet
    {
        typedef unsigned char   Char;
        // bitmap representing 256 characters
        Uint32  bmp[8];

        CharSet ();
        CharSet (const CharSet &copy);
        ~CharSet ();

        void Init (bool exclusive);
        void Add (Char ch, bool exclusive);
        void AddRange (Char lo, Char hi, bool exclusive);
        bool Contain (Char ch) const;
        bool IsSingleChar (Char &ch) const;
        bool Empty () const;
    };

    /* Shortcut 
     *
     * e.g. <0>; <1.2.7>, instance iteration, 
     * doesn't point to any element with countset or 
     * within countset element
     */
    struct Node;
    typedef Node *NodePtr;

    typedef typename Traits::TagNode    TagNode;
    typedef TagNode *   TagPtr;
    typedef TagPtr      Shortcut;

    /* Edge */
    struct Edge
    {
        enum Type
        {
            TypeNone,
            TypeSingleChar,
            TypeAnyChar,
            TypeCharSet,
            TypeString,
            TypeShortcut,
        } type;

        union
        {
            Char        ch;
            TagPtr      shortcut;
            void *      data;   // for charset and string
        };

        NodePtr     dest;

        Edge ();
        Edge (const Edge &copy);
        ~Edge ();

        void AssignSingleChar (Char c);
        void AssignCharSet (const CharSet &chset);
        void AssignString (const String &s);
        void AssignShortcut (const TagPtr &shcut);

        const Char & GetSingleChar () const;
        const CharSet & GetCharSet () const;
        const String & GetString () const;
        const TagPtr & GetShortcut () const;
    };

    typedef Edge *  EdgePtr;

    /* Node */
    typedef int OutletItr;
    struct Node
    {
        enum Type
        {
            None,
            Terminal,       // Termina node
            Plain,          // Charset, shortcut, ...
            SimpleNone,     // Start node
            SimpleLeft,     // (
            SimpleRight,    // )
            Repeatable      // Coiled
        };
        Type                type;
        Node (Type t = None) : type(t) {}
    };
    struct TerminalNode : public Node
    {
        TerminalNode () : Node(Node::Terminal) {}
    };
    struct PlainNode : public Node
    {
        std::vector<Edge>   outlets;

        PlainNode (typename Node::Type t = Node::Plain) : Node(t) {}
    };
    struct SimpleNode : public PlainNode
    {
        TagPtr  pTag;

        SimpleNode (typename Node::Type t = Node::SimpleNone) 
            : PlainNode(t), pTag(NULL){}
    };
    struct RepeatableNode : public PlainNode
    {
        Count           rep;
        CountSet        countset;

        RepeatableNode () : PlainNode(Node::Repeatable), rep(0) { }
    };
    typedef std::vector<NodePtr>    NodeVec;

protected:
    typedef typename Traits::template AutomataWrap<TagNode *>::Stack    TagStack;
    typedef typename Traits::template AutomataWrap<TagNode *>::Map      TagMap;

    // Tag creator
    TagMap          m_TagMap;
    TagStack        m_TagStack;

    NodeVec         m_NodeVec;
    NodePtr         m_pStart, m_pTerminal;

public:
    RegExpAutomata ();
    ~RegExpAutomata ();

    // Creating
    NodePtr SpawnFirstNode ();  // including initialization
    NodePtr SpawnBeginNode ();  // spawn a simple node
    NodePtr SpawnEndNode ();
    NodePtr SpawnRepNode ();    // spawn a repeatable node
    protected:  NodePtr SpawnPlainNode ();
    public: NodePtr SpawnTerminal ();   // spawn a terminal node

    bool FinishNode (NodePtr pLastNode, NodePtr pEndNode, PtrnItr &lastItr, const PtrnItr &p); // including finalization

    bool CreatePath  (NodePtr pBegin, NodePtr pEnd, PtrnItr &begin, const PtrnItr &end);
    static void SetCountSet (NodePtr pNode, PtrnItr &p, const PtrnItr &end);
    bool TagShortcutOpen (NodePtr pBegin, PtrnItr &p, const PtrnItr &end);
    bool TagShortcutClose (NodePtr pBegin, NodePtr pEnd);
    static void GetShortcutText (const TagPtr &tag, TextItr &begin, TextItr &end);

    // Matching
    static bool TryEdge (NodePtr pNode, const OutletItr &i, TextItr &begin, const TextItr &end);
    static OutletItr OutletBegin (NodePtr pNode);
    static OutletItr OutletEnd   (NodePtr pNode);

    // .. Rep related
    static void  SetRep         (NodePtr pNode, const Count &rep);
    static const Count & GetRep (NodePtr pNode);
    static bool NodeRepeatable  (NodePtr pNode);
    static bool NodeForceLeave  (NodePtr pNode);
    static bool NodeAllowLeave  (NodePtr pNode);

    // .. Transition
    static void PassEdge    (NodePtr &pNode, const OutletItr &i);
    static void PassEdgeOn  (NodePtr &pNode, const OutletItr &i);
    static void PassEdgeOff (NodePtr &pNode, const OutletItr &i);

    // .. Nodes
    NodePtr Begin ();
    NodePtr End ();

#if defined(DEBUG_REGEXP) && DEBUG_REGEXP
    int GetNodeIndex (NodePtr pNode);
    static void ViewTree (TagPtr p, String name);
    void DebugView ();
#endif

protected:
    template <class PtrnItr2>
    static void SkipBlanks    (PtrnItr2 &p, const PtrnItr2 &end);
    template <class PtrnItr2>
    static bool ParseInteger  (Uint32 &val, PtrnItr2 &p, const PtrnItr2 &end);
    static bool ParseCountSet (CountSet &countset, PtrnItr &p, const PtrnItr &end);
    public: static bool ParseEscape (Char &ch, PtrnItr &p, const PtrnItr &end);
    protected: static bool ParseCharSet (CharSet &charset, PtrnItr &p, const PtrnItr &end);

public:
    template <class PtrnItr2>
    bool ParseShortcut (TagPtr &shortcut, PtrnItr2 &p, const PtrnItr2 &end);
    bool ParseShortcut (TagPtr &shortcut, PtrnItr &p, const PtrnItr &end);
    
protected:
    template <class TextItr2>
    static bool CompString    (TextItr &begin, const TextItr &end, TextItr2 &begin2, const TextItr2 &end2);

protected:
    static void Link (NodePtr begin, NodePtr end, EdgePtr &edge);

public:
    void Clear ();

};

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::CountSet::Clear ()
{
    vec.clear();
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::CountSet::Add (CountRange &range)
{
    if (range.lo > range.hi)
    {
        Count tmp = range.lo;
        range.lo = range.hi;
        range.hi = tmp;
    }

    if (vec.empty())
    {
        vec.push_back(range);
        return;
    }

    CountVecRitr ritr = vec.rbegin();
    CountRange local = *ritr;

    if (range.lo > local.hi)
    {
        vec.push_back(range);
        return;
    }

    typename CountVec::reverse_iterator begin_ritr = vec.rbegin(), end_ritr = vec.rend();
    Count hi = (begin_ritr->hi < range.hi) ? range.hi : begin_ritr->hi;
    Count lo = (vec[0].lo < range.lo) ? vec[0].lo : range.lo;
    for ( ; ritr != vec.rend(); ++ritr)
    {
        if (range.hi < ritr->lo)
        {
            begin_ritr = ritr; ++begin_ritr;
            hi = (begin_ritr->hi < range.hi) ? range.hi : begin_ritr->hi;
        }
        if (ritr->hi < range.lo)
        {
            end_ritr = ritr;
            --ritr;
            lo = (ritr->lo < range.lo) ? ritr->lo : range.lo;
            break;
        }
    }

    begin_ritr->hi = hi;
    end_ritr->lo = lo;

    ++begin_ritr;
    vec.erase(end_ritr.base(), begin_ritr.base());
}

template <class PtrnItr, class TextItr, class Traits>
RegExpAutomata<PtrnItr, TextItr, Traits>::CharSet::CharSet ()
{
    Init(false);
}

template <class PtrnItr, class TextItr, class Traits>
RegExpAutomata<PtrnItr, TextItr, Traits>::CharSet::CharSet (const CharSet &copy)
{
    memcpy(bmp, copy.bmp, 8 * sizeof(Uint32));
}

template <class PtrnItr, class TextItr, class Traits>
RegExpAutomata<PtrnItr, TextItr, Traits>::CharSet::~CharSet ()
{
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::CharSet::Init (bool exclusive)
{
    if (exclusive)
    {
        memset(bmp, 0xff, 8 * sizeof(Uint32));
    }
    else
    {
        memset(bmp, 0, 8 * sizeof(Uint32));
    }
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::CharSet::Add (Char ch, bool exclusive)
{
    if (exclusive)
    {
        bmp[ch >> 5] &= ~(1UL << (ch & 0x1f));
    }
    else
    {
        bmp[ch >> 5] |= 1UL << (ch & 0x1f);
    }
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::CharSet::AddRange (Char lo, Char hi, bool exclusive)
{
    if (lo > hi) 
    {
        Char tmp = lo;
        lo = hi;
        hi = tmp;
    }
    int idx = lo >> 5;
    int diff = hi - lo + 1;
    Uint32  shift = 1UL << (lo & 0x1f);
    for (int i = 0; i < diff; i++)
    {
        if (exclusive)
        {
            bmp[idx] &= ~shift;
        }
        else
        {
            bmp[idx] |= shift;
        }
        if (shift == 0x80000000)
        {
            shift = 1;
            idx++;
        }
        else
        {
            shift <<= 1;
        }
    }
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::CharSet::Contain (Char ch) const
{
    return (bmp[ch >> 5] & (1UL << (ch & 0x1f))) != 0;
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::CharSet::IsSingleChar (Char &ch) const
{
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        if (bmp[i])
        {
            if (count) return false;
            Char u = 1;
            int  j;
            for (j = 0; j < 32; j++, u <<= 1)
            {
                if (bmp[i] == j)
                {
                    ch = (i<<3) + j;
                    count += 1;
                    break;
                }
            }
            if (j == 32) return false;
        }
    }
    return (count == 1);
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::CharSet::Empty () const
{
    for (int i = 0; i < 8; i++)
    {
        if (bmp[i])
        {
            return false;
        }
    }
    return true;
}

template <class PtrnItr, class TextItr, class Traits>
RegExpAutomata<PtrnItr, TextItr, Traits>::Edge::Edge () 
    : type(TypeNone), dest(NULL), data(NULL)
{
}

template <class PtrnItr, class TextItr, class Traits>
RegExpAutomata<PtrnItr, TextItr, Traits>::Edge::Edge (const Edge &copy)
    : type (copy.type), dest(copy.dest)
{
    switch (type)
    {
    case TypeSingleChar:
        ch = copy.ch;
        break;
    case TypeCharSet:
        {
            const CharSet &chset = *static_cast<CharSet *>(copy.data);
            CharSet *p = new CharSet(chset);
            data = p;
        }
        break;
    case TypeString:
        {
            const String &str = *static_cast<String *>(copy.data);
            String *p = new String(str);
            data = p;
        }
        break;
    case TypeShortcut:
        shortcut = copy.shortcut;
    }
}

template <class PtrnItr, class TextItr, class Traits>
RegExpAutomata<PtrnItr, TextItr, Traits>::Edge::~Edge ()
{
    switch (type)
    {
    case TypeSingleChar:
        break;
    case TypeCharSet:
        if (data) { delete static_cast<CharSet *>(data); }
        break;
    case TypeString:
        if (data) { delete static_cast<String *>(data); }
        break;
    case TypeShortcut:
        break;
    }
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::Edge::AssignSingleChar (Char c)
{
    ch = c;
    type = TypeSingleChar;
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::Edge::AssignCharSet (const CharSet &chset)
{
    CharSet *p = new CharSet(chset);
    data = static_cast<void *>(p);
    type = TypeCharSet;
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::Edge::AssignString (const String &s)
{
    String *p = new String(s);
    data = static_cast<void *>(p);
    type = TypeString;
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::Edge::AssignShortcut (const TagPtr &shcut)
{
    shortcut = shcut;
    type = TypeShortcut;
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::Char const & RegExpAutomata<PtrnItr, TextItr, Traits>::Edge::GetSingleChar () const
{
    return ch;
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::CharSet const & RegExpAutomata<PtrnItr, TextItr, Traits>::Edge::GetCharSet () const
{
    return *static_cast<CharSet *>(data);
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::String const & RegExpAutomata<PtrnItr, TextItr, Traits>::Edge::GetString () const
{
    return *static_cast<String *>(data);
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::TagPtr const & RegExpAutomata<PtrnItr, TextItr, Traits>::Edge::GetShortcut () const
{
    return shortcut;
}

template <class PtrnItr, class TextItr, class Traits>
RegExpAutomata<PtrnItr, TextItr, Traits>::RegExpAutomata ()
    : m_pStart(NULL), m_pTerminal(NULL)
{
}

template <class PtrnItr, class TextItr, class Traits>
RegExpAutomata<PtrnItr, TextItr, Traits>::~RegExpAutomata ()
{
    Clear();
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::NodePtr 
    RegExpAutomata<PtrnItr, TextItr, Traits>::SpawnFirstNode ()
{
    // node creation
    m_pStart = new SimpleNode();
    m_NodeVec.push_back(m_pStart);

    return m_pStart;
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::NodePtr RegExpAutomata<PtrnItr, TextItr, Traits>::SpawnBeginNode ()
{
    NodePtr pNode = new SimpleNode(Node::SimpleLeft);
    m_NodeVec.push_back(pNode);

    return pNode;
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::NodePtr RegExpAutomata<PtrnItr, TextItr, Traits>::SpawnEndNode ()
{
    NodePtr pNode = new SimpleNode(Node::SimpleRight);
    m_NodeVec.push_back(pNode);

    return pNode;
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::NodePtr RegExpAutomata<PtrnItr, TextItr, Traits>::SpawnPlainNode ()
{
    NodePtr pNode = new PlainNode();
    m_NodeVec.push_back(pNode);

    return pNode;
}


template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::NodePtr RegExpAutomata<PtrnItr, TextItr, Traits>::SpawnRepNode ()
{
    NodePtr pNode = new RepeatableNode();
    m_NodeVec.push_back(pNode);

    return pNode;
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::NodePtr RegExpAutomata<PtrnItr, TextItr, Traits>::SpawnTerminal ()
{
    NodePtr pNode = new TerminalNode();
    m_NodeVec.push_back(pNode);

    return pNode;
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::FinishNode (NodePtr pLastNode, NodePtr pEndNode, PtrnItr &lastItr, const PtrnItr &p)
{
    if (!pEndNode)
    {
        pEndNode = SpawnTerminal();
    }

    m_pTerminal = pEndNode;
    
    return CreatePath(pLastNode, pEndNode, lastItr, p);
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::CreatePath (NodePtr pBegin, NodePtr pEnd, PtrnItr &begin, const PtrnItr &end)
{
    String      local;
    Char        ch;
    TagPtr      shortcut;
    CharSet     charset;
    CountSet    countset;

    PtrnItr     &p = begin;
    NodePtr     pLastNode, pNode;
    EdgePtr     pEdge = NULL;

    enum
    {
        TypeSingleChar,
        TypeCharSetChar,
        TypeShortcut,
        TypeAnyChar,
    } type;

    pLastNode = pBegin;
    for ( ; p != end; )
    {
        if (*p == '[')
        {
            if (!ParseCharSet(charset, p, end))
            {
                begin = p;
                return false;
            }
            type = charset.IsSingleChar(ch)? TypeSingleChar : TypeCharSetChar;
        }
        else if (*p == '.')
        {
            ++p;
            type = TypeAnyChar;
        }
        else if (*p == '\\')
        {
            PtrnItr pTry = p;
            ++pTry;
            if (*pTry == 'k')
            {
                p = pTry;
                ++p;    // 'p' is now expected to be at '<'
                if (!ParseShortcut(shortcut, p, end))
                {
                    begin = p;
                    return false;
                }
                type = TypeShortcut;
            }
            else
            {
                if (!ParseEscape(ch, p, end))
                {
                    begin = p;
                    return false;
                }
                type = TypeSingleChar;
            }
        }
        else
        {   // plain char
            PtrnItr pn = p;
            ++pn;
            ch = *p;
            p = pn;
            type = TypeSingleChar;
        }

        if (p != end && (*p == '{' || *p == '*' || *p == '+'))
        {
            pNode = SpawnRepNode();
            RepeatableNode *pRepNode = static_cast<RepeatableNode *>(pNode);
            if (!ParseCountSet(pRepNode->countset, p, end))
            {
                begin = p;
                return false;
            }
            
            Link(pLastNode, pNode, pEdge);
            pEdge->AssignString(local);

            Link(pNode, pNode, pEdge);
            pLastNode = pNode;

            switch (type)
            {
            case TypeSingleChar:
                pEdge->AssignSingleChar(ch);
                break;
            case TypeCharSetChar:
                pEdge->AssignCharSet(charset);
                break;
            case TypeAnyChar:
                pEdge->type = Edge::TypeAnyChar;
                break;
            case TypeShortcut:
                pEdge->AssignShortcut(shortcut);
                break;
            }

            local = "";
        }
        else
        {
            switch (type)
            {
                case TypeSingleChar:
                    local += ch;

                    if (p == end)
                    {
                        Link(pLastNode, pEnd, pEdge);
                        if (local.size() == 1)
                        {
                            pEdge->AssignSingleChar(ch);
                        }
                        else
                        {
                            pEdge->AssignString(local);
                        }
                        return true;
                    }
                    break;
                case TypeCharSetChar: case TypeAnyChar:
                    if (local != "")
                    {
                        pNode = SpawnPlainNode();
                        Link(pLastNode, pNode, pEdge);
                        pEdge->AssignString(local);
                        pLastNode = pNode;
                    }

                    if (p != end)
                    {
                        pNode = SpawnPlainNode();
                    }
                    else
                    {
                        pNode = pEnd;
                    }

                    Link(pLastNode, pNode, pEdge);

                    if (type == TypeCharSetChar)
                    {
                        pEdge->AssignCharSet(charset);
                    }
                    else    // TypeAnyChar
                    {
                        pEdge->type = Edge::TypeAnyChar;
                    }

                    if (p == end)
                    {
                        return true;
                    }
                    
                    pLastNode = pNode;
                    local = "";

                    break;
                case TypeShortcut:
                    if (local != "")
                    {
                        pNode = SpawnPlainNode();
                        Link(pLastNode, pNode, pEdge);
                        pEdge->AssignString(local);
                        pLastNode = pNode;
                    }

                    if (p == end)
                    {
                        Link(pLastNode, pEnd, pEdge);
                        pEdge->AssignShortcut(shortcut);
                        return true;
                    }

                    pNode = SpawnPlainNode();
                    Link(pLastNode, pNode, pEdge);
                    pEdge->AssignShortcut(shortcut);

                    pLastNode = pNode;
                    local = "";
                    break;
            }
        }
    }

    Link(pLastNode, pEnd, pEdge);
    pEdge->AssignString(local);

    return true;
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::TryEdge (NodePtr pNode, const OutletItr &i, TextItr &begin, const TextItr &end)
{
    EdgePtr pEdge = &static_cast<PlainNode *>(pNode)->outlets[i];
    switch (pEdge->type)
    {
    case Edge::TypeSingleChar:
        {
            if (*begin != pEdge->ch)
            {
                return false;
            }
            ++begin;
            return true;
        }
    case Edge::TypeString:
        {
            const String &str = pEdge->GetString();
            String::const_iterator itrstr = str.begin();
            const String::const_iterator &strend = str.end();
            return CompString<String::const_iterator>(begin, end, itrstr, strend);
        }
    case Edge::TypeCharSet:
        {
            const CharSet &charset = pEdge->GetCharSet();
            if (!charset.Contain(*begin))
            {
                return false;
            }
            ++begin;
            return true;
        }
    case Edge::TypeAnyChar:
        {
            ++begin;
            return true;
        }
    case Edge::TypeShortcut:
        {
            const TagPtr &shortcut = pEdge->shortcut;
            TextItr tagBegin;
            TextItr tagEnd;

            if (!pEdge->shortcut->GetTextItr(tagBegin, tagEnd))
            {   // shortcut undefined
                return false;
            }

            return CompString<TextItr>(begin, end, tagBegin, tagEnd);
        }
    }
    return false;
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::OutletItr RegExpAutomata<PtrnItr, TextItr, Traits>::OutletBegin (NodePtr pNode)
{
    return 0;   // always 0
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::OutletItr RegExpAutomata<PtrnItr, TextItr, Traits>::OutletEnd (NodePtr pNode)
{
    return static_cast<OutletItr>(static_cast<PlainNode *>(pNode)->outlets.size());
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::SetCountSet (NodePtr pNode, PtrnItr &p, const PtrnItr &end)
{
    RepeatableNode *pRepNode = static_cast<RepeatableNode *>(pNode);
    ParseCountSet(pRepNode->countset, p, end);
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::TagShortcutOpen (NodePtr pBegin, PtrnItr &p, const PtrnItr &end)
{
    if (*p == '?')
    {   /*  shortcut (backreference) for this paranthesis  */
        ++p;
    }
    else
    {
        /* TODO: to support implicit numbered tag, start from here */
        return true;
    }

    // p is intially pointing to '<'
    // parse the signature of the tag (for backreference)
    if (p == end || *p != '<') return false;
    PtrnItr pTagNameBegin = ++p;
    for (; p != end && *p != '>'; ++p)
    {
    }
    if (p == end) return false;
    PtrnItr pTagNameEnd = p;
    ++p;

    TagNode *pTagNode = m_TagMap.map<PtrnItr>(pTagNameBegin, pTagNameEnd);
    if (pTagNode == NULL)
    {
        pTagNode = new TagNode();
        m_TagMap.add<PtrnItr>(pTagNameBegin, pTagNameEnd, pTagNode);
    }
    m_TagStack.push(pTagNode);

    SimpleNode *pSimpleBegin = static_cast<SimpleNode *>(pBegin);
    pSimpleBegin->pTag = pTagNode;

    return true;
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::TagShortcutClose (NodePtr pBegin, NodePtr pEnd)
{
    if (m_TagStack.empty())
    {
        return true;
    }

    TagNode *pTagNode = m_TagStack.top();
    m_TagStack.pop();

    SimpleNode *pSimpleEnd = static_cast<SimpleNode *>(pEnd);
    pSimpleEnd->pTag = pTagNode;
    return true;
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::GetShortcutText (const TagPtr &tag, TextItr &begin, TextItr &end)
{
    begin = tag->begin;
    end = tag->end;
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::SetRep (NodePtr pNode, const Count &rep)
{
    RepeatableNode *pRepNode = static_cast<RepeatableNode *>(pNode);
    pRepNode->rep = rep;
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::Count const & RegExpAutomata<PtrnItr, TextItr, Traits>::GetRep (NodePtr pNode)
{
    const RepeatableNode *pRepNode = static_cast<RepeatableNode *>(pNode);
    return pRepNode->rep;
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::NodeRepeatable (NodePtr pNode)
{
    return (pNode->type == Node::Repeatable);
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::NodeForceLeave (NodePtr pNode)
{
    const RepeatableNode *pRepNode = static_cast<RepeatableNode *>(pNode);
    const CountSet  &countset = pRepNode->countset;
    typename CountSet::CountVec::const_reverse_iterator ritr = countset.vec.rbegin();
    return (pRepNode->rep == ritr->hi);
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::NodeAllowLeave (NodePtr pNode)
{
    const RepeatableNode *pRepNode = static_cast<RepeatableNode *>(pNode);
    const CountSet  &countset = pRepNode->countset;
    typename CountSet::CountVec::const_reverse_iterator p = countset.vec.rbegin();
    Count rep = pRepNode->rep;
    for ( ; p != countset.vec.rend(); ++p)
    {
        if (rep >= p->lo && rep <= p->hi)
        {
            return true;
        }
    }
    return false;
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::PassEdge (NodePtr &pNode, const OutletItr &i)
{
    PlainNode *pPlainNode = static_cast<PlainNode *>(pNode);
    pNode = pPlainNode->outlets[i].dest;
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::PassEdgeOn (NodePtr &pNode, const OutletItr &i)
{
    RepeatableNode *pRepNode = static_cast<RepeatableNode *>(pNode);
    ++pRepNode->rep;
    PassEdge(pNode, i);
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::PassEdgeOff (NodePtr &pNode, const OutletItr &i)
{
    RepeatableNode *pRepNode = static_cast<RepeatableNode *>(pNode);
    pRepNode->rep = 0;
    PassEdge(pNode, i);
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::NodePtr RegExpAutomata<PtrnItr, TextItr, Traits>::Begin ()
{
    return m_pStart;
}

template <class PtrnItr, class TextItr, class Traits>
typename RegExpAutomata<PtrnItr, TextItr, Traits>::NodePtr RegExpAutomata<PtrnItr, TextItr, Traits>::End ()
{
    return m_pTerminal;
}

template <class PtrnItr, class TextItr, class Traits> template <class PtrnItr2>
void RegExpAutomata<PtrnItr, TextItr, Traits>::SkipBlanks (PtrnItr2 &p, const PtrnItr2 &end)
{
    for ( ; p != end && *p == ' '; ++p);
}

template <class PtrnItr, class TextItr, class Traits> template <class PtrnItr2>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::ParseInteger (Uint32 &val, PtrnItr2 &p, const PtrnItr2 &end)
{
    val = 0;
    PtrnItr2 old = p;
    Char    ch;
    for ( ; p != end && (ch = *p) >= '0' && *p <= '9'; ++p)
    {
        val *= 10;
        val += ch - '0';
    }
    return (p != old);
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::ParseCountSet (CountSet &countset, PtrnItr &p, const PtrnItr &end)
{
    CountRange  cr;
    Count       cr_hi;

    if (*p == '*')
    {   
        cr.lo = 0; cr.hi = CountSet::Infinity();
        countset.Add(cr);
        ++p;
        return true;
    }
    else if (*p == '+')
    {
        cr.lo = 1; cr.hi = CountSet::Infinity();
        countset.Add(cr);
        ++p;
        return true;
    }

    ++p;  // skip '{'
    countset.Clear();
    while ( p != end )
    {
        SkipBlanks<PtrnItr>(p, end);
        if (p == end) { return false; }

        if (!ParseInteger<PtrnItr>(cr.lo, p, end))
        {
            return false;
        }
        cr_hi = cr.lo;

        SkipBlanks<PtrnItr>(p, end);
        if (p == end) { return false; }

        if (*p == ',')
        {
            ++p;
            SkipBlanks<PtrnItr>(p, end);
            if (p == end) { return false; }

            if (*p == '}')
            {
                ++p;
                cr.hi = CountSet::Infinity();
                countset.Add(cr);
                return true;
            }
            else if (ParseInteger<PtrnItr>(cr_hi, p, end))
            {   
                SkipBlanks<PtrnItr>(p, end);
            }
            else
            {   // error
                return false;
            }
        }
        
        if (*p == ';')
        {
            cr.hi = cr_hi;
            countset.Add(cr);
            ++p;
        }
        else if (*p == '}')
        {
            cr.hi = cr_hi;
            countset.Add(cr);
            ++p;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::ParseEscape (Char &ch, PtrnItr &p, const PtrnItr &end)
{
    ++p;    // skip '\\'
    if (p == end) { return false; }

    if (*p == 'x')
    {
        Char c;
        ++p;

        /* A proposal for feature in the future:
         * multi-char support
         * e.g. \x2.ab denotes the 8-bit charactor 0xab, which is
         * backward compatible with \xab
         * while, \x4.ab23 denotes 16-bit dual char {0xab,0x23}
         */
        ch = 0;
        for (int i = 0; i < 2; ++i, ++p)
        {
            if (p == end) { return false; }

            Char    val;
            c = *p;
            if (c >= '0' && c <= '9')
            {
                val = c - '0';
            }
            else if (c >= 'A' && c <= 'F')
            {
                val = c - 'A' + 10;
            }
            else if (c >= 'a' && c <= 'f')
            {
                val = c - 'a' + 10;
            }
            else
            {
                return false;
            }
            ch <<= 4;
            ch |= val;
        }
    }
    else if (*p == 'n')
    {
        ch = '\n';
        ++p;
    }
    else if (*p == 't')
    {
        ch = '\t';
        ++p;
    }
    else
    {
        ch = *p;
        ++p;
    }
    return true;
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::ParseCharSet (CharSet &charset, PtrnItr &p, const PtrnItr &end)
{
    ++p;  // skip '['
    bool exclusive = *p == '^';
    enum
    {
        NoChar = 0,
        OneChar,
        Waiting,
    } state = NoChar;
    Char    ch, lastCh;

    if (exclusive)
    {
        ++p;
    }

    charset.Init(exclusive);

    for ( ; p != end; )
    {
        if (*p == '\\')
        {
            if (!ParseEscape(ch, p, end)) 
            { 
                return false;
            }
        }
        else if (*p != ']')
        {
            ch = *p++;
        }
        else
        {   // ']'
            if (state == OneChar)
            {
                charset.Add(lastCh, exclusive);
            }
            else if (state == Waiting)
            {
                charset.Add(lastCh, exclusive);
                charset.Add('-', exclusive);
            }
            ++p;
            return !charset.Empty();
        }

        switch (state)
        {
        case NoChar:
            lastCh = ch;
            state = OneChar;
            break;
        case OneChar:
            charset.Add(lastCh, exclusive);
            lastCh = ch;
            break;
        case Waiting:
            charset.AddRange(lastCh, ch, exclusive);
            state = NoChar;
            break;
        }
        
        if (state == OneChar && *p == '-')
        {
            state = Waiting;
            ++p;
        }
    }
    return false;
}

template <class PtrnItr, class TextItr, class Traits> template <class PtrnItr2>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::ParseShortcut (TagPtr &shortcut, PtrnItr2 &p, const PtrnItr2 &end)
{
    // is now one character after "\k", expecting '<'
    ++p;  // skip '<'
    PtrnItr2 pTagNameBegin = p;
    for ( ; p != end && *p != '>'; ++p)
    {
    }
    if (p == end) return false;

    PtrnItr2 pTagNameEnd = p;
    ++p;

    shortcut = m_TagMap.map<PtrnItr2>(pTagNameBegin, pTagNameEnd);

    return shortcut != NULL;
}

template <class PtrnItr, class TextItr, class Traits>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::ParseShortcut (TagPtr &shortcut, PtrnItr &p, const PtrnItr &end)
{
    return ParseShortcut<PtrnItr>(shortcut, p, end);
}


template <class PtrnItr, class TextItr, class Traits> template <class TextItr2>
bool RegExpAutomata<PtrnItr, TextItr, Traits>::CompString (TextItr &begin, const TextItr &end, 
                                 TextItr2 &begin2, const TextItr2 &end2)
{
    for ( ; begin != end && begin2 != end2; ++begin, ++begin2)
    {
        if (*begin != *begin2)
        {
            return false;
        }
    }
    return (begin2 == end2);
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::Link (NodePtr begin, NodePtr end, EdgePtr &edge)
{
    PlainNode *pPlainBegin = static_cast<PlainNode *>(begin);
    pPlainBegin->outlets.push_back(Edge());
    edge = &(*pPlainBegin->outlets.rbegin());
    edge->dest = end;
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::Clear ()
{
    // Clear nodes
    for (typename NodeVec::iterator p = m_NodeVec.begin(); p != m_NodeVec.end(); ++p)
    {
        NodePtr pNode = *p;
        switch (pNode->type)
        {
        case Node::SimpleNone: case Node::SimpleLeft: case Node::SimpleRight:
            delete static_cast<SimpleNode *>(pNode);
            break;
        case Node::Repeatable:
            delete static_cast<RepeatableNode *>(pNode);
            break;
        case Node::Plain:
            delete static_cast<PlainNode *>(pNode);
            break;
        case Node::Terminal:
            delete static_cast<TerminalNode *>(pNode);
            break;
        default:    /* Node::None, error case */
            delete pNode;
            break;
        }
    }
    m_NodeVec.clear();
    m_TagMap.Clear();
}

/* class RegExpStack */
template <class T>
class RegExpStack
{
    typedef std::stack<T>   StackType;
    StackType   data;
    int         depth;

    struct Exception
    {
        enum Type
        {
            Unknown,
            Overflow,
            Underflow
        } type;

        Exception (Type t) : type(t) {}
    };

    enum { kMaxDepth = 0x7fffffff };

public:
    struct Specifier
    {
        int dummy;
        Specifier () {}
    };

public:
    RegExpStack () : depth(0) {}
    RegExpStack (const Specifier &) : depth(0) {}
    int size() { assert(data.size() == depth); return depth; }
    bool empty () { return depth == 0; }
    void push (const T &t);
    void pop ();
    T & top ();
};

template <class T>
void RegExpStack<T>::push (const T &t)
{
    if (++depth >= kMaxDepth) 
    {
        throw Exception(Exception::Overflow);
    }
    data.push(t);
}

template <class T>
void RegExpStack<T>::pop ()
{
    if (depth-- <= 0)
    {
        throw Exception(Exception::Underflow);
    }
    data.pop();
}

template <class T>
T & RegExpStack<T>::top ()
{
    if (depth == 0) 
    { 
        throw Exception(Exception::Underflow);
    }
    return data.top();
}

/* class TagMap */

template <class T>
struct TagMap
{
    typedef std::map<std::string, T>    InnerMap;
    typedef typename InnerMap::iterator InnerMapItr;
    InnerMap    m_Map;

    template <class PtrnItr>
    void add(PtrnItr pTagNameBegin, PtrnItr pTagNameEnd, T pTagNode)
    {
        std::string s;
        for (PtrnItr p = pTagNameBegin; p != pTagNameEnd; ++p)
        {
            s += *p;
        }
        m_Map[s] = pTagNode;
    }

    template <class PtrnItr>
    T map(PtrnItr pTagNameBegin, PtrnItr pTagNameEnd)
    {
        std::string s;
        for (PtrnItr p = pTagNameBegin; p != pTagNameEnd; ++p)
        {
            s += *p;
        }
        InnerMapItr itr = m_Map.find(s);
        if (itr != m_Map.end())
        {
            return itr->second;
        }
        else
        {
            return (T)0;
        }
    }

    void Clear()
    {   /* clear allocated nodes */
        for (InnerMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
        {
            delete itr->second;
        }
    }
};


/* class RegExpTraits - common traits for RegExp */

template <class PtrnItr, class TextItr>
struct RegExpTraits
{
    template <class T>
    struct AutomataWrap
    {
        typedef RegExpStack<T>  Stack;
        typedef TagMap<T>       Map;
    };

    typedef EasyTagNode<TextItr>                TagNode;
    typedef RegExpAutomata<PtrnItr, TextItr, RegExpTraits>  Automata;
    typedef typename Automata::NodePtr          NodePtr;
    typedef typename Automata::OutletItr        OutletItr;
    typedef typename Automata::Count            Count;
    typedef std::vector<TextItr>                CoilVec;
    typedef EasyTagState<TextItr>               TagState;
    typedef typename Automata::Shortcut         Shortcut;

    /* 
     * With this kind of cute trick, C++ group could really
     * refute any proposal of type definition of template
     */

    template <class T>
    struct ParsingWrap
    {
        typedef RegExpStack<T>      Stack;
    };

    template <class T>
    struct RuntimeWrap
    {
        typedef RegExpStack<T>      Stack;
    };

};

/* class RegExp */

template <class _PtrnItr = char *, class _TextItr = char *, 
          class _Traits = RegExpTraits<_PtrnItr, _TextItr> >
class RegExp
{
public:
    typedef _Traits                         Traits;
    typedef _PtrnItr                        PtrnItr;
    typedef _TextItr                        TextItr;
    typedef typename _Traits::Shortcut      Shortcut;
    typedef typename _Traits::Automata      Automata;
    typedef typename _Traits::TagState      TagState;

protected:
    /* 
     * escaped characters 
     *
     * {}\[]().*+|^-
     */

    typedef typename _Traits::NodePtr       NodePtr;
    typedef typename _Traits::OutletItr     OutletItr;
    typedef typename _Traits::Count         Count;
    typedef typename _Traits::CoilVec       CoilVec;

    typedef typename CoilVec::iterator      CoilItr;

    struct ParsingState
    {
        NodePtr     pBeginNode;
        NodePtr     pEndNode;
    };

    struct RuntimeState
    {
        OutletItr   outletNextTry;
        NodePtr     pNode;
        Count       rep;
        _TextItr    textItr;
        TagState    tagState;
    };

    typedef typename _Traits::template ParsingWrap<ParsingState>::Stack          ParsingStack;
    typedef typename _Traits::template RuntimeWrap<RuntimeState>::Stack          RuntimeStack;

    typedef typename RuntimeStack::Specifier        RtStkSpec;

protected:
    Automata    m_Automata;

public:
    RegExp () {}
    RegExp (Automata &automata) : m_Automata(automata) {}
    ~RegExp () {}

    bool SetPattern (_PtrnItr &begin, const _PtrnItr &end);
    bool TryMatch (const _TextItr &begin, const _TextItr &end, _TextItr &p);
    bool TryMatch (const _TextItr &begin, const _TextItr &end, _TextItr &p, RtStkSpec &spec);

    template <class PtrnItr2>
    bool ParseShortcut (Shortcut &shortcut, PtrnItr2 &begin, const PtrnItr2 &end);
    static bool RunShortcut (Shortcut &shortcut, _TextItr &begin, _TextItr &end);

    template <class PtrnItr2>
    bool GetShortcut (_TextItr &tbegin, _TextItr &tend, PtrnItr2 &pbegin, const PtrnItr2 &pend);

    Automata & GetAutomata () { return m_Automata; }
    void AssignAutomata (Automata &automata) { m_Automata = automata; }

protected:
    static bool NodeBindText (RuntimeStack &stk, NodePtr pNode, TextItr &p);

    bool _TryMatch (const _TextItr &begin, const _TextItr &end, _TextItr &m, RuntimeStack &stk);

    bool Preparse (CoilVec &coil, _PtrnItr &begin, const _PtrnItr &end);

#if defined(DEBUG_REGEXP) && DEBUG_REGEXP
public:
    void DebugView () { m_Automata.DebugView(); }
#endif

};

template <class _PtrnItr, class _TextItr, class _Traits>
bool RegExp<_PtrnItr, _TextItr, _Traits>::SetPattern (_PtrnItr &pParsing, const _PtrnItr &end)
{
    try
    {
        _PtrnItr        begin = pParsing;
        ParsingState    cur;        /* current state */
        CoilVec         coilVec;
        CoilItr         itrCoil;

        NodePtr         pLastNode;

        ParsingStack    stk;
        bool            inCharSet = false;

        // Reset the automata first
        m_Automata.Clear();

        /* 
         * what should be dealt with explicitly here are
         * parentheses, |, countsets, etc.
         */

        if (!Preparse(coilVec, pParsing, end))
        {
            return false;
        }
        itrCoil = coilVec.begin();

        NodePtr pStartNode = cur.pBeginNode = m_Automata.SpawnFirstNode();
        pLastNode = cur.pBeginNode;

        cur.pEndNode = NULL;
        
        stk.push(cur);

        pParsing = begin;
        _PtrnItr p = pParsing;
        _PtrnItr lastItr = p;
        _PtrnItr pDest;
        while (p != end)
        {
            if (*p == '\\')
            {
                ++p;
                if (p == end) { break; }
                ++p;
                continue;
            }

            if (*p == '[')
            {
                ++p;
                inCharSet = true;
                continue;
            }
            if (inCharSet)
            {
                if (*p == ']')
                {
                    ++p;
                    inCharSet = false;
                }
                else
                {
                    ++p;
                }
                continue;
            }
            
            if (*p == '(')
            {
                NodePtr pLPNode;
                if (itrCoil != coilVec.end() && *itrCoil == p)
                {
                    /* fix end node in this coiled case */
                    cur.pBeginNode = m_Automata.SpawnRepNode();
                    cur.pEndNode = cur.pBeginNode;
                    pLPNode = m_Automata.SpawnBeginNode();  /* left parenthesis in the coil */

                    // connect pBeginNode to pLPNode with an empty path
                    m_Automata.CreatePath(cur.pBeginNode, pLPNode, p, p);

                    ++itrCoil;

                    /* [:lastItr:] is moved to [:p:] if successful */
                    pDest = p;
                    pParsing = lastItr;
                    if (!m_Automata.CreatePath(pLastNode, cur.pBeginNode, pParsing, pDest))
                    {
                        return false;
                    }
                    pLastNode = pLPNode;
                }
                else
                {
                    pLPNode = cur.pBeginNode = m_Automata.SpawnBeginNode();
                    cur.pEndNode = NULL;    /* leave for future assignment */
                    /* [:lastItr:] is moved to [:p:] if successful */

                    pDest = p;
                    pParsing = lastItr;
                    if (!m_Automata.CreatePath(pLastNode, cur.pBeginNode, pParsing, pDest))
                    {
                        return false;
                    }
                    pLastNode = cur.pBeginNode;
                }

                ++p;    // adding to p after to have it excluded from path creation.

                if (!m_Automata.TagShortcutOpen(pLPNode, p, end))
                {
                    return false;
                }

                lastItr = p;

                stk.push(cur);

            }
            else if (*p == ')')
            {
                NodePtr pLPNode, pRPNode;
                cur = stk.top();
                stk.pop();
                assert(!stk.empty());   // assured by preparsing

                pDest = p;
                ++p;
                if ((p != end) && (*p == '{' || *p == '*' || *p == '+'))
                {
                    cur.pEndNode = cur.pBeginNode;
                    m_Automata.SetCountSet(cur.pBeginNode, p, end);
                    /* [:p:] is moved towards the position right 
                     * after the countset descriptor */

                    pLPNode = cur.pBeginNode;
                    m_Automata.PassEdge(pLPNode, 0);    // path 0 must be the coiled one

                    pRPNode = m_Automata.SpawnEndNode();  /* right parenthesis in the coil */;

                    pParsing = lastItr;
                    if (!m_Automata.CreatePath(pLastNode, pRPNode, pParsing, pDest))
                    {
                        return false;
                    }

                    m_Automata.CreatePath(pRPNode, cur.pEndNode, pDest, pDest);
                }
                else
                {
                    if (!cur.pEndNode) 
                    { 
                        cur.pEndNode = m_Automata.SpawnEndNode();
                    }

                    pParsing = lastItr;
                    if (!m_Automata.CreatePath(pLastNode, cur.pEndNode, pParsing, pDest))
                    {
                        return false;
                    }

                    pLPNode = cur.pBeginNode;
                    pRPNode = cur.pEndNode;
                }

                /* 
                 * Add a shortcut tracing node. Beginning node is provided here.
                 * The decision on whether creating the unnecessary tag node is
                 * up to the implementation.
                 * 
                 * action is taken only when there is a tag in the stack
                 */
                if (!m_Automata.TagShortcutClose(pLPNode, pRPNode))
                {
                    begin = pDest;
                    return false;
                }

                lastItr = p;    /* this is the due position [:lastItr:] should take */
                pLastNode = cur.pEndNode;

                // load the context from stk top
                cur = stk.top();
            }
            else if (*p == '|')
            {
                if (!cur.pEndNode) 
                {
                    if (cur.pBeginNode == pStartNode)
                    {   /* main path of the parsing */
                        cur.pEndNode = m_Automata.SpawnTerminal();
                    }
                    else
                    {
                        cur.pEndNode = m_Automata.SpawnEndNode();
                    }
                    stk.top().pEndNode = cur.pEndNode;
                }

                pDest = p;
                pParsing = lastItr;
                if (!m_Automata.CreatePath(pLastNode, cur.pEndNode, pParsing, pDest))
                {
                    return false;
                }

                ++p;
                lastItr = p;
                pLastNode = cur.pBeginNode;
            }
            else
            {
                ++p;
            }
        }

        cur = stk.top();
        stk.pop();
        assert(stk.empty());    // assured by preparsing

        /* add a new node pLastNode links to as terminal */
        pDest = p;
        pParsing = lastItr;
        return m_Automata.FinishNode(pLastNode, cur.pEndNode, pParsing, pDest);
    }
    catch (...)
    {
        return false;
    }
}

template <class _PtrnItr, class _TextItr, class _Traits>
bool RegExp<_PtrnItr, _TextItr, _Traits>::TryMatch (const _TextItr &begin, const _TextItr &end, _TextItr &p)
{
    RuntimeStack    stk;
    return _TryMatch(begin, end, p, stk);
}

template <class _PtrnItr, class _TextItr, class _Traits>
bool RegExp<_PtrnItr, _TextItr, _Traits>::TryMatch (const _TextItr &begin, const _TextItr &end, _TextItr &p, RtStkSpec &spec)
{
    RuntimeStack    stk(spec);
    return _TryMatch(begin, end, p, stk);
}

template <class _PtrnItr, class _TextItr, class _Traits> template <class PtrnItr2>
bool RegExp<_PtrnItr, _TextItr, _Traits>::ParseShortcut (Shortcut &shortcut, PtrnItr2 &begin, const PtrnItr2 &end)
{
    return m_Automata.ParseShortcut<PtrnItr2>(shortcut, begin, end);
}

template <class _PtrnItr, class _TextItr, class _Traits>
bool RegExp<_PtrnItr, _TextItr, _Traits>::RunShortcut (Shortcut &shortcut, _TextItr &begin, _TextItr &end)
{
    return shortcut->GetTextItr(begin, end);
}

template <class _PtrnItr, class _TextItr, class _Traits> template <class PtrnItr2>
bool RegExp<_PtrnItr, _TextItr, _Traits>::GetShortcut(_TextItr &tbegin, _TextItr &tend, PtrnItr2 &pbegin, const PtrnItr2 &pend)
{
    Shortcut shortcut;
    if (!ParseShortcut(shortcut, pbegin, pend)) return false;
    return RunShortcut(shortcut, tbegin, tend);
}

template <class _PtrnItr, class _TextItr, class _Traits>
bool RegExp<_PtrnItr, _TextItr, _Traits>::Preparse (CoilVec &coil, _PtrnItr &begin, const _PtrnItr &end)
{
    _PtrnItr p = begin;
    try
    {
        bool inCharSet = false;
        int nParentheses = 0;

        for ( ; p != end; ++p)
        {
            if (*p == '\\')
            {
                ++p;
                if (p == end) { break; }
                continue;
            }

            if (*p == '[')
            {
                inCharSet = true;
                continue;
            }
            if (inCharSet)
            {
                if (*p == ']')
                {
                    inCharSet = false;
                }
                continue;
            }

            if (*p == '(')
            {
                coil.push_back(p);
                nParentheses++;
            }
            else if (*p == ')')
            {
                begin = p;
                if (nParentheses <= 0 || coil.empty())
                {   // ')' not expected
                    return false;
                }

                nParentheses--;

                ++p;
                if (p == end)
                {
                    coil.pop_back();
                    break;
                }
                if (!(*p == '{' || *p == '*' || *p == '+'))
                {
                    coil.pop_back();
                    p = begin;
                }
            }
        }
        begin = p;
        return (nParentheses == 0);
    }
    catch (...)
    {
        begin = p;
        return false;
    }
}

template <class _PtrnItr, class _TextItr, class _Traits>
bool RegExp<_PtrnItr, _TextItr, _Traits>::NodeBindText (RuntimeStack &stk, NodePtr pNode, TextItr &p)
{
    typedef Automata::SimpleNode    SimpleNode;
    typedef Automata::Node          Node;
    if (pNode->type != Node::SimpleLeft && pNode->type != Node::SimpleRight)
    {
        return false;
    }
    SimpleNode *pSimpleNode = static_cast<SimpleNode *>(pNode);
    if (!pSimpleNode->pTag)
    {
        return false;
    }

    if (stk.empty())
    {
        if (pSimpleNode->type == Node::SimpleLeft)
        {
            pSimpleNode->pTag->BindLeft(p);
        }
        else
        {
            pSimpleNode->pTag->BindRight(p);
        }
    }
    else
    {
        if (pSimpleNode->type == Node::SimpleLeft)
        {
            stk.top().tagState.BindLeft(pSimpleNode->pTag, p);
        }
        else
        {
            stk.top().tagState.BindRight(pSimpleNode->pTag, p);
        }
    }
    return true;
}

template <class _PtrnItr, class _TextItr, class _Traits>
bool RegExp<_PtrnItr, _TextItr, _Traits>::_TryMatch (const _TextItr &begin, const _TextItr &end, _TextItr &m, RuntimeStack &stk)
{
    try
    {
        m = begin;

        RuntimeState    cur;

        cur.pNode = m_Automata.Begin();
        cur.outletNextTry = m_Automata.OutletBegin(cur.pNode);

        while (1)
        {
            bool        passed = false;
            _TextItr    oldp = m;

            OutletItr i;
            OutletItr outletEnd = m_Automata.OutletEnd(cur.pNode);
            OutletItr outletLast = outletEnd; --outletLast;

            bool repeatable = m_Automata.NodeRepeatable(cur.pNode);
            if (repeatable)
            {
                OutletItr outlet2ndLast = outletLast; --outlet2ndLast;

                if (!m_Automata.NodeForceLeave(cur.pNode))
                {
                    for (i = cur.outletNextTry; i != outletLast; ++i)
                    {
                        if (m_Automata.TryEdge(cur.pNode, i, m, end))
                        {
                            if (i != outlet2ndLast || m_Automata.NodeAllowLeave(cur.pNode))
                            {   // push next try
                                cur.outletNextTry = i;
                                ++cur.outletNextTry;
                                cur.textItr = oldp;
                                cur.rep = m_Automata.GetRep(cur.pNode);
                                stk.push(cur);
                                /*
                                 * As to [:push:], the implementation can have the following
                                 * possible disposition:
                                 * 1. Throw exception on overflow, which straightly cease 
                                 *    the trying
                                 * 2. Do nothing on overflow, which reduces the attempt set
                                 * Each can give a reasonable response.
                                 */
                            }

                            m_Automata.PassEdgeOn(cur.pNode, i);
                            cur.outletNextTry = 0;
                            passed = true;
                            break;
                        }
                        m = oldp;
                    }
                }
                if (!passed && m_Automata.NodeAllowLeave(cur.pNode) 
                    && m_Automata.TryEdge(cur.pNode, outletLast, m, end))
                {
                    /* 
                     * Use PassEdgeOff instead to discriminate this case
                     * from PassEdge which makes a leave temporary from 
                     * the node into the circle 
                     */
                    m_Automata.PassEdgeOff(cur.pNode, i);   
                    cur.outletNextTry = m_Automata.OutletBegin(cur.pNode);
                    passed = true;
                }
            }
            else
            {
                // repeatable node is not allowed text binding for shortcut
                _TextItr bm = m;
                NodeBindText(stk, cur.pNode, bm);

                for (i = cur.outletNextTry; i != outletEnd; ++i)
                {
                    if (m_Automata.TryEdge(cur.pNode, i, m, end))
                    {
                        if (i != outletLast)
                        {
                            cur.outletNextTry = i;
                            ++cur.outletNextTry;
                            cur.textItr = oldp;
                            stk.push(cur);
                        }
                        m_Automata.PassEdge(cur.pNode, i);

                        cur.outletNextTry = 0;
                        passed = true;
                        break;
                    }
                    m = oldp;
                }
            }

            if (cur.pNode == m_Automata.End())
            {
                break;
            }
            if (m == end)
            {
                passed = false;
            }

            if (!passed)
            {
                if (stk.empty())
                {   /* all chances tried out */
                    return false;
                }
                cur = stk.top();
                m = cur.textItr;
                stk.pop();
                cur.tagState.Restore();
                if (m_Automata.NodeRepeatable(cur.pNode))
                {
                    m_Automata.SetRep(cur.pNode, cur.rep);
                }
            }
        }
        return true;
    }
    catch (...)
    {
        return false;
    }
}


#if defined(DEBUG_REGEXP) && DEBUG_REGEXP

template <typename Itr>
std::string DebugString (Itr begin, Itr end)
{
    std::string res = "";
    for (; begin!=end; ++begin)
    {
        res += *begin;
    }
    return res;
}

template <class PtrnItr, class TextItr, class Traits>
int RegExpAutomata<PtrnItr, TextItr, Traits>::GetNodeIndex (NodePtr pNode)
{
    // searching the node among all nodes
    for (int i = 0; i < (int)m_NodeVec.size(); ++i)
    {
        if (pNode == m_NodeVec[i])
        {
            return i;
        }
    }
    return -1;
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::ViewTree (TagPtr p, String name)
{
    for (int i = 0; i < (int)p->subs.size(); i++)
    {
        String lname = name;
        char chbuf[32];
        if (name != "")
        {
            lname += ".";
        }
        sprintf(chbuf, "%d", i+1);
        lname += chbuf;
        TagPtr np = p->subs[i];
        printf("Tag[%s] = 0x%08x\n", lname.c_str(), (Uint32)np);
        
        ViewTree(np, lname);
    }
}

template <class PtrnItr, class TextItr, class Traits>
void RegExpAutomata<PtrnItr, TextItr, Traits>::DebugView ()
{
    // view nodes
    printf("==  Nodes  ==\n");
    for (int i = 0; i < (int)m_NodeVec.size(); ++i)
    {
        if (m_NodeVec[i]->type == Node::Terminal)
        {
            printf("-- node[%03d] at 0x%08x --\n", i, reinterpret_cast<Uint32>(m_NodeVec[i]));
            printf(" END ()\n");
            continue;
        }

        PlainNode *pNode = static_cast<PlainNode *>(m_NodeVec[i]);

        printf("-- node[%03d] at 0x%08x --\n", i, reinterpret_cast<Uint32>(pNode));
        switch (pNode->type)
        {
        case Node::None:
            printf(" NONE ()\n");
            break;
        case Node::Plain:
            printf(" PLAIN ()\n");
            break;
        case Node::SimpleNone: case Node::SimpleLeft: case Node::SimpleRight:
            {
                SimpleNode  *pSimple = static_cast<SimpleNode *>(pNode);
                if (pSimple->type == Node::SimpleLeft) 
                    printf(" LEFT  (0x%08x)\n", reinterpret_cast<Uint32>(pSimple->pTag));
                else if (pSimple->type == Node::SimpleRight) 
                    printf(" RIGHT (0x%08x)\n", reinterpret_cast<Uint32>(pSimple->pTag));
                else
                    printf(" START (0x%08x)\n", reinterpret_cast<Uint32>(pSimple->pTag));
            }
            break;
        case Node::Repeatable:
            {
                RepeatableNode  *pRepNode = static_cast<RepeatableNode *>(pNode);
                const CountSet & countset = pRepNode->countset;
                printf(" REPEAT(");
                for (int j = 0; j < (int)countset.vec.size(); j++)
                {
                    const CountRange & cr = countset.vec[j];
                    if (cr.hi == cr.lo)
                    {
                        printf("%d", cr.lo);
                    }
                    else
                    {
                        if (cr.hi == CountSet::Infinity())
                            printf("%d-", cr.lo, cr.hi);
                        else
                            printf("%d-%d", cr.lo, cr.hi);
                    }
                    if (j != countset.vec.size() - 1) printf(", ");
                }
                printf(")\n");
            }
            break;
        }

        for (int j = 0; j < (int)pNode->outlets.size(); j++)
        {
            Edge &edge = pNode->outlets[j];
            NodePtr pDest = edge.dest;
            int dest = GetNodeIndex(pDest);

            if (dest >= 0)
                printf(" edge[%02d]->node[%03d] = ", j, dest);
            else
                printf(" edge[%02d]->node[0x%08x] = ", j, pDest);

            switch (edge.type)
            {
            case Edge::TypeNone:
                printf("NONE()\n");
                break;
            case Edge::TypeAnyChar:
                printf("ANYCHAR()\n");
                break;
            case Edge::TypeSingleChar:
                printf("CHAR(%c)\n", edge.GetSingleChar());
                break;
            case Edge::TypeCharSet:
                {
                    const CharSet &charset = edge.GetCharSet();
                    printf("CHARSET(");
                    for (int k = 0; k < 8; k++)
                    {
                        if (k > 0) printf("                               ");
                        printf("%08x", charset.bmp[k]);
                        if (k < 7) printf("\n");
                        else printf(")\n");
                    }
                }
                break;
            case Edge::TypeString:
                printf("STRING(%s)\n", edge.GetString().c_str());
                break;
            case Edge::TypeShortcut:
                {
                    TagPtr shortcut = edge.GetShortcut();
                    printf("SHORTCUT(0x%08x)\n", shortcut);
                }
                break;
            }
        }
    }
}

#endif  /* if defined(DEBUG_REGEXP) && DEBUG_REGEXP */

_QTL_END

#endif  /* ifndef _regexp_H */
