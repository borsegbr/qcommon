#ifndef _bstmm_H
#define _bstmm_H

#include <qtl/ctnr/avltree.h>
#include "../all/predef.h"

_QUL_BEGIN

template <class _Ptr = unsigned char *, class _Size = std::size_t>
struct BstMMController
{
    typedef _Ptr    Ptr;
    typedef _Size   Size;

    struct SizeLessThan
    {
        bool operator()(const Size &s1, const Size &s2)
        {
            if (s1 < s2)
            {
                return true;
            }
            else if (s1 > s2)
            {
                return false;
            }
            else    /* s1 == s2 */
            {
                return ((&s1) < (&s2));
            }
        }
    };

    struct AdjustTraits : public qtl::avl_adjust_traits<Size, SizeLessThan>
    {
    private:
        typedef qtl::avl_adjust_traits<Size, SizeLessThan> _Mybase;

    public:
        Ptr nil;

        bool _isnil(node_itr __node) const
        {
            return ((Ptr)__node == nil);
        }

        node_itr _nil() const
        {
            return (node_itr)nil;
        }

        /*
         * <remarks>
         *  make sure that any node creation and destruction not be
         *  performed within the tree
         * </remarks>
         */
    };
    
    typedef typename AdjustTraits::avl_node Node;
    typedef Node *  NodePtr;
    
    struct AvlTree : public qtl::avl_tree<Size, SizeLessThan, false, AdjustTraits>
    {
        ~AvlTree() 
        {   /* detach root before destruction */
            _M_root = _nil(); 
        }
        void SetRoot(NodePtr pRoot) { _M_root = pRoot; }
        NodePtr GetRoot() { return (NodePtr)_M_root; }
    };

    void Initialize(Ptr pStart, Size nTotal)
    {
        NodePtr pRoot = (NodePtr)pStart;
        m_pStart = pStart;
        m_nTotal = nTotal;

        m_Tree.SetRoot(pRoot);
        m_Tree.nil = (Ptr)((Size)m_pStart - 1);
        new(pRoot) FMCHead(m_nTotal, NilPtr());

        FMCTail *pFmcTail = (FMCTail *)((Size)pStart + m_nTotal - FMCTail::StructSize());
        pFmcTail->indicator = m_nTotal;

    }

    /*
     * nSize: as input: the size of the memory indended to be allocated
     *        as output: actual size allocated
     */
    void Allocate(Ptr &p, Size &nSize)
    {
        Size nSizeAc = nSize + SizeBaseAC();
        NodePtr pNode = Search(nSizeAc);
        Size nTotalOverhead = SizeBaseAC() + SizeBaseFMC();
        if (IsNil((Ptr)pNode))   /* insufficient contiguous space */
        {
            p = NilPtr();
            nSize = 0;
            return;
        }

        Ptr pInit = (Ptr)pNode;
        p = (Ptr)((Size)pInit + ACHead::StructSize());

        m_Tree.remove_bypointer(pNode);
        static_cast<FMCHead*>(pNode)->~FMCHead();
        if (nSize + nTotalOverhead > pNode->_M_data)
        {   // too tight memory, just leave all the space for AC
            nSizeAc = pNode->_M_data;   // adjust the size
            nSize = nSizeAc - SizeBaseAC();
        }
        else
        {   // sufficient memory to make room for a free memory chunk
            Size nSizeFmc = pNode->_M_data - nSizeAc;
            FMCHead *pFmcHead = (FMCHead *)((Size)pInit + nSizeAc);
            FMCTail *pFmcTail = (FMCTail *)((Size)pInit + pNode->_M_data - FMCTail::StructSize());
            new(pFmcHead) FMCHead(nSizeFmc, NilPtr());
            new(pFmcTail) FMCTail(nSizeFmc);
            m_Tree.insert(pFmcHead);
        }
        ACHead *pAcHead = (ACHead*)pInit;
        ACTail *pAcTail = (ACTail*)((Size)pInit + nSizeAc - ACTail::StructSize());
        new(pAcHead) ACHead();
        new(pAcTail) ACTail();
    }

    void Deallocate(Ptr p, Size nSize)
    {
        Size nSizeAc = nSize + SizeBaseAC();
        Ptr pAc = (Ptr)((Size)p - ACHead::StructSize());

        Size nTotalFmc = nSizeAc;
        FMCHead *pFmc;

        if (pAc > m_pStart)
        {
            // memory before
            ChunkTail *pTailBefore = (ChunkTail *)((Size)pAc - ChunkTail::StructSize());
            Size sizeBefore = pTailBefore->indicator;
            if (sizeBefore == 0)
            {   // the preceding chunk is also AC
                pFmc = (FMCHead *)pAc;  // the new FMC begins from the current AC
            }
            else
            {   // the chunk is FMC
                // remove it
                FMCHead *pHeadBefore = (FMCHead *)((Size)pAc - sizeBefore);
                m_Tree.remove_bypointer(pHeadBefore);
                static_cast<FMCHead*>(pHeadBefore)->~FMCHead();
                nTotalFmc += sizeBefore;
                pFmc = pHeadBefore;
            }
        }
        else
        {
            pFmc = (FMCHead *)pAc;
        }

        // memory after
        ChunkHead *pHeadAfter = (ChunkHead *)((Size)pAc + nSizeAc);
        Size sizeAfter = pHeadAfter->indicator;
        if (sizeAfter == 0)
        {   // it's AC
        }
        else
        {   // it's FMC
            m_Tree.remove_bypointer((NodePtr)pHeadAfter);
            ((FMCHead*)(pHeadAfter))->~FMCHead();
            nTotalFmc += sizeAfter;
        }

        new(pFmc) FMCHead(nTotalFmc, NilPtr());
        new((Ptr)((Size)pFmc + nTotalFmc - FMCTail::StructSize())) FMCTail(nTotalFmc);
        m_Tree.insert(pFmc);
    }

protected:
    Ptr     m_pStart;
    Size    m_nTotal;
    AvlTree m_Tree;

    Ptr NilPtr()
    {
        return m_Tree.nil;
    }

    bool IsNil(Ptr p)
    {
        return (p == m_Tree.nil);
    }

    /*
     * <remarks>
     *  The structure of free memory chunk: 
     *   [Node structure], since _M_data lies at the beginning, it can be differentiated from AC
     *   [unused memory]
     *   [size indicator]
     *  The structure of allocated chunk
     *   [0] same size as Size
     *   memory for use
     *   [0] same size as Size
     * </remarks>
     */
    struct ChunkHead
    {
        Size    indicator;
        static Size StructSize() { return sizeof(ACHead); }
    };
    struct ChunkTail
    {
        Size    indicator;
        static Size StructSize() { return sizeof(ChunkTail); }
    };
    struct FMCHead : public Node
    {
        FMCHead(Size nSize, Ptr pNil) : Node(nSize) 
        {
            _M_parent = _M_left = _M_right = (NodePtr)pNil;
        }
        ~FMCHead() {}
        static Size StructSize() { return sizeof(FMCHead); }
    };
    struct FMCTail : public ChunkTail
    {
        FMCTail(Size nSize) { indicator = nSize; }
    };
    Size SizeBaseFMC()
    {
        int a = FMCHead::StructSize();
        return FMCHead::StructSize() + FMCTail::StructSize();
    }

    struct ACHead : public ChunkHead
    {
        ACHead() { indicator = 0; }
    };
    struct ACTail : public ChunkTail 
    {
        ACTail() { indicator = 0; }
    };
    Size SizeBaseAC()
    {
        return ACHead::StructSize() + ACTail::StructSize();
    }


    NodePtr Search(Size nSize)
    {
        NodePtr p = m_Tree.GetRoot();
        NodePtr pLast = p;
        while (!IsNil((Ptr)p))
        {
            pLast = p;
            if (p->_M_data < nSize)
            {
                p = (NodePtr)p->_M_right;
            }
            else
            {
                p = (NodePtr)p->_M_left;
            }
            if (pLast->_M_data == nSize && p->_M_data != nSize) break;
        }
        if (pLast->_M_data < nSize)
        {
            return (NodePtr)NilPtr();
        }
        else
        {
            return pLast;
        }
    }

};

_QUL_END

#endif /* ifndef _bstmm_H */
