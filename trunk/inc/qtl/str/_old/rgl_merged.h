/**
 * === String Library ===
 *  Unit Name:  rgl_pars
 *  Author:     Lincoln Yu
 *  Purpose:	An enhanced regular expression parser, uses unit "rgl_auto".
 * == History ==        
 *  2005-11-13: Created.
 *	2005-12-04: Simply debugged.
 *	2005-12-08: Minor modification along with unit autoptr changed.
 *  2007-09-13: View svn logs.
 *  2007-12-09: The method of [:Deinit:] added in favor of strict Tag creation
 *              held in the unit of [:rgl_auto:]. The template system is still
 *              too hard to handle for some ineptitude compilers like g++.
 * == Comments ==
 *
 */

 #ifndef QTL_rgl_pars_H
 #define QTL_rgl_pars_H


/* ========== Beginning of "predef.h" included in "rgl_pars.h" ========== */

#if !defined(_QTL_predef_H)
#define _QTL_predef_H	"20051120"

#define _QTL_BEGIN	namespace qtl{
#define _QTL_END	}



#endif	// if !defined(_QTL_predef_H)

/* ========== End of "predef.h" included in "rgl_pars.h" ========== */

/* ========== Beginning of "rgl_auto.h" included in "rgl_pars.h" ========== */

/**
 * === String Library ===
 *  Unit Name:  rgl_auto
 *  Author:     Lincoln Yu
 *  Purpose:	An enhanced automata for regular expression.
 * == History ==        
 *  2005-11-26: Created.
 *	2005-11-30: Simply Debugged.
 *  2007-09-13: View svn logs.
 *  2007-12-09: Compliance with Visual Studio 2005. More cautious iterator moving 
 *              around the calling of [:MakeNode:].
 *              More formal and precise type specification on Tag creation.
 * == Comments ==
 *
 */

#ifndef _QTL_rgl_auto_H
#define _QTL_rgl_auto_H


/* ========== Including of "predef.h" in "rgl_auto.h" ignored  ========== */


/* ========== Beginning of "trie.h" included in "predef.h" ========== */

#ifndef _QTL_trie_H
#define _QTL_trie_H

/* ========== Including of "predef.h" in "trie.h" ignored  ========== */

#include <vector>


_QTL_BEGIN

using namespace std;

template <class _Tp> class trie
{
protected:
    struct node
    {
        typedef vector<node *> branches;

        _Tp		    _M_entry;
        branches	_M_branches;

        node() {}

        node(_Tp __data) : _M_entry(__data) {}

    };

    typedef typename node::branches						Branches;
    typedef typename Branches::iterator					BranchesItr;
    typedef typename Branches::reverse_iterator			BranchesRItr;
    typedef typename Branches::const_iterator			BranchesCItr;
    typedef typename Branches::const_reverse_iterator	BranchesCRItr;

    node	*_M_root;

    void clear(node *__p)
    {

        for(BranchesItr it = __p->_M_branches.begin(); it < __p->_M_branches.end(); ++it)
            if(*it)
                clear(*it);
        delete __p;
    }

public:
    trie(node *pNode = NULL) : _M_root(pNode) {}

    virtual ~trie() 
    {
        if(_M_root != NULL)
            clear(_M_root);
    }
};

_QTL_END

#endif	// ifndef _QTL_trie_H

/* ========== End of "trie.h" included in "predef.h" ========== */

/* ========== Beginning of "rgl_auto.h" included in "rgl_pars.h" ========== */

/* ========== Beginning of "autoptr.h" included in "rgl_auto.h" ========== */

#ifndef _QTL_autoptr_H
#define _QTL_autoptr_H

/* ========== Including of "predef.h" in "autoptr.h" ignored  ========== */


_QTL_BEGIN

// class autoptr

template <class _Target, class _Count = int>
class autoptr 
{
private:
    typedef autoptr<_Target>        _Myt;

#if defined(_MSC_VER)
    friend class autoptr;
#endif

protected:
    typedef _Target                 Target;
    typedef _Count                  Count;

    typedef Target *                TargetPtr;
    typedef Count *                 CountPtr;

    /**
    * Derived class should call these functions cautiously, esp. [:_target:] and [:_newly_attach_:].
    */

    // elementary functions
    bool _alone () const
    {
        return (_M_pCount != NULL && *_M_pCount == 1);
    }

    void _attach ()
    {
        if(_M_pCount != NULL)
            ++*_M_pCount;
    }

    void _detach ()
    {
        if (_M_pCount != NULL && --*_M_pCount <= 0)
        {
            delete _M_pTarget;
            delete _M_pCount;
        }
    }

    // compulsive functions, seldom used!
#if !defined(_MSC_VER)
public:
#endif

    TargetPtr _get_pTarget () const 
    { 
        return ( _M_pTarget ); 
    }

    void _set_pTarget (TargetPtr __pTarget)
    {
        _M_pTarget = __pTarget;
    }

    CountPtr _get_pCount () const
    {
        return ( _M_pCount );
    }

    void _set_pCount (CountPtr __pCount)
    {
        _M_pCount = __pCount;
    }

protected:

    // Toolbox
    void _release ()
    {
        _detach();
        _M_pTarget  = NULL;
        _M_pCount   = NULL;
    }

public:
    autoptr () : _M_pTarget(NULL), _M_pCount(NULL) {}

    autoptr ( const _Myt &__copy ) 
        : _M_pTarget(__copy._M_pTarget), _M_pCount(__copy._M_pCount)
    {
        _attach();
    }

    template <typename _OtherAutoPtr>
    autoptr ( const _OtherAutoPtr &__copy ) 
#if defined(_MSC_VER)
        : _M_pTarget(__copy._M_pTarget), _M_pCount(__copy._M_pCount)
#else
        : _M_pTarget(__copy._get_pTarget()), _M_pCount(__copy._get_pCount())
#endif
    {
        _attach();
    }

    autoptr (TargetPtr __pTarget) 
        : _M_pTarget(__pTarget), _M_pCount(new Count(1))
    {
    }

    autoptr (const Target &__target)
        : _M_pTarget(new Target(__target)), _M_pCount(new Count(1))
    {
    }

    virtual ~autoptr ()
    {
        _detach();
    }

    _Myt &operator= ( TargetPtr __pTarget )
    {
        _detach();
        _M_pTarget  = __pTarget;
        _M_pCount   = new Count(1);     
        return (*this);
    }

    _Myt &operator= ( const Target &__target )
    {
        _detach();
        _M_pTarget  = new Target(__target);
        _M_pCount   = new Count(1);     
        return (*this);
    }

    _Myt &operator= ( const _Myt &__right )
    {
        _detach();
        _M_pTarget  = __right._M_pTarget;
        _M_pCount   = __right._M_pCount;
        _attach();
        return (*this);
    }

    template < typename _T >
    _Myt &operator= ( const autoptr<_T> &__right )
        /*
        *  assign other autoptr type, which is almost always compatible to this one
        */
    {
        _detach();
        //    _M_pTarget  = __right._M_pTarget;
        //    _M_pCount   = __right._M_pCount;
        _M_pTarget = __right._get_pTarget();
        _M_pCount = __right._get_pCount();
        _attach();
        return (*this);
    }

    bool operator== ( const _Myt &__right ) const
    {
        return ( _M_pTarget == __right._M_pTarget );
    }

    // is this ever necessary?
    template < typename _OtherTgt >
    bool operator== ( const autoptr<_OtherTgt> &__right ) const
    {
#if defined(_MSC_VER)
        return ( _M_pTarget = __right._M_pTarget );
#else
        return ( _M_pTarget = __right._get_pTarget() );
#endif
    }

    template <typename _Other>
    bool operator== ( const _Other &__right ) const
    {
        return ( _M_pTarget == __right._get_pTarget() );
    }

    operator bool () const
    {
        return ( _M_pTarget != NULL );
    }

    TargetPtr operator-> () const
    {
        return ( _M_pTarget );
    }

    const Target & operator* () const
    {
        return ( *_M_pTarget );
    }

    Target & operator* ()
    {
        return ( *_M_pTarget );
    }

    template <class _PointerType>
    operator _PointerType ()
    {
        return ( reinterpret_cast<_PointerType>(_M_pTarget) );
    }

private:
    TargetPtr                       _M_pTarget;     // pointer to target
    CountPtr                        _M_pCount;      // pointer to reference counter

};  // class autoptr

_QTL_END

#endif  // ifndef _QTL_autoptr_H

/* ========== End of "autoptr.h" included in "rgl_auto.h" ========== */

#include <cassert>
#include <string>
#include <set>
#include <vector>

_QTL_BEGIN

class RegAutomataBase
    {
public:
    struct Tag
        {
        Tag ()
            {
            m_LeftRight = NULL;
            }

        ~Tag ()
            {
            assert(m_LeftRight == NULL);
            }

        template <class _CharItrType>
            void Create ()
            {
            if (m_LeftRight)
                {
                _CharItrType *pLR = reinterpret_cast<_CharItrType *>(m_LeftRight);
                delete[] pLR;
                }
            m_LeftRight = reinterpret_cast<void *>(new _CharItrType[2]);
            }

        template <class _CharItrType>
            void Destroy ()
            {
            _CharItrType *pLR = reinterpret_cast<_CharItrType *>(m_LeftRight);
            if (pLR)
                delete[] pLR;
            m_LeftRight = NULL;
            }

        template <class _CharItrType>
            void Set (const _CharItrType &value)
            {
            GetLeft<_CharItrType>() = GetRight<_CharItrType>() = value;
            }

        template <class _CharItrType>
            void Set (const _CharItrType &left, const _CharItrType &right)
            {
            GetLeft<_CharItrType>() = left;
            GetRight<_CharItrType>() = right;
            }

        template <class _CharItrType>
            _CharItrType & GetLeft()
            {
            return reinterpret_cast<_CharItrType *>(m_LeftRight)[0];
            }

        template <class _CharItrType>
            const _CharItrType & GetLeft() const
            {
            return reinterpret_cast<_CharItrType *>(m_LeftRight)[0];
            }

        template <class _CharItrType>
            _CharItrType & GetRight()
            {
            return reinterpret_cast<_CharItrType *>(m_LeftRight)[1];
            }

        template <class _CharItrType>
            const _CharItrType & GetRight() const
            {
            return reinterpret_cast<_CharItrType *>(m_LeftRight)[1];
            }

        private:
            void    *m_LeftRight;

        };	// struct Tag

    class TagTrie : public trie<Tag>
        { 
        typedef trie<Tag>						_Mybase;

        public:
            typedef _Mybase::node				Node;
            typedef _Mybase::BranchesCItr		BranchesCItr;

            TagTrie () : _Mybase(new Node) {}
        ~TagTrie () 
            {	// do nothing, let base class do destruction
            }

        Node *root () const { return _M_root; }

        void Destroy ()
            {
            for (BranchesCItr it = _M_root->_M_branches.begin(); it < _M_root->_M_branches.end(); ++it)
                if (*it != NULL)
                    clear(*it);
            _M_root->_M_branches.clear();
            }

        };	// struct TagTrie

    typedef TagTrie::BranchesCItr	BranchesCItr;
    typedef TagTrie::Node           TagNode;

    typedef std::set<char>          CharSet;
    typedef std::string				String;

    struct State
        {
        struct CountRange
            {
            int m_Low, m_High;

            CountRange (int low = 0, int high = 0) : m_Low(low), m_High(high) {}

            };	// struct CountRange

        typedef std::set<CountRange>            CountSet;

        struct CounterData
            {
            CountSet	m_CountSet;
            int			m_UsrCount;

            CounterData () : m_UsrCount(0) {}

        CounterData (const CountSet &val) : m_UsrCount(0), m_CountSet(val) 
            {
            }			
            };	// struct CounterData

        class Counter : public autoptr<CounterData>
            {
            typedef CounterData				_MyTarget;
            typedef autoptr<_MyTarget>		_Mybase;
            public:

            Counter () : _Mybase() {}
            Counter (const CountSet &val) : _Mybase(new _MyTarget(val))
                {
                }

            void Release ()
                {
                _release();
                }

            };	// class Counter

        struct Transfer
            {
            enum Style { k_Serial, k_Inclusive, k_Exclusive, k_Shortcut };

            Tag			*m_pTag;
            Style		m_Style;
            Counter		m_Counter;
            State		*m_pOutlet;

            CharSet		m_CharSet;
            String		m_String;

            Transfer (Style style = k_Serial) : m_Style(style), m_pOutlet(NULL)
                {
                }
            Transfer (Tag *tag, State *pOutlet)
                : m_Style(k_Shortcut), m_pTag(tag), m_pOutlet(pOutlet)
                {
                }
            Transfer (Style style, const CharSet &charSet, State *pOutlet = NULL)
                : m_Style(style), m_CharSet(charSet), m_pOutlet(pOutlet)
                {
                }

            Transfer (const String &str, State *pOutlet = NULL)
                : m_Style(k_Serial), m_String(str), m_pOutlet(pOutlet)
                {
                }

            };	// struct Transfer

        typedef std::vector<Transfer>		Transfers;
        typedef Transfers::iterator			TransfersItr;
        typedef Transfers::reverse_iterator TransfersRItr;

        struct Mark
            {
            typedef unsigned char Flag;
            Flag m_Flag;
            Tag *m_pTag;

            Mark () : m_pTag(NULL), m_Flag(0)
                {
                }
            };

        State ()
            {
            }

        ~State ()
            {
            }

        // members of struct State

        Transfers	m_Transfers;
        Mark		m_Mark;

        };	// struct State

    typedef std::vector<State *>	ClearList;

    };

template <class _CharItr> class RegAutomata : protected RegAutomataBase
	{
protected:
	struct Bridge
		{
		struct Begin
			{
			State *m_pState;
			bool m_IsMulti;		// multioutlet flag, set by specific programs
			State::Counter m_Precounter;

			Begin (State *pState = NULL) : m_pState(pState), m_IsMulti(false)
				{
				}

			void Rebuild (State *pState, bool isMultioutlet = false)
				{
				m_pState = pState;
				m_IsMulti = isMultioutlet;
				m_Precounter.Release();
				}

			bool IsMarked () const
				{
				return (m_pState->m_Mark.m_pTag != NULL);
				}
			};	// struct Begin

		struct End
			{
			State *m_pState;
			bool m_IsFree;

			End (State *pState = NULL, bool isFree = true) : m_pState(pState), m_IsFree(isFree)
				{
				}
	        
			};	// struct End


		Bridge () : m_Begin(), m_End()
			{
			}
		Bridge (const Begin &begin, const End &end) : m_Begin(begin), m_End(end)
			{
			}
		~Bridge ()
			{
			}

		void Rebuild (const Begin &begin, const End &end)
			{
			m_Begin = begin, m_End = end;
			}

		void Link (State::Transfer::Style style, const CharSet &charSet = CharSet())
			{
			State::Transfer transfer(style, charSet, m_End.m_pState);
			transfer.m_Counter = m_Begin.m_Precounter;
			m_Begin.m_pState->m_Transfers.push_back(transfer);
			}

		void Link (const String &str = "")
			{
			State::Transfer transfer(str, m_End.m_pState);
			transfer.m_Counter = m_Begin.m_Precounter;
			m_Begin.m_pState->m_Transfers.push_back(transfer);
			}

		void Shortcut (Tag *pTag)
			{
			State::Transfer transfer(pTag, m_End.m_pState);
			transfer.m_Counter = m_Begin.m_Precounter;
			m_Begin.m_pState->m_Transfers.push_back(transfer);
			}

		Begin &begin () { return m_Begin; }
		End &end () { return m_End; }
		State *beginState () { return m_Begin.m_pState; }
		State *endState () { return m_End.m_pState; }

	protected:
		Begin	m_Begin;
		End		m_End;

		};	// struct Bridge


	struct Element
		{
		enum Type {k_Nil, k_Character, k_Exclusive, k_Inclusive, k_AnyChar, k_Shortcut,
				k_Count, k_Plus, k_AnyTimes} m_Type;

		union
		{
			char m_Ch;
			State::CountSet *m_pCountSet;
			TagNode *m_pNode;
			CharSet *m_CharSet;
		};

		Element () : m_Type(k_Nil) {}
		Element (const Element &copy)
			{
			CopyCreate(copy);
			}
		~Element ()
			{
			Reset();
			}

#if 0
		Element &operator= (const Element &copy)
			{
			if (m_Type == copy.m_Type)
				{
				switch(m_Type)
					{
					case k_Character:
						m_Ch = copy.m_Ch;
						break;
					case k_Exclusive: case k_Inclusive:
						*m_CharSet = *copy.m_CharSet;
						break;
					case k_Shortcut:
						m_Node = copy.m_Node;
						break;
					case k_Count:
						*m_pCountSet = *copy.m_pCountSet;
						break;
					}
				}
			else
				{
				Reset();
				CopyCreate(copy);
				}
			}
#endif

		void CopyCreate (const Element &copy)
			{
			switch(m_Type = copy.m_Type)
				{
				case k_Character:
					m_Ch = copy.m_Ch;
					break;
				case k_Exclusive: case k_Inclusive:
					m_CharSet = new CharSet;
					*m_CharSet = *copy.m_CharSet;
					break;
				case k_Shortcut:
					m_pNode = copy.m_pNode;
					break;
				case k_Count:
					m_pCountSet = new State::CountSet;
					*m_pCountSet = *copy.m_pCountSet;
					break;
				}			
			}

		void Reset ()
			{
			switch(m_Type)
				{
				case k_Count:
					delete m_pCountSet;
					break;
				case k_Exclusive: case k_Inclusive:
					delete m_CharSet;
					break;
				}
			}
		};	// struct Element


	struct Topo
		{
		enum Style {NORMAL, k_Count, ANY, k_Plus};

		Style m_Style;

		State::CountSet	m_CountSet;

		Topo (Style style = k_Count) : m_Style(style) {}
		};	// struct Topo
	
	struct EndInfo
		{
		int		m_Index;
		bool	m_IsPure;
		};	// struct EndInfo

	enum MarkFlag 
		{
		MF_NULL			= 0,
		MF_LEFT         = 1,
		MF_RIGHT        = 2,
		MF_BOTH         = 3,
		MF_BOTHVISITED  = 4,
		MF_BASICMASK    = 0x07,
		MF_EVENTMASK	= 0x10
		};
	
protected:
    enum
    {
        k_LogicalInfinite = -1
    };

	State		*m_pStart;
	TagTrie		m_TagTrie;

	static bool IsDigit (char c)
		{
		return (c >= '0' && c <= '9');
		}

    static int GetNumber (_CharItr &ciPos, const _CharItr &ciEnd)
		{
		int result = 0;
		char ch;
		for ( ; ciPos < ciEnd && (ch = *ciPos) >= '0' && ch <= '9'; ++ciPos)
			result *= 10, result += ch - '0';
		return result;
		}

    static int GetHex (_CharItr &ciPos, const _CharItr &ciEnd)
		{
		int result = 0;
		char ch;
        Element e1,e2;
        e2 = e1;
		for ( ; ciPos < ciEnd; ++ciPos)
			{
			result *= 16;
			if ( (ch = *ciPos) >= 'A' && ch <= 'F' )
				result += ch - 'A' + 10;
			else if (ch >= 'a' && ch <= 'f')
				result += ch - 'a' + 10;
			else if (ch >= '0' && ch <= '9')
				result += ch - '0';
			else break;
			}
		return result;
		}

    static char GetChar (_CharItr &ciPos, const _CharItr &ciEnd)
		{
		if (ciPos >= ciEnd)
			return 0;
		switch(char ch = *ciPos++)
			{
			case '\\':
				if(ciPos >= ciEnd)
					return 0;
				switch(ch = *ciPos++)
					{
					case 'x': case 'X':
						if(ciPos + 2 > ciEnd)
							return 0;
						return (char)GetHex(ciPos, ciPos + 2);
					case 'n':
						return '\n';
					case 't':
						return '\t';
					default:
						return ch;
					}
				// already returned
			default:
				return ch;
			}
		}

    static typename Element::Type GetCharSet (_CharItr &ciPos, const _CharItr &ciEnd, CharSet *pCharSet)
		{
		char ch1 = 0, ch2;
		typename Element::Type result;
		if (*ciPos == '^')
			{
			++ciPos;
			result = Element::k_Exclusive;
			}
		else
			result = Element::k_Inclusive;
		while (true)
			{
			switch(*ciPos)
				{
				case ']':
					++ciPos;
					return result;	// get out here
				case '-':
					ch2 = GetChar(++ciPos, ciEnd);
					while(ch1 < ch2)
						pCharSet->insert(++ch1);
					break;
				default:
					ch1 = GetChar(ciPos, ciEnd);
					pCharSet->insert(ch1);
				}
			}
		}

    static void SkipSpace (_CharItr &ciPos, const _CharItr &ciEnd)
		{
		for( ; ciPos < ciEnd && *ciPos == ' '; ++ciPos);
		}

    static void GoCountRangeEnd (_CharItr &ciPos, const _CharItr &ciEnd)
		{
		for( ; ciPos < ciEnd && *ciPos != ',' && *ciPos != '}'; ++ciPos);
		}

    static void GetCountRange (_CharItr &ciPos, const _CharItr &ciEnd, State::CountRange &countRange)
		{
		SkipSpace(ciPos, ciEnd);
		countRange.m_High = countRange.m_Low = GetNumber(ciPos, ciEnd);
		SkipSpace(ciPos, ciEnd);
		switch(*ciPos++)
			{
			case ',': case '}':
				break;
			case '-':
				SkipSpace(ciPos, ciEnd);
				if(!IsDigit(*ciPos))
					countRange.m_High = k_LogicalInfinite;
				else
					countRange.m_High = GetNumber(ciPos, ciEnd);
				GoCountRangeEnd(ciPos, ciEnd);
				++ciPos;
			}
		}

    static void GetCountSet (_CharItr &ciPos, const _CharItr &ciEnd, State::CountSet *pCountSet)
		{
		for( ; ciPos < ciEnd && *(ciPos - 1) != '}'; )
			{
			State::CountRange countRange;
			GetCountRange(ciPos, ciEnd, countRange);
			pCountSet->insert(countRange);
			}
		}

    TagNode *GetShortcut (_CharItr &ciPos, const _CharItr &ciEnd, TagNode *pNode)
		{
		TagNode *result;
		if(*ciPos == '.')
			{
			result = m_TagTrie.root();
			++ciPos;
			}
		else
			result = pNode;
		int index;
		while(true)
			{
			index = GetNumber(ciPos, ciEnd) - 1;
			if(index >= 0)
				{
				BranchesCItr it = result->_M_branches.begin();
				it += index;
				if(it < result->_M_branches.end())
					result = *it;
				else
					return NULL;
				}
			if(ciPos >= ciEnd)
				return result;

			switch(*ciPos++)
				{
				case '>':
					return result;
				case '.':
					break;
				default:
					--ciPos;
					return NULL;
				}
			}
		}

    void getElement (_CharItr &ciPos, const _CharItr &ciEnd, Element &elem, TagNode *pNode)
		{
		elem.Reset();
		switch(char ch = *ciPos++)
			{
			case '<':
				ch = *ciPos;
				if(ch >= '0' && ch <= '9' || ch == '.')
					{
					elem.m_Type = Element::k_Shortcut;
					elem.m_pNode = GetShortcut(ciPos, ciEnd, pNode);
					}
				break;
			case '\\':
				elem.m_Type = Element::k_Character;
				elem.m_Ch = GetChar(--ciPos, ciEnd);
				break;
			case '.':
				elem.m_Type = Element::k_AnyChar;
				break;
			case '[':
				elem.m_CharSet = new CharSet;
				elem.m_Type = GetCharSet(ciPos, ciEnd, elem.m_CharSet);
				break;
			case '{':
				elem.m_Type = Element::k_Count;
				elem.m_pCountSet = new State::CountSet;
				GetCountSet(ciPos, ciEnd, elem.m_pCountSet);
				break;
			case '+':
				elem.m_Type = Element::k_Plus;
				break;
			case '*':
				elem.m_Type = Element::k_AnyTimes;
				break;
			default:
				elem.m_Type = Element::k_Character;
				elem.m_Ch = ch;
				break;
			}
		}


//  creating pure structure

    static void CreatePureSerial (const String &str, typename Bridge::Begin &begin, typename Bridge::End &end, bool finished)
		{
		if(finished && !end.m_IsFree)
			Bridge(begin, end.m_pState).Link(str);
		else
			{
			Bridge bridge(begin, typename Bridge::End(new State));
			bridge.Link(str);
			begin.Rebuild(bridge.end().m_pState);
			}
		}

    static void CreatePureCharSetOnly (const CharSet &charSet, State::Transfer::Style style, 
		typename Bridge::Begin &begin, typename Bridge::End &end, bool finished)
		{
		if(finished && !end.m_IsFree)
			{
			Bridge(begin, end.m_pState).Link(style, charSet);
			}
		else
			{
			Bridge bridge(begin, typename Bridge::End(new State));
			bridge.Link(style, charSet);
			begin.Rebuild(bridge.end().m_pState);
			}
		}

    static void CreatePureCharSetCount (const CharSet &charSet, State::Transfer::Style style, 
		const State::CountSet &countSet, typename Bridge::Begin &begin, typename Bridge::End &end, bool finished)
		{
		Bridge bridge;
		if(begin.m_IsMulti || begin.IsMarked())
			{
			bridge.Rebuild(begin, typename Bridge::End(new State));
			bridge.Link();
			begin.m_pState = bridge.endState();
			begin.m_IsMulti = false;
			}
		begin.m_Precounter = countSet;
		bridge.Rebuild(begin, typename Bridge::End(begin.m_pState));
		bridge.Link(style, charSet);
		begin.m_IsMulti = true;
		begin.m_Precounter.Release();
		if(finished && !end.m_IsFree)
			{
			bridge.Rebuild(begin, typename Bridge::End(end.m_pState));
			bridge.Link();
			}
		}

    static void CreatePureCharSetAnyTimes (const CharSet &charSet, State::Transfer::Style style, 
		typename Bridge::Begin &begin, typename Bridge::End &end, bool finished)
		{
		Bridge bridge;
		if(begin.m_IsMulti || begin.IsMarked())
			{
			bridge.Rebuild(begin, typename Bridge::End(new State));
			bridge.Link();
			begin.Rebuild(bridge.end().m_pState);
			}
		bridge.Rebuild(begin, typename Bridge::End(begin.m_pState));
		bridge.Link(style, charSet);
		begin.m_IsMulti = true;
		begin.m_Precounter.Release();
		if(finished && !end.m_IsFree)
			{
			bridge.Rebuild(begin, typename Bridge::End(end.m_pState));
			bridge.Link();
			}			
		}

    static void CreatePureCharSetPlus (const CharSet &charSet, State::Transfer::Style style, 
		typename Bridge::Begin &begin, typename Bridge::End &end, bool finished)
		{
		Bridge bridge;
		if(begin.m_IsMulti || begin.IsMarked())
			{
			bridge.Rebuild(begin, typename Bridge::End(new State));
			bridge.Link();
			begin.Rebuild(bridge.endState());
			}
		bridge.Rebuild(begin, typename Bridge::End(new State));
		bridge.Link(style, charSet);
		begin.Rebuild(bridge.endState(), true);
		bridge.Rebuild(begin, typename Bridge::End(bridge.beginState()));
		bridge.Link();
		if(finished && !end.m_IsFree)
			{
			bridge.Rebuild(begin, typename Bridge::End(end.m_pState));
			bridge.Link();
			}
		}

    static void CreatePureShortcutOnly (TagNode *pTagNode, typename Bridge::Begin &begin, 
		typename Bridge::End &end, bool finished)
		{
		if(finished && !end.m_IsFree)
			Bridge(begin, end.m_pState).Shortcut(&pTagNode->_M_entry);
		else
			{
			Bridge bridge(begin, typename Bridge::End(new State));
			bridge.Shortcut(&pTagNode->_M_entry);
			begin.Rebuild(bridge.endState());
			}
		}

    static void CreatePureShortcutCount (TagNode *pTagNode, const State::CountSet &countSet, 
		typename Bridge::Begin &begin, typename Bridge::End &end, bool finished)
		{
		Bridge bridge;
		if(begin.m_IsMulti || begin.IsMarked())
			{
			bridge.Rebuild(begin, typename Bridge::End(new State));
			bridge.Link();
			begin.m_pState = bridge.endState();
			begin.m_IsMulti = false;
			}
		begin.m_Precounter = countSet;
		bridge.Rebuild(begin, typename Bridge::End(begin.m_pState));
		bridge.Shortcut(&pTagNode->_M_entry);
		begin.m_IsMulti = true;
		begin.m_Precounter.Release();
		if(finished && !end.m_IsFree)
			{
			bridge.Rebuild(begin, typename Bridge::End(end.m_pState));
			bridge.Link();
			}
		}

    static void CreatePureShortcutAnyTimes (TagNode *pTagNode, typename Bridge::Begin &begin, typename Bridge::End &end, bool finished)
		{
		Bridge bridge;
		if(begin.m_IsMulti || begin.IsMarked())
			{
			bridge.Rebuild(begin, typename Bridge::End(new State));
			bridge.Link();
			begin.Rebuild(bridge.endState());
			}
		bridge.Rebuild(begin, typename Bridge::End(begin.m_pState));
		bridge.Shortcut(&pTagNode->_M_entry);
		begin.m_IsMulti = true;
		begin.m_Precounter.Release();
		if(finished && !end.m_IsFree)
			{
			bridge.Rebuild(begin, typename Bridge::End(end.m_pState));
			bridge.Link();
			}
		}

    static void CreatePureShortcutPlus(TagNode *pTagNode, typename Bridge::Begin &begin, typename Bridge::End &end, bool finished)
		{
		Bridge bridge;
		if(begin.m_IsMulti || begin.IsMarked())
			{
			bridge.Rebuild(begin, typename Bridge::End(new State));
			bridge.Link();
			begin.Rebuild(bridge.endState());
			}
		bridge.Rebuild(begin, typename Bridge::End(new State));
		bridge.Shortcut(&pTagNode->_M_entry);
		begin.Rebuild(bridge.endState(), true);
		bridge.Rebuild(begin, typename Bridge::End(bridge.beginState()));
		bridge.Link();
		if(finished && !end.m_IsFree)
			{
			bridge.Rebuild(begin, typename Bridge::End(end.m_pState));
			bridge.Link();
			}
		}

    bool CreatePureChar(Element &elem, _CharItr &ciPos, const _CharItr &ciEnd, 
		typename Bridge::Begin &begin, typename Bridge::End &end, TagNode *pNode)
		{
		CharSet charSet;
		charSet.insert(elem.m_Ch);
		String str = "";
		str += elem.m_Ch;
		if(ciPos >= ciEnd)
			{
			CreatePureSerial(str, begin, end, true);
			return false;
			}
		Element lastElem = elem;
		getElement(ciPos, ciEnd, elem, pNode);
		switch(elem.m_Type)
			{
			case Element::k_Character:
				return CreatePureCharChar(elem, str, ciPos, ciEnd, begin, end, pNode);
			case Element::k_Count:
				CreatePureCharSetCount(charSet, State::Transfer::k_Inclusive, *elem.m_pCountSet, 
					begin, end, ciPos >= ciEnd);
				return false;
			case Element::k_AnyTimes:
				CreatePureCharSetAnyTimes(charSet, State::Transfer::k_Inclusive, begin, end, ciPos >= ciEnd);
				return false;
			case Element::k_Plus:
				CreatePureCharSetPlus(charSet, State::Transfer::k_Inclusive, begin, end, ciPos >= ciEnd);
				return false;
			default:
				CreatePureSerial(str, begin, end, false);
				return true;
			}
		}

    bool CreatePureCharChar(Element &elem, String &str, _CharItr &ciPos, const _CharItr &ciEnd, typename Bridge::Begin &begin, typename Bridge::End &end, TagNode *pNode)
		{
		if(ciPos >= ciEnd)
			{
			str += elem.m_Ch;
			CreatePureSerial(str, begin, end, true);
			return false;
			}
		Element lastElem = elem;
		getElement(ciPos, ciEnd, elem, pNode);
		
		CharSet charSet;
		switch(elem.m_Type)
			{
			case Element::k_Character:
				str += lastElem.m_Ch;
				return CreatePureCharChar(elem, str, ciPos, ciEnd, begin, end, pNode);
			case Element::k_Count:
				CreatePureSerial(str, begin, end, false);
				charSet.insert(lastElem.m_Ch);
				CreatePureCharSetCount(charSet, State::Transfer::k_Inclusive,
					*elem.m_pCountSet, begin, end, ciPos >= ciEnd);
				return false;
			case Element::k_AnyTimes:
				CreatePureSerial(str, begin, end, false);
				charSet.insert(lastElem.m_Ch);
				CreatePureCharSetAnyTimes(charSet, State::Transfer::k_Inclusive, begin,
					end, ciPos >= ciEnd);
				return false;
			case Element::k_Plus:
				CreatePureSerial(str, begin, end, false);
				charSet.insert(lastElem.m_Ch);
				CreatePureCharSetPlus(charSet, State::Transfer::k_Inclusive, begin, end,
						ciPos >= ciEnd);
				return false;
			default:
				str += lastElem.m_Ch;
				CreatePureSerial(str, begin, end, false);
				return true;
			}
		}

    bool CreatePureAnyChar(Element &elem, _CharItr &ciPos, const _CharItr &ciEnd, typename Bridge::Begin &begin, 
		typename Bridge::End &end, TagNode *pNode)
		{
		if(ciPos >= ciEnd)
			{
			CreatePureCharSetOnly(CharSet(), State::Transfer::k_Exclusive, begin, end, true);
			return false;
			}
		Element lastElem = elem;
		getElement(ciPos, ciEnd, elem, pNode);
		switch(elem.m_Type)
			{
			case Element::k_Count:
				CreatePureCharSetCount(CharSet(), State::Transfer::k_Exclusive, *elem.m_pCountSet, begin, end, 
					ciPos >= ciEnd);
				return false;
			case Element::k_AnyTimes:
				CreatePureCharSetAnyTimes(CharSet(), State::Transfer::k_Exclusive, begin, end, ciPos >= ciEnd);
				return false;
			case Element::k_Plus:
				CreatePureCharSetPlus(CharSet(), State::Transfer::k_Exclusive, begin, end, ciPos >= ciEnd);
				return false;
			default:
				CreatePureCharSetOnly(CharSet(), State::Transfer::k_Exclusive, begin, end, false);
				return true;
			}
		}

    bool CreatePureCharSet(Element &elem, _CharItr &ciPos, const _CharItr &ciEnd, typename Bridge::Begin &begin, 
		typename Bridge::End &end, TagNode *pNode)
		{
		State::Transfer::Style style = (elem.m_Type == Element::k_Inclusive)?
				State::Transfer::k_Inclusive : State::Transfer::k_Exclusive;
		if(ciPos >= ciEnd)
			{
				CreatePureCharSetOnly(*elem.m_CharSet, style, begin, end, true);
				return false;
			}
		Element lastElem = elem;
		getElement(ciPos, ciEnd, elem, pNode);
		switch(elem.m_Type)
			{
			case Element::k_Count:
				CreatePureCharSetCount(*lastElem.m_CharSet, style, *elem.m_pCountSet, begin, end, 
					ciPos >= ciEnd);
				return false;
			case Element::k_AnyTimes:
				CreatePureCharSetAnyTimes(*lastElem.m_CharSet, style, begin, end, ciPos >= ciEnd);
				return false;
			case Element::k_Plus:
				CreatePureCharSetPlus(*lastElem.m_CharSet, style, begin, end, ciPos >= ciEnd);
				return false;
			default:
				CreatePureCharSetOnly(*lastElem.m_CharSet, style, begin, end, false);
				return true;
			}
		}

    bool CreatePureShortcut(Element &elem, _CharItr &ciPos, const _CharItr &ciEnd, typename Bridge::Begin &begin, 
		typename Bridge::End &end, TagNode *pNode)
		{
		if(ciPos >= ciEnd)
			{
			CreatePureShortcutOnly(elem.m_pNode, begin, end, true);
			return false;
			}
		Element lastElem = elem;
		getElement(ciPos, ciEnd, elem, pNode);
		switch(elem.m_Type)
			{
			case Element::k_Count:
				CreatePureShortcutCount(lastElem.m_pNode, *elem.m_pCountSet, begin, end, ciPos >= ciEnd);
				return false;
			case Element::k_AnyTimes:
				CreatePureShortcutAnyTimes(lastElem.m_pNode, begin, end, ciPos >= ciEnd);
				return false;
			case Element::k_Plus:
				CreatePureShortcutPlus(lastElem.m_pNode, begin, end, ciPos >= ciEnd);
				return false;
			default:
				CreatePureShortcutOnly(lastElem.m_pNode, begin, end, false);
				return true;
			}
		}

    bool CreatePureTrigger(Element &elem, _CharItr &ciPos, const _CharItr &ciEnd, typename Bridge::Begin &begin, 
		typename Bridge::End &end, TagNode *pNode)
		{
		switch(elem.m_Type)
			{
			case Element::k_Character:
				return CreatePureChar(elem, ciPos, ciEnd, begin, end, pNode);
			case Element::k_AnyChar:
				return CreatePureAnyChar(elem, ciPos, ciEnd, begin, end, pNode);
			case Element::k_Inclusive: case Element::k_Exclusive:
				return CreatePureCharSet(elem, ciPos, ciEnd, begin, end, pNode);
			case Element::k_Shortcut:
				return CreatePureShortcut(elem, ciPos, ciEnd, begin, end, pNode);
			default:
				if(end.m_IsFree)
					Bridge(begin, typename Bridge::End(new State)).Link();
				else
					{
					Bridge bridge(begin, end.m_pState);
					bridge.Link();
					begin.Rebuild(bridge.endState());
					}
			}
		ciPos = ciEnd;
		return false;		
		}

    void CreatePure(const _CharItr &ciBegin, const _CharItr &ciEnd, typename Bridge::Begin &begin, typename Bridge::End &end, 
		TagNode *pNode)
		{
		_CharItr ciPos = ciBegin;
		Element elem;
		bool res = false;
		while(res || ciPos < ciEnd)
			{
			if(!res)
				getElement(ciPos, ciEnd, elem, pNode);
			res = CreatePureTrigger(elem, ciPos, ciEnd, begin, end, pNode);
			}
		}

    void CreateTopoNormal (const _CharItr &ciBegin, const _CharItr &ciEnd, typename Bridge::Begin &begin, TagNode *pNode)
		/*  Pre:
		 *	Post:   begin = (bridge.m_End.m_pState, false, (NULL)) 
		 *	Hint:	Creating topo structure
		 */
		{
		Bridge bridge(begin, typename Bridge::End(new State));
		CreateParallel(ciBegin, ciEnd, bridge, pNode);
		begin.m_pState = bridge.endState();
		}

    void CreateTopoCount (const State::CountSet &topoCountSet, const _CharItr &ciBegin, 
		const _CharItr &ciEnd, typename Bridge::Begin &begin, TagNode *pNode)
		/*  Pre:
		 *	Post:   begin = (begin.m_pState, true, (NULL))   
		 */
		{
		begin.m_Precounter = topoCountSet;
		Bridge bridge(begin, begin.m_pState);
		CreateParallel(ciBegin, ciEnd, bridge, pNode);
		begin.m_IsMulti = true;
		begin.m_Precounter.Release();		
		}

    void CreateTopoAny (const _CharItr &ciBegin, const _CharItr &ciEnd, typename Bridge::Begin &begin, TagNode *pNode)
		/*  Pre:
		 *	Post:	begin = (begin.m_pState, true, (NULL))   
		 */
		{
		Bridge bridge(begin, begin.m_pState);
		CreateParallel(ciBegin, ciEnd, bridge, pNode);
		begin.m_IsMulti = true;
		}

    void CreateTopoPlus(const _CharItr &ciBegin, const _CharItr &ciEnd, typename Bridge::Begin &begin, TagNode *pNode)
		/*  Pre:
		 *	Post:   begin = (bridge.m_End.m_pState, true, (NULL))   
		 */
		{
		Bridge bridge(begin, typename Bridge::End(new State));
		CreateParallel(ciBegin, ciEnd, bridge, pNode);
		begin.m_pState = bridge.endState();
		begin.m_IsMulti = true;
		bridge.Rebuild(begin, typename Bridge::End(bridge.beginState()));
		bridge.Link();
		}

    void CreateTopo (const Topo &topo, const _CharItr &ciBegin, const _CharItr &ciEnd, typename Bridge::Begin &begin, TagNode *pNode)
		/*  Pre:    [:len:] is the length of the expression including the terminator
		 *			after the last valid character.
		 *	Post:   
		 */
		{
		switch(topo.m_Style)
			{
			case Topo::k_Count:
				CreateTopoCount(topo.m_CountSet, ciBegin, ciEnd, begin, pNode);
				break;
			case Topo::NORMAL:
				CreateTopoNormal(ciBegin, ciEnd, begin, pNode);
				break;
			case Topo::ANY:
				CreateTopoAny(ciBegin, ciEnd, begin, pNode);
				break;
			case Topo::k_Plus:
				CreateTopoPlus(ciBegin, ciEnd, begin, pNode);
				break;
			}
		}

    static Topo ParseTopo (_CharItr &ciPos, const _CharItr &ciEnd)
		{
		Topo result;
		switch(*ciPos++)
			{
			case '*':
				result.m_Style = Topo::ANY;
				return result;
			case '+':
				result.m_Style = Topo::k_Plus;
				return result;
			case '{':
				result.m_Style = Topo::k_Count;
				GetCountSet(ciPos, ciEnd, &result.m_CountSet);
				return result;
			default:
				ciPos--;
				result.m_Style = Topo::NORMAL;
				return result;
			}
		}

    //  macro creating
    static void ClearTags (TagNode *pNode, const _CharItr ciBegin, const BranchesCItr &endBranch)
		{
		for(BranchesCItr it = pNode->_M_branches.begin(); it < endBranch; ++it)
			(*it)->_M_entry.Set(ciBegin);
		}

    void CreateSerial (const _CharItr &ciBegin, const _CharItr &ciEnd, Bridge &bridge, TagNode *pNode, const BranchesCItr &endBranches)
		{
		_CharItr ciPos = ciBegin;
		Topo topo;
		typename Bridge::Begin begin = bridge.begin();

		for(BranchesCItr it = pNode->_M_branches.begin(); it < pNode->_M_branches.end(); ++it)
			{
			_CharItr ciTemp = (*it)->_M_entry.GetLeft<_CharItr>();
			if ( ciTemp > ciPos )
				{
				CreatePure(ciPos, ciTemp, begin, typename Bridge::End(), pNode);
				}
				
			if(begin.IsMarked() || begin.m_IsMulti)
			//  any precounter will be released here
				{
				Bridge temp(begin, typename Bridge::End(new State));
				temp.Link();
				begin.Rebuild(temp.endState());
				}
			topo = ParseTopo((ciPos = (*it)->_M_entry.GetRight<_CharItr>()), ciEnd);
			CreateTopo(topo, (*it)->_M_entry.GetLeft<_CharItr>() + 1, (*it)->_M_entry.GetRight<_CharItr>(), begin, *it);
			}

		if(ciEnd > ciPos)
			{
			CreatePure(ciPos, ciEnd, begin, typename Bridge::End(bridge.endState(), false), pNode);
			
			}
		else
			{
			Bridge temp(begin, bridge.endState());
			temp.Link();
			}
		ClearTags(pNode, ciBegin, endBranches);
		}

    static void MakeNode (TagNode *pNode, BranchesCItr &itrBranches, const _CharItr &ciLeft, const _CharItr &ciRight)
		{
		if(itrBranches < pNode->_M_branches.end())
            {
			(*itrBranches)->_M_entry.Set(ciLeft, ciRight);
            ++itrBranches;
            }
		else
			{
			TagNode *pNewNode = new TagNode();
            pNewNode->_M_entry.Create<_CharItr>();
            pNewNode->_M_entry.Set<_CharItr>(ciLeft, ciRight);
			pNode->_M_branches.push_back(pNewNode);
            itrBranches = pNode->_M_branches.end();
			}
		}

    void MakeMark (Bridge &bridge, TagNode *pNode)
		{
		if(pNode == m_TagTrie.root())
			return;
		if(bridge.beginState() == bridge.endState())
			{
			bridge.endState()->m_Mark.m_pTag = &pNode->_M_entry;
			bridge.endState()->m_Mark.m_Flag = MF_BOTH;
			}
		else
			{
			bridge.beginState()->m_Mark.m_pTag = &pNode->_M_entry;
			bridge.beginState()->m_Mark.m_Flag = MF_LEFT;
			bridge.endState()->m_Mark.m_pTag = &pNode->_M_entry;
			bridge.endState()->m_Mark.m_Flag = MF_RIGHT;
			}
		}

    void CreateParallel (const _CharItr &ciBegin, const _CharItr &ciEnd, Bridge &bridge, TagNode *pNode)
		/*  Pre:    [:len:] is the length of the expression including the terminator after
		 *			the last valid character
		 *	Post: 
		 */
		{
		_CharItr ciPos, ciNext, ciPosLeftBracket, ciPosOld = ciBegin;

		BranchesCItr endBranch = pNode->_M_branches.begin();
		int countBackSlash = 0;
		int bracketDepth = 0;
		char ch;
		MakeMark(bridge, pNode);
		for(ciPos = ciBegin; ciPos <= ciEnd; ciPos = ciNext)
			{
			
            if (ciPos == ciEnd) ch = '\0';  // we can see, zero is not allowed in the string
            else { ch = *ciPos; ++(ciNext = ciPos); }
			if(ch == '\\')
				countBackSlash++;
			else
				{
				if(countBackSlash % 2 == 0)
					{
					switch(ch)
						{
						case '(':
							if(bracketDepth++ == 0)
								ciPosLeftBracket = ciPos;
							break;
						case ')':
							if(--bracketDepth == 0)
								{
								MakeNode(pNode, endBranch, ciPosLeftBracket, ciNext);
								break;
								}
							else if(bracketDepth > 0)
								break;
							// else go on to the next case
						case '|': case '\0': // '|', '\0', ')' 
							if(ch == ')' && bracketDepth < 0 || bracketDepth == 0)
							{
								bridge.begin().m_IsMulti |= (ch == '|');
								CreateSerial(ciPosOld, ciPos, bridge, pNode, endBranch);
								endBranch = pNode->_M_branches.begin();
								bracketDepth = 0;
								ciPosOld = ciNext;
							}
							break;
						}
					}
				countBackSlash = 0;
				}
            if (ciPos == ciEnd) break;
			}		
		}

    void DestroyFrom (State *pState, ClearList &clearList)
		{
		for(ClearList::reverse_iterator r = clearList.rbegin(); r < clearList.rend(); ++r)
			if(*r == pState) return;
		clearList.push_back(pState);
		for(State::TransfersItr it = pState->m_Transfers.begin(); it < pState->m_Transfers.end(); ++it)
			if(it->m_pOutlet)
				DestroyFrom(it->m_pOutlet, clearList);
		delete pState;		
		}

    template <class _ChItr>
        void _DestroyTags (TagNode *pNode)
        {
        pNode->_M_entry.Destroy<_ChItr>();

        BranchesCItr it;
        for (it = pNode->_M_branches.begin(); it < pNode->_M_branches.end(); ++it)
            if (*it)
                _DestroyTags<_ChItr>(*it);
        }

    template <class _ChItr>
            void _CreateTags (TagNode *pNode)
        {
        pNode->_M_entry.Create<_ChItr>();

        BranchesCItr it;
        for (it = pNode->_M_branches.begin(); it < pNode->_M_branches.end(); ++it)
            if (*it)
                _CreateTags<_ChItr>(*it);
        }

    template <class _ChItr>
            void _ResetTags (TagNode *pNode, _ChItr ci)
        {
        pNode->_M_entry.Set<_ChItr>(ci);

        BranchesCItr it;
        for (it = pNode->_M_branches.begin(); it < pNode->_M_branches.end(); ++it)
            if (*it)
                _ResetTags(*it, ci);
        }

public:
	RegAutomata () : m_pStart(NULL) {}
	
	RegAutomata (const _CharItr &ciBegin, const _CharItr &ciEnd)
		: m_pStart(NULL)
		{
		Create(ciBegin, ciEnd);
		}

	~RegAutomata ()
		{
		if (m_pStart != NULL)
			{
			ClearList clearList;
			DestroyFrom(m_pStart, clearList);
			}
		}

	void Create (const _CharItr &ciBegin, const _CharItr &ciEnd)
		/*  Pre:	[:expression:] must be termintated in '\0',
		 *			[:len:] is the length of the expression including the terminator,
		 *			e.g. for expression "abc", in memory it should be like
		 *			('a','b','c','\0'), hence the length is 4.
		 *	Post: 
		 */
		{
		Destroy();
		m_pStart = new State;
		Bridge bridge(m_pStart, NULL);
		CreateParallel(ciBegin, ciEnd, bridge, m_TagTrie.root());
		}

	void Destroy ()
		{
		if (m_pStart != NULL)
			{
			ClearList list;
			DestroyFrom(m_pStart, list);
			m_pStart = NULL;
			}
		m_TagTrie.Destroy();
		}

    State * GetStart ()
        {
        return m_pStart;
        }

    template <class _ChItr>
            void DestroyTags ()
        {
        _DestroyTags<_ChItr>(m_TagTrie.root());
        }

    template <class _ChItr>
            void CreateTags ()
        {
        _CreateTags<_ChItr>(m_TagTrie.root());
        }

    template <class _ChItr>
            void ResetTags (const _ChItr &ci)
        {
        _ResetTags<_ChItr>(m_TagTrie.root(), ci);
        }

#ifdef _RGL_AUTO_DEBUG

protected:
	bool IsEnd(State *pState) const
		{
		State::Transfers::reverse_iterator r = pState->m_Transfers.rbegin();
		if (r->m_Style == State::Transfer::k_Serial && r->m_CharSet.size() == 0 && r->m_pOutlet == NULL)
			return true;
		return false;
		}

	void ViewTransfer(State::Transfer &transfer) const
		{
		cout << "% {outlet: " << transfer.m_pOutlet << ", ";
		cout << "style: " << transfer.m_Style << ", ";
		switch(transfer.m_Style)
			{
			case State::Transfer::k_Serial:
				{
				cout << "string:";
				String::iterator iter;
				for(iter = transfer.m_String.begin(); iter != transfer.m_String.end(); ++iter)
					cout << *iter;
				cout << ", ";
				break;
				}
			case State::Transfer::k_Inclusive:
			case State::Transfer::k_Exclusive:
				{
				cout << "charlist:";
				CharSet::iterator iter;
				for(iter = transfer.m_CharSet.begin(); iter != transfer.m_CharSet.end(); ++iter)
					cout << *iter;
				cout << ", ";
				break;
				}
			case State::Transfer::k_Shortcut:
				cout << "Shortcut:" << transfer.m_pTag;
				break;
			}
		if(transfer.m_Counter.assigned())
			{
			State::CounterData counterData = *transfer.m_Counter;
			State::CountSet::iterator iter;
			cout << "count: ";
			for(iter = counterData.m_CountSet.begin(); iter != counterData.m_CountSet.end(); ++iter)
				cout << iter->m_Low << ", " << iter->m_High << ";";
			}
		cout << "};" << endl;
		}

	void ViewFromState(State *pState, ClearList &clearList) const
		{
		if (pState == NULL)
			return;

		for(ClearList::reverse_iterator r = clearList.rbegin(); r < clearList.rend(); ++r)
			if(*r == pState) 
				return;
		clearList.push_back(pState);
		cout << "% [" << pState << "] ";
		if (IsEnd(pState)) cout << "* ";
		cout << "mark: " << pState->m_Mark.m_pTag << ":" << (int)pState->m_Mark.m_Flag << endl;
		for(State::TransfersItr it = pState->m_Transfers.begin(); it < pState->m_Transfers.end(); ++it)
			ViewTransfer(*it);
		for(State::TransfersItr it = pState->m_Transfers.begin(); it < pState->m_Transfers.end(); ++it)
			ViewFromState(it->m_pOutlet, clearList);
		}


	void ViewTree(const TagNode *pNode) const
		{
		cout << "% pNode: " << pNode << " BEGIN" << endl;

		for(BranchesCItr it = pNode->_M_branches.begin(); it < pNode->_M_branches.end(); ++it)
			if(*it)
				ViewTree(*it);
		cout << "% pNode: " << pNode << " END" << endl;
		}

public:
	void View() const
	{
		ClearList clearList;
		cout << "% states:" << endl;
		ViewFromState(m_pStart, clearList);
		cout << "% tree:" << endl;
		if(m_TagTrie.root() != NULL) ViewTree(m_TagTrie.root());
	}
	
#endif	// ifdef _RGL_AUTO_DEBUG

	};	// class RegAutomata

bool operator< (const RegAutomataBase::State::CountRange &left, 
		const RegAutomataBase::State::CountRange &right)
	{
	if (left.m_High < right.m_High)
		return true;
	if (left.m_High > right.m_High)
		return false;
	if (left.m_Low < right.m_Low)
		return true;
	return false;
	}

_QTL_END

#endif	// ifndef _QTL_rgl_auto_H

/* ========== End of "rgl_auto.h" included in "rgl_pars.h" ========== */


#include <vector>
#include <cassert>


#define _RGL_PARS_OPTIMIZE

_QTL_BEGIN
	
template <class _CharItr> class RegExpParser
	{
protected:

	typedef RegAutomataBase			            Automata;
	typedef typename Automata::State			State;

	typedef typename Automata::ClearList		ClearList;
	typedef typename Automata::CharSet			CharSet;
	typedef typename CharSet::const_iterator	CharSetCItr;

	typedef typename State::Transfer			Transfer;
	typedef typename State::TransfersItr		TransferItr;
    typedef std::vector< Transfer * >			TransferPtrs;
	typedef typename TransferPtrs::const_iterator	
												TransferPtrsCItr;
    typedef typename Automata::TagNode			TagNode;
	typedef typename Automata::BranchesCItr		BranchesCItr;

	typedef typename State::CountSet			CountSet;
	typedef typename CountSet::const_iterator	CountSetCItr;

	typedef	typename State::CounterData			CounterData;


	typedef unsigned char						MarkFlagVal;


	enum Status {k_Matched, k_Unmatched, k_Success, k_Failure};
	enum CounterStat {k_Noncount = 0, k_Inside, k_Outside, k_Max};

    enum
    {
        k_LogicalInfinite = -1
    };

	enum MarkFlag 
		{
		k_MfNull		= 0,
		k_MfLeft        = 1,
		k_MfRight       = 2,
		k_MfBoth        = 3,
		k_MfBothVisited = 4,
		k_MfBasicMask   = 0x07,
		k_MfEventMask   = 0x10
		};

    struct Packet
		{
        enum Style {
			k_Noncount, 
			k_Inside, 
			k_Outside, 
			k_Reserved, 
			k_PureCountLoop,
            k_NoncountLoop, 
			k_NoncountLoopVisited, 
			k_CountLoop, 
			k_Single
			};
                
        Style m_Style;
        State *m_pState;
        int m_Count;

        union
			{
            struct
				{
                _CharItr	m_Pos;
                int			m_Index;
                int			m_Pace;
				};
            struct
				{
                _CharItr	m_Left;
                _CharItr	m_Right;
				};
			};

		};	// struct Packet

	class Track
		{
	protected:
		typedef unsigned long		    UInt;

        struct IntUnit
        {
            union
            {
                UInt            uint_val;
                int             int_val;
                State *         state_ptr;
                typename Packet::Style   style;
            };

            IntUnit (int val) : int_val(val) {}
            IntUnit (UInt val) : uint_val(val) {}
            IntUnit (State *val) { state_ptr = val; }
            IntUnit (typename Packet::Style val) { style = val; }
        };

        typedef std::vector<IntUnit>	IntStack;
        typedef std::vector<_CharItr>   CItStack;
		IntStack					    m_IntStack;
		CItStack					    m_CItStack;

	public:	

		bool IsEmpty ()
			{
			return (m_IntStack.empty());
			}

		void PushPacket (const Packet &packet)
			{
			switch (packet.m_Style)
				{
				case Packet::k_Noncount: case Packet::k_Inside: case Packet::k_Outside:
					m_IntStack.push_back(packet.m_Pace);
					m_IntStack.push_back(packet.m_Index);
					m_CItStack.push_back(packet.m_Pos);
					break;
				case Packet::k_PureCountLoop:
					m_IntStack.push_back(packet.m_Count);
					m_IntStack.push_back(packet.m_Pace);
					m_IntStack.push_back(packet.m_Index);
					m_CItStack.push_back(packet.m_Pos);
					break;
				case Packet::k_NoncountLoop: case Packet::k_NoncountLoopVisited:
					m_CItStack.push_back(packet.m_Right);
					m_CItStack.push_back(packet.m_Left);
					break;
				case Packet::k_CountLoop:
					m_CItStack.push_back(packet.m_Right);
					m_CItStack.push_back(packet.m_Left);
					m_IntStack.push_back(packet.m_Count);
					break;
				case Packet::k_Single:
					m_CItStack.push_back(packet.m_Pos);
					break;
				}
			m_IntStack.push_back(packet.m_pState);
			m_IntStack.push_back(packet.m_Style);	
			}

		void PopPacket (Packet &packet)
			{
			packet.m_Style = m_IntStack.back().style;	m_IntStack.pop_back();
			packet.m_pState = m_IntStack.back().state_ptr;	m_IntStack.pop_back();
			switch (packet.m_Style)
				{
				case Packet::k_Noncount: case Packet::k_Inside: case Packet::k_Outside:
					packet.m_Pos = m_CItStack.back();	m_CItStack.pop_back();
					packet.m_Index = m_IntStack.back().int_val;	m_IntStack.pop_back();
					packet.m_Pace = m_IntStack.back().int_val;	m_IntStack.pop_back();
					break;
				case Packet::k_PureCountLoop:
					packet.m_Pos = m_CItStack.back();	m_CItStack.pop_back();
					packet.m_Index = m_IntStack.back().int_val;	m_IntStack.pop_back();
					packet.m_Pace = m_IntStack.back().int_val;	m_IntStack.pop_back();
					packet.m_Count = m_IntStack.back().int_val;	m_IntStack.pop_back();
					break;
				case Packet::k_NoncountLoop: case Packet::k_NoncountLoopVisited:
					packet.m_Left = m_CItStack.back();	m_CItStack.pop_back();
					packet.m_Right = m_CItStack.back();	m_CItStack.pop_back();
					break;
				case Packet::k_CountLoop:
					packet.m_Count = m_IntStack.back().int_val;	m_IntStack.pop_back();
					packet.m_Left = m_CItStack.back();	m_CItStack.pop_back();
					packet.m_Right = m_CItStack.back();	m_CItStack.pop_back();
					break;
				case Packet::k_Single:
					packet.m_Pos = m_CItStack.back();	m_CItStack.pop_back();
					break;
				}			
			}

		void FreeEvents ()
			{
			State *pState;
			typename Packet::Style style;

			typename IntStack::reverse_iterator iter;

			for (iter = m_IntStack.rbegin(); 
				iter > m_IntStack.rend() && (style = iter->style) >= Packet::k_NoncountLoop; )
				{
                --iter;
				pState = iter->state_ptr;
				pState->m_Mark.m_Flag &= ~k_MfEventMask;
				switch (style)
					{
					case Packet::k_NoncountLoop: case Packet::k_NoncountLoopVisited:
						iter -= 1;		// subtract 2 totally
						break;
					case Packet::k_CountLoop:
						iter -= 2;		// subtract 3 totally
						break;
					case Packet::k_Single:
						iter -=	1;		// subtract 2 totally
						break;
					}

				}
			}

		void Clear ()
			{
			FreeEvents();
			m_IntStack.clear();
			m_CItStack.clear();
			}

		};	// struct Track


    //  parsing data
    Track			m_Track;
    CounterStat		m_CounterStat;
    State			*m_pCurrent;
    ClearList		m_Pioneers;
    TransferPtrs	m_ForbiddenList;

	//	string data
	_CharItr		m_ciBegin, m_ciEnd, m_ciPos;

    static bool IsPureCountLoop (State *pState)
		{
		return (pState->m_Transfers.begin()->m_Counter &&  pState->m_Transfers.begin()->m_pOutlet == pState);
		}

    static bool IsCounterAssigned (State *pState)
		{
	    return (pState->m_Transfers.begin()->m_Counter);
		}

    static int GetUserCount (State *pState)
		{
		Transfer &transfer = *pState->m_Transfers.begin();
		if (!transfer.m_Counter)
			return -1;
		return transfer.m_Counter->m_UsrCount;
		}

    static void SetUserCount (State *pState, int value)
		{
		Transfer &transfer = *pState->m_Transfers.begin();
		if (!transfer.m_Counter)
			return;
		transfer.m_Counter->m_UsrCount = value;
		}

    static MarkFlagVal GetBasicFlag(State *pState)
		{
		return (pState->m_Mark.m_Flag & k_MfBasicMask);
		}

    static void SetBasicFlag (State *pState, MarkFlagVal value)
		{
		pState->m_Mark.m_Flag |= k_MfBasicMask;
		pState->m_Mark.m_Flag &= value;
		}

    static bool GetEventFlag (State *pState)
		{
	    return ((pState->m_Mark.m_Flag & k_MfEventMask) != 0);
		}

    static void SetEventFlag (State *pState, bool value)
		{
		if (value)
			pState->m_Mark.m_Flag |= k_MfEventMask;
		else
			pState->m_Mark.m_Flag &= ~k_MfEventMask;
		}

    bool IsForbidden (Transfer *pTransfer) const
		{
		for (TransferPtrsCItr iter = m_ForbiddenList.begin(); iter < m_ForbiddenList.end(); ++iter)
			if (pTransfer == *iter)
				return true;
		return false;		
		}

    bool IsPioneer (State *pState) const
		{
		if (pState == m_pCurrent)
			return true;
		for (ClearList::const_iterator iter = m_Pioneers.begin(); iter < m_Pioneers.end(); ++iter)
			if (pState == *iter)
				return true;
		return false;		
		}

    int MatchSerial (Transfer &transfer)
		{
		int count = (int)transfer.m_String.size();
		if (count == 0)
			{
			if (IsForbidden(&transfer))
				return -1;
			#ifdef _RGL_PARS_OPTIMIZE
				if (transfer.m_pOutlet == NULL)
					return 0;
				if (IsPioneer(transfer.m_pOutlet))
					return -1;
				m_Pioneers.push_back(transfer.m_pOutlet);
				for (TransferItr j = transfer.m_pOutlet->m_Transfers.begin(); j < transfer.m_pOutlet->m_Transfers.end(); ++j)
					if (MatchAdvance(*j) >= 0)
						return 0;
				return -1;
			#endif
			}

		_CharItr ciEnd = m_ciPos + count;

		if (ciEnd > m_ciEnd)
			return -1;

        std::string::iterator ciSrc;
		_CharItr ciDst;
		for (ciDst = m_ciPos, ciSrc = transfer.m_String.begin(); ciDst < ciEnd; ++ciDst, ++ciSrc)
			if (*ciSrc != *ciDst)
				return -1;
		return count;
		}

    int MatchInclusive (const Transfer &transfer) const
		{
		if (m_ciPos >= m_ciEnd)
			return -1;
		for (CharSetCItr it = transfer.m_CharSet.begin(); it != transfer.m_CharSet.end(); ++it)
			if (*m_ciPos == *it)
				return 1;
		return -1;
		}

    int MatchExclusive (const Transfer &transfer) const
		{
		if (m_ciPos >= m_ciEnd)
			return -1;
		for (CharSetCItr it = transfer.m_CharSet.begin(); it != transfer.m_CharSet.end(); ++it)
			if (*m_ciPos == *it)
				return -1;
		return 1;
		}

    int MatchShortcut (const Transfer &transfer) const
		{
		if (transfer.m_pTag == NULL || transfer.m_pTag->GetLeft<_CharItr>() == transfer.m_pTag->GetRight<_CharItr>() ||
			m_ciEnd - m_ciPos < transfer.m_pTag->GetRight<_CharItr>() - transfer.m_pTag->GetLeft<_CharItr>())
			return -1;
		_CharItr ci1, ci2;
		for (ci1 = transfer.m_pTag->GetLeft<_CharItr>(), ci2 = m_ciPos; ci1 < transfer.m_pTag->GetRight<_CharItr>(); ++ci1, ++ci2)
			if (*ci1 != *ci2)
				return -1;
		return (int)(ci1 - transfer.m_pTag->GetLeft<_CharItr>());		
		}

    int MatchAdvance (Transfer &transfer)
		{
		switch (transfer.m_Style)
			{
			case Transfer::k_Serial:
				return MatchSerial(transfer);
			case Transfer::k_Inclusive:
				return MatchInclusive(transfer);
			case Transfer::k_Exclusive:
				return MatchExclusive(transfer);
			default:
				return 0;
			}		
		}

    int Match (Transfer &transfer)
		{
		int res;
		switch (transfer.m_Style)
			{
			case Transfer::k_Serial:
				res = MatchSerial(transfer);
				m_Pioneers.clear();
				return res;
			case Transfer::k_Inclusive:
				return MatchInclusive(transfer);
			case Transfer::k_Exclusive:
				return MatchExclusive(transfer);
			case Transfer::k_Shortcut:
				return MatchShortcut(transfer);
			default:
				return -1;
			}		
		}

    CounterStat GetCounterStat(State *pState) const
		{
		Transfer &transfer = *(pState->m_Transfers.begin());
		if (!transfer.m_Counter)
			return k_Noncount;
		CounterData &counterData = *transfer.m_Counter;
		CountSet &countSet = counterData.m_CountSet;
		int count = counterData.m_UsrCount;
		int max = 0;
		bool inside = false;

		for (CountSetCItr it = countSet.begin(); it != countSet.end(); ++it)
			{
			if (max < it->m_High || it->m_High == k_LogicalInfinite)
				max = it->m_High;

			if (inside || count >= it->m_Low && (count <= it->m_High || it->m_High == k_LogicalInfinite))
				{
				inside = true;
				if (count < max || max == k_LogicalInfinite)
					return k_Inside;
				}
			}
		if (count == max)
			return k_Max;
		return k_Outside;
		}

    bool Enter (Packet &packet)
		{
		bool result = !GetEventFlag(m_pCurrent);
		switch (MarkFlagVal flag = GetBasicFlag(m_pCurrent))
			{
			case k_MfLeft:
				if (result)
					{
					packet.m_Style = Packet::k_Single;
					packet.m_Pos = m_pCurrent->m_Mark.m_pTag->GetLeft<_CharItr>();
					#ifdef _RGL_PARS_DEBUG
						cout << "% To push left parenthesis: " << (long)(packet.m_Pos - m_ciBegin) << endl;
					#endif
					}
				#ifdef _RGL_PARS_DEBUG
					cout << "% Left parenthesis: " << (long)(m_ciPos - m_ciBegin) << endl;
				#endif
				m_pCurrent->m_Mark.m_pTag->GetLeft<_CharItr>() = m_ciPos;
				return result;
			case k_MfRight:
				if (result)
					{
					packet.m_Style = Packet::k_Single;
					packet.m_Pos = m_pCurrent->m_Mark.m_pTag->GetRight<_CharItr>();
					#ifdef _RGL_PARS_DEBUG
						cout << "% To push right parenthesis: " << (long)(packet.m_Pos - m_ciBegin) << endl;
					#endif
					}
				#ifdef _RGL_PARS_DEBUG
					cout << "% Right parenthesis: " << (long)(m_ciPos - m_ciBegin) << endl;
				#endif
				m_pCurrent->m_Mark.m_pTag->GetRight<_CharItr>() = m_ciPos;
				return result;
			case k_MfBoth:
				if (result)
					{
					packet.m_Count = GetUserCount(m_pCurrent);
					packet.m_Left = m_pCurrent->m_Mark.m_pTag->GetLeft<_CharItr>();
					packet.m_Right = m_pCurrent->m_Mark.m_pTag->GetRight<_CharItr>();
					packet.m_Style = (packet.m_Count >= 0) ? Packet::k_CountLoop : 
						(flag == k_MfBoth)? Packet::k_NoncountLoop : Packet::k_NoncountLoopVisited;
					#ifdef _RGL_PARS_DEBUG
						cout << "% To push parentheses: " << (void *)&*packet.m_Left << ", " <<
							(void *)&*packet.m_Right << ": " << packet.m_Count << endl;
					#endif
					}
				#ifdef _RGL_PARS_DEBUG
					cout << "% new parentheses: " << m_ciPos << endl;
				#endif
				m_pCurrent->m_Mark.m_pTag->GetLeft<_CharItr>() = m_ciEnd;	// put to the invalid position
				m_pCurrent->m_Mark.m_pTag->GetRight<_CharItr>() = m_ciPos;
				SetBasicFlag(m_pCurrent, k_MfBothVisited);
				return result;
			case k_MfBothVisited:
				if (result)
					{
					packet.m_Count = GetUserCount(m_pCurrent);
					packet.m_Left = m_pCurrent->m_Mark.m_pTag->GetLeft<_CharItr>();
					packet.m_Right = m_pCurrent->m_Mark.m_pTag->GetRight<_CharItr>();
					packet.m_Style = (packet.m_Count >= 0) ? Packet::k_CountLoop : Packet::k_NoncountLoopVisited;
					#ifdef _RGL_PARS_DEBUG
						cout << "% To push parentheses: " << packet.m_Left << ", " <<
							packet.m_Right << ": " << packet.m_Count << endl;
					#endif
					}
				m_pCurrent->m_Mark.m_pTag->GetLeft<_CharItr>() = m_pCurrent->m_Mark.m_pTag->GetRight<_CharItr>();
				m_pCurrent->m_Mark.m_pTag->GetRight<_CharItr>() = m_ciPos;
				#ifdef _RGL_PARS_DEBUG
					cout << "% Visited parentheses: " << m_ciPos << endl;
					cout << "% We have: " << m_pCurrent->m_Mark.m_pTag << ": " <<
						m_pCurrent->m_Mark.m_pTag->GetLeft<_CharItr>() << ", " << m_pCurrent->m_Mark.m_pTag->GetRight<_CharItr>() << endl;
				#endif
				return result;
			default:
				return false;
			}
		}

    void SearchTransfer (TransferItr &iterTf, int &pace)
		{
		pace = -1;
		if(m_CounterStat == k_Outside)
			{
			for(; iterTf < m_pCurrent->m_Transfers.end() && iterTf->m_Counter && 
				(pace = Match(*iterTf)) < 0; ++iterTf);
			}
		else
			{
			for(; iterTf < m_pCurrent->m_Transfers.end() && (pace = Match(*iterTf)) < 0; ++iterTf);
			}
		}
    
	void Leave (const TransferItr &iterTf)
		{
		if (GetBasicFlag(m_pCurrent) == k_MfBothVisited && iterTf + 1 == m_pCurrent->m_Transfers.end() )
			{
			if (m_pCurrent->m_Mark.m_pTag->GetLeft<_CharItr>() == m_ciEnd)
				{
				#ifdef _RGL_PARS_DEBUG
					cout << "% Parentheses incompleted!" << endl;
				#endif
				m_pCurrent->m_Mark.m_pTag->Set(m_ciBegin);	// reset
				}
			#ifdef _RGL_PARS_DEBUG
				cout << "% Leaving: " << (void *)&*m_pCurrent->m_Mark.m_pTag->GetLeft<_CharItr>() << ", " <<
					(void *)&*m_pCurrent->m_Mark.m_pTag->GetRight<_CharItr>() << endl;
			#endif
			SetBasicFlag(m_pCurrent, k_MfBoth);
			}

		if (m_CounterStat == k_Inside)
			{
			if (iterTf->m_Counter)
				iterTf->m_Counter->m_UsrCount++;
			else
				m_pCurrent->m_Transfers.begin()->m_Counter->m_UsrCount = 0;
			}
		else if (m_CounterStat == k_Outside)
			iterTf->m_Counter->m_UsrCount++;
		}

    void DoTransfer (const TransferItr &iterTf, int pace)
		{
		m_ciPos += pace;
		#ifdef _RGL_PARS_DEBUG
			assert(iterTf >= m_pCurrent->m_Transfers.begin());
		#endif
		State *pOutlet = iterTf->m_pOutlet;
		if (iterTf->m_Style == Transfer::k_Serial && iterTf->m_CharSet.empty() && !IsForbidden(&(*iterTf)))
			m_ForbiddenList.push_back(&(*iterTf));
		else
			m_ForbiddenList.clear();
		m_pCurrent = pOutlet;
		}

    void SaveAttempt (int index, int pace)
		{
		m_Track.FreeEvents();
		Packet packet;
		packet.m_pState = m_pCurrent;
		packet.m_Pos = m_ciPos;
		packet.m_Index = index;
		packet.m_Pace = pace;
		if (IsPureCountLoop(m_pCurrent))
		{
			packet.m_Style = Packet::k_PureCountLoop;
			packet.m_Count = GetUserCount(m_pCurrent);
		}
		else
			packet.m_Style = (typename Packet::Style)m_CounterStat;
		m_Track.PushPacket(packet);
		}

    void SaveExtra (Packet &packet)
		{
		packet.m_pState = m_pCurrent;
		SetEventFlag(m_pCurrent, true);
		m_Track.PushPacket(packet);
		}


    bool Restore (Packet &packet)
		{
		while (true)
			{
			if (m_Track.IsEmpty())
				return false;

			m_Track.PopPacket(packet);

			if (packet.m_Style < Packet::k_NoncountLoop)
				break;

            RegAutomataBase::Tag *pTag = packet.m_pState->m_Mark.m_pTag;

			switch (packet.m_Style)
				{
				case Packet::k_NoncountLoop:
					pTag->GetLeft<_CharItr>() = packet.m_Left;
					pTag->GetRight<_CharItr>() = packet.m_Right;
					packet.m_pState->m_Mark.m_Flag = k_MfBoth;
					break;
				case Packet::k_NoncountLoopVisited:
					pTag->GetLeft<_CharItr>() = packet.m_Left;
					pTag->GetRight<_CharItr>() = packet.m_Right;
					packet.m_pState->m_Mark.m_Flag = k_MfBothVisited;
					break;
				case Packet::k_CountLoop:
					pTag->GetLeft<_CharItr>() = packet.m_Left;
					pTag->GetRight<_CharItr>() = packet.m_Right;
					SetUserCount(packet.m_pState, packet.m_Count);
					packet.m_pState->m_Mark.m_Flag = (packet.m_Count == 0)? k_MfBoth : k_MfBothVisited;
					break;
				case Packet::k_Single:
					SetEventFlag(packet.m_pState, false);
					switch (packet.m_pState->m_Mark.m_Flag)
						{
						case k_MfLeft:
							pTag->GetLeft<_CharItr>() = packet.m_Pos;
							break;
						case k_MfRight:
							pTag->GetRight<_CharItr>() = packet.m_Pos;
							break;
						}
					break;
				}
			}
		#ifdef _RGL_PARS_DEBUG
			cout << "% Restoring pState: " << packet.m_pState << "..." << endl;
		#endif
		m_pCurrent = packet.m_pState;
		m_ciPos = packet.m_Pos;
		m_CounterStat = (CounterStat)packet.m_Style;
		if (packet.m_Style == Packet::k_PureCountLoop)
			SetUserCount(m_pCurrent, packet.m_Count);
		return true;
		}

    Status AttemptNormal ()
		{
		Packet packet;
		bool extra = Enter(packet);
		int pace1, pace2;
		TransferItr iterTf = m_pCurrent->m_Transfers.begin(), iterTf2;
		SearchTransfer(iterTf, pace1);
		if(pace1 < 0)
			{
			if(extra)
				SaveExtra(packet);
			else if(IsPureCountLoop(m_pCurrent))
				SetUserCount(m_pCurrent, 0);
			return k_Unmatched;
			}
		++(iterTf2 = iterTf);
		SearchTransfer(iterTf2, pace2);
		if(pace2 >= 0)
			{
			int index = (long)(iterTf2 - m_pCurrent->m_Transfers.begin());
			SaveAttempt(index, pace2);
			#ifdef _RGL_PARS_DEBUG
				cout << "% Pushing pState: " << m_pCurrent << ", pos: " << (long)(m_ciPos - m_ciBegin) <<
					", index: " << index << endl;
			#endif
			}
		if(extra)
			SaveExtra(packet);
		Leave(iterTf);
		DoTransfer(iterTf, pace1);
		return k_Matched;
		}

    Status AttemptMax ()
		{
		Packet packet;
		bool extra = Enter(packet);
		TransferItr iterLast = m_pCurrent->m_Transfers.end();
		--iterLast;
		if(iterLast->m_Counter)
			return k_Success;
		int pace = Match(*iterLast);
		if(pace < 0)
			{
			if(extra)
				SaveExtra(packet);
			return k_Unmatched;
			}
		if(extra)
			SaveExtra(packet);
		SetUserCount(m_pCurrent, 0);
		DoTransfer(iterLast, pace);
		return k_Matched;
		}

    Status Attempt ()
		{
		#ifdef _RGL_PARS_DEBUG
			cout << "% Attempting pState: " << m_pCurrent << ", pos: " << (long)(m_ciPos - m_ciBegin) << endl;
		#endif
		if (m_pCurrent == NULL)
			return k_Success;
		switch (m_CounterStat = GetCounterStat(m_pCurrent))
			{
			case k_Noncount: case k_Inside: case k_Outside:
				return AttemptNormal();
			case k_Max:
				return AttemptMax();
			default:
				return k_Failure;
			}		
		}
    
	Status ReAttempt()
		{
		Packet packet;
		if (Restore(packet) == false)
			return k_Failure;

		m_ForbiddenList.clear();

		#ifdef _RGL_PARS_DEBUG
			cout << "% Reattempting pState: " << m_pCurrent << ", pos: "
				<< (long)(m_ciPos - m_ciBegin) << ", index: " << packet.m_Index << endl;
		#endif


		int pace1 = packet.m_Pace, pace2;
		TransferItr iterTf = m_pCurrent->m_Transfers.begin(), iterTf2;
		bool extra = Enter(packet);
		iterTf += packet.m_Index;
		++(iterTf2 = iterTf);
		SearchTransfer(iterTf2, pace2);
		if (pace2 >= 0)
			{
			int index = (int)(iterTf2 - m_pCurrent->m_Transfers.begin());
			SaveAttempt(index, pace2);
			#ifdef _RGL_PARS_DEBUG
				cout << "% Pushing pState: " << m_pCurrent << ", pos: " << (long)(m_ciPos - m_ciBegin) <<
					", index: " << (packet.m_Index + 1) << endl;
			#endif
			}
		if (extra)
			SaveExtra(packet);

		Leave(iterTf);
		DoTransfer(iterTf, pace1);
		return k_Matched;
		}

    bool Process()
		{
		Status res = k_Matched;
		while (true)
			{
			switch (res)
				{
				case k_Matched:
					res = Attempt();
					#ifdef _RGL_PARS_DEBUG
						cout << "% Attempt() returns " << res << endl;
					#endif
					break;
				case k_Unmatched:
					res = ReAttempt();
					break;
				case k_Success:
					return true;
				case k_Failure:
					return false;
				}
			}
		}

public:
    template <class _RegAutomata>
            void Init (_RegAutomata &automata)
        {
        automata.CreateTags<_CharItr>();
        }

    template <class _RegAutomata>
            void Deinit (_RegAutomata &automata)
        {
        automata.DestroyTags<_CharItr>();
        }

    template <class _RegAutomata>
	        bool Parse (_RegAutomata &automata, const _CharItr &ciBegin, const _CharItr &ciEnd, _CharItr &ciMatchEnd)
		{
		if ( (m_pCurrent = automata.GetStart()) == NULL )
			return false;
		m_ciPos = m_ciBegin = ciBegin;
		m_ciEnd = ciEnd;

		#ifdef _RGL_PARS_DEBUG
			cout << "m_ciBegin = " << (void *)&*m_ciBegin << "; m_ciEnd = " << (void *)&*m_ciEnd << endl;
		#endif
        automata.ResetTags<_CharItr>(m_ciBegin);

		bool res = Process();
		m_Track.Clear();
		ciMatchEnd = m_ciPos;
		return res;
		}

	};	// class RegExpParser


_QTL_END


#if defined(RGL_PARS_DEBUG) && RGL_PARS_DEBUG

using namespace std;
using namespace qtl;


void print_manual ()
{
    printf("Not available\n");
}

int file_len (FILE *fIn)
{
    int resvpos = ftell(fIn);
    fseek(fIn, 0, SEEK_END);
    int res = ftell(fIn);
    fseek(fIn, resvpos, SEEK_SET);
    return res;
}

int TestEntry  (int argc, char **argv)
{
    typedef string::iterator	stritr;
    typedef RegAutomata<stritr> rgl_auto;
    typedef RegExpParser<unsigned char *> rgl_pars;

    char inpbuf[256];
    unsigned char *in_file_buf = NULL;
    int infbuf_len = 0;
    bool quit = false;
    while (!quit)
    {
        printf("-");
        int c = getch();
        printf("%c\n", c);
        switch (c)
        {
        case 'Q': case 'q':
            quit = true;
            break;
        case 'F': case 'f':
            {
                unsigned char *start, *end, *match;

                printf("Name of the input file>");
                gets(inpbuf);

                FILE *fIn = fopen(inpbuf, "rb");
                if (!fIn)
                {
                    printf("Error opening input file\n");
                    break;
                }

                printf("Name of the output file>");
                gets(inpbuf);

                FILE *fOut = fopen(inpbuf, "wb");

                int flen = file_len(fIn);
                if (flen == 0)
                {
                    printf("Empty file\n");
                    break;
                }

                if (flen > 32*1024*1024)
                {
                    printf("Too large file\n");
                    break;
                }
                if (infbuf_len < flen)
                {
                    in_file_buf = (unsigned char *)realloc(in_file_buf, flen);
                    infbuf_len = flen;
                }

                fread(in_file_buf, 1, flen, fIn);

                printf("Pattern>");
                gets(inpbuf);
                string pattern = inpbuf;

                rgl_auto	r(pattern.begin(), pattern.end());
                rgl_pars	pars;
                pars.Init(r);

                start = in_file_buf;
                end = in_file_buf + flen;

                while (start < end)
                {
                    if (pars.Parse(r, start, end, match))
                    {
                        printf("find a match at offset %d with length %d\n", start - in_file_buf, match - start);
                        c = getch();
                        if (c == 'Q' || c == 'q')
                        {
                            break;
                        }
                        start++;
                    }
                    else
                    {
                        start++;
                    }
                }
                pars.Deinit(r);
                fclose(fIn);
                fclose(fOut);
            }
            break;
        }
    }


    return 0;
}

#endif  /* if defined(RGL_PARS_DEBUG) && RGL_PARS_DEBUG */

#endif	// ifndef QTL_rgl_pars_H
