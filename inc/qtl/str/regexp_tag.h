#ifndef _regexp_tag_H
#define _regexp_tag_H

#include "../all/predef.h"

_QTL_BEGIN


template <class TextItr>
struct EasyTagNode
{
    std::stack<TextItr> m_Left;
    TextItr     m_Right;
    int m_DepthLeft;
    int m_DepthRight;

    EasyTagNode()
    {
        m_DepthLeft = 0;
        m_DepthRight = 0;
    }

    bool GetTextItr(TextItr &begin, TextItr &end)
    {
        if (m_DepthLeft < 1 || m_DepthRight < 1)
        {
            return false;
        }
        begin = m_Left.top();
        end = m_Right;
        return true;
    }

    void BindLeft(TextItr titr)
    {
        if (m_DepthRight == 0)
        {
            m_Left.push(titr);
            m_DepthLeft++;
        }
        else
        {
            m_DepthRight = 0;
            m_Left.pop();
            m_Left.push(titr);
        }
    }

    bool BindRight(TextItr titr)
    {
        if (m_DepthRight == 0)
        {
            if (m_DepthLeft == 0)
            {
                return false;
            }
            else
            {
                m_DepthRight = 1;
                m_Right = titr;
            }
        }
        else
        {
            if (m_DepthLeft <= 1)
            {
                return false;
            }
            else
            {
                m_DepthLeft--;
                m_Left.pop();
                m_Right = titr;
            }
        }
        return true;
    }

};  /* struct TagNode */

template <class TextItr>
struct EasyTagState
{
    typedef EasyTagNode<TextItr>            TagNode;
    typedef std::map<TagNode *, TagNode>    TagMap;
    typedef typename TagMap::iterator       TagItr;
    TagMap changedNodes;

    void BindLeft(TagNode *pTag, TextItr titr)
    {
        TagItr tagitr = changedNodes.find(pTag);
        if (tagitr == changedNodes.end())
        {   // not found
            changedNodes[pTag] = *pTag;
        }
        pTag->BindLeft(titr);
    }

    void BindRight(TagNode *pTag, TextItr titr)
    {
        TagItr tagitr = changedNodes.find(pTag);
        if (tagitr == changedNodes.end())
        {   // not found
            changedNodes[pTag] = *pTag;
        }
        pTag->BindRight(titr);
    }

    void Restore()
    {
        for (TagItr tagitr = changedNodes.begin(); tagitr != changedNodes.end(); ++tagitr)
        {
            TagNode *pTag = tagitr->first;
            TagNode &tag = tagitr->second;
            *pTag = tag;
        }
        changedNodes.clear();
    }
};  /* struct TagState */

_QTL_END

#endif  /* ifndef _regexp_tag_H */
