#ifndef _regexp_replacer_H
#define _regexp_replacer_H

#include "regexp.h"

_QTL_BEGIN

template <class _RegExp>
class RegExpReplacer
{
public:
    typedef _RegExp RegExp;
    typedef typename RegExp::Shortcut   Shortcut;
    typedef typename RegExp::Automata::Char Char;
    typedef typename RegExp::TextItr    TextItr;
    typedef std::string String;

    template <class PtrnItr>
    bool AssignPattern(RegExp &re, PtrnItr &begin, const PtrnItr &end);

    bool GetRepString (String &str);

protected:
    struct Element
    {
        enum Type { k_String, k_Shortcut } type;
        void *pData;
        
        Element(void *p, Type t);

        Shortcut GetShortcut();
        String & GetString();
    };
    std::vector<Element>    m_Elements;

    void AddString(String s);
    void AddShortcut(Shortcut shortcut);
    void ClearElements();

};  /* class RegExpReplacer */

template <class _RegExp> template <class PtrnItr>
bool RegExpReplacer<_RegExp>::AssignPattern(RegExp &re, PtrnItr &p, const PtrnItr &end)
{
    String local;
    Shortcut shortcut;
    ClearElements();
    for (; p != end; )
    {
        if (*p == '\\')
        {
            PtrnItr pk = p; ++pk;
            if (*pk == 'k')
            {   // shortcut
                if (local != "")
                {
                    AddString(local);
                    local = "";
                }

                p = pk; ++p;

                if (!re.ParseShortcut(shortcut, p, end))
                {
                    return false;
                }
                AddShortcut(shortcut);
            }
            else
            {
                Char ch;
                if (!RegExp::Automata::ParseEscape(ch, p, end))
                {
                    return false;
                }
                local += ch;
            }
        }
        else
        {
            local += *p;
            ++p;
        }
    }
    if (local != "")
    {
        AddString(local);
    }
    return true;
}

template <class _RegExp>
bool RegExpReplacer<_RegExp>::GetRepString (String &str)
{
    str = "";
    size_t n = m_Elements.size();
    for (size_t i = 0; i < n; i++)
    {
        Element &element = m_Elements[i];
        if (element.type == Element::k_Shortcut)
        {
            TextItr tBegin, tEnd;
            Shortcut shortcut = element.GetShortcut();
            if (RegExp::RunShortcut(shortcut, tBegin, tEnd))
            {
                for (TextItr titr = tBegin; titr != tEnd; ++titr)
                {
                    str += *titr;
                }
            }
            else
            {
                return false;
            }
        }
        else    // string
        {
            str += element.GetString();
        }
    }
    return true;
}

template <class _RegExp>
RegExpReplacer<_RegExp>::Element::Element(void *p, Type t)
{
    pData = p;
    type = t;
}

template <class _RegExp>
typename RegExpReplacer<_RegExp>::Shortcut RegExpReplacer<_RegExp>::Element::GetShortcut()
{
    return static_cast<Shortcut>(pData);
}

template <class _RegExp>
typename RegExpReplacer<_RegExp>::String & RegExpReplacer<_RegExp>::Element::GetString()
{
    return*static_cast<String *>(pData);
}

template <class _RegExp>
void RegExpReplacer<_RegExp>::AddString(String s)
{
    String *pS = new String(s);
    m_Elements.push_back(Element(pS, Element::k_String));
}

template <class _RegExp>
void RegExpReplacer<_RegExp>::AddShortcut(Shortcut shortcut)
{
    m_Elements.push_back(Element(shortcut, Element::k_Shortcut));
}

template <class _RegExp>
void RegExpReplacer<_RegExp>::ClearElements()
{
    for (int i = 0; i < m_Elements.size(); i++)
    {
        if (m_Elements[i].pData && m_Elements[i].type == Element::k_String)
        {
            delete m_Elements[i].pData;
        }
    }
    m_Elements.clear();
}


_QTL_END

#endif  /* ifndef _regexp_replacer_H */
