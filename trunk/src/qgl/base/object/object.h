/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : xxx.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-06-08  quanben     Created.
 *          2008-10-29  quanben     use get_pointer() to retrieve internal 
 *                                  pointer of a auto pointer
 *
 ******************************************************************************/

#ifndef _object_H
#define _object_H

/*--------------------------------- Includes ---------------------------------*/



#include <list>
#include <qtl/ctnr/autoptr.h>

#include "qgl_common.h"


/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

using namespace std;

/*--------------------------------- Classes ----------------------------------*/


class Object    /* abstract */
{
public:

    Object () {}
    virtual ~Object () {}

};      /* class Object */

typedef Object *    ObjectPtr;

template <class _Type = ObjectPtr>
struct ItrCoreBase
{
    ItrCoreBase () {}
    virtual ~ItrCoreBase () {}

    virtual _Type & Get () = 0;

    virtual operator _Type ()
    {
        return Get();
    }
};

struct PtrItrCoreBase : public ItrCoreBase<ObjectPtr>
{
    template <class _PointerType>
    _PointerType GetDynPtr()
    {
        return dynamic_cast<_PointerType>(Get());
    }

    virtual bool IsValid ()
    {
        return false;
    }
};

typedef qtl::autoptr<PtrItrCoreBase>    Itr;

template <class _Type = ObjectPtr>
class Enumerator
{
public:
    typedef const _Type &   _TypeCref;

public:
    Enumerator ()
    {
    }

    virtual ~Enumerator ()
    {
    }

    virtual int GetCount () = 0;
    virtual void GetFirst (Itr &itr) = 0;
    virtual void GetLast (Itr &itr) = 0;
    virtual void GetNext (Itr &itr) = 0;
    virtual void GetPrev (Itr &itr) = 0;
    virtual bool IsValid (Itr &itr) = 0;

    virtual void Insert (Itr &itr, _TypeCref obj) = 0;
    virtual void Remove (Itr &itr) = 0;
    virtual bool Find (_TypeCref obj, Itr &itr) = 0;    
    
};

template <class _Type>
class GenericListEnumerator : public Enumerator<_Type>
{
    typedef Enumerator<_Type>   _Mybase;

protected:
    typedef list<_Type>                 List;
    typedef typename List::iterator     ListItr;

public:
    struct ItrCore : public PtrItrCoreBase
    {
        ListItr     listItr;

        ItrCore ()
        {
        }

        ItrCore (const ListItr &itr)
            : listItr(itr)
        {
        }

        virtual bool IsValid ()
        {
            return true;    /* always true, making no sense */
        }

    };

public:
    GenericListEnumerator ()
    {
    }

    virtual ~GenericListEnumerator ()
    {
    }

    virtual int GetCount ()
    {
        return (int)m_List.size();
    }

    virtual void GetFirst (Itr &itr)
    {
        ListItr listItr = m_List.begin();
        itr = new ItrCore(listItr);
    }

    virtual void GetLast (Itr &itr)
    {
        ListItr listItr = m_List.end();
        if (m_List.size() != 0)
        {
            --listItr;
        }
        itr = new ItrCore(listItr);
    }

    virtual void GetNext (Itr &itr)
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        if (base->listItr != m_List.end())
        {
            ++base->listItr;
        }
    }

    virtual void GetNext (Itr &itr) const
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        if (base->listItr != m_List.end())
        {
            ++base->listItr;
        }        
    }

    virtual void GetPrev (Itr &itr)
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        if (base->listItr != m_List.begin())
        {
            --base->listItr;
        }
    }

    virtual void GetPrev (Itr &itr) const
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        if (base->listItr != m_List.begin())
        {
            --base->listItr;
        }
    }

    virtual void Insert (Itr &itr, const ObjectPtr &obj)
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        m_List.insert(base->listItr, obj);
    }

    virtual void Remove (Itr &itr)
    {
        if (itr->IsValid())
        {
            ItrCore *core = (ItrCore *)itr.get_pointer();
            m_List.erase(core->listItr);
        }
    }

    virtual bool Find (const ObjectPtr &obj, Itr &itr)
    {
        for (GetFirst(itr); itr->IsValid(); GetNext(itr))
        {
            if (itr->Get() == obj)
            {
                return true;
            }
        }
        return false;
    }

    virtual bool IsValid (Itr &itr)
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        return base->listItr != m_List.end();
    }

protected:
    List    m_List;
};  /* GenericListEnumerator */


class ListEnumerator : public Enumerator<ObjectPtr>
{
    typedef Enumerator<ObjectPtr>   _Mybase;

protected:
    typedef list<ObjectPtr>     List;
    typedef List::iterator      ListItr;

public:
    struct ItrCore : public PtrItrCoreBase
    {
        ListItr     listItr;
        bool        valid;

        ItrCore () : valid(false)
        {
        }

        ItrCore (const ListItr &itr, bool _valid)
            : listItr(itr), valid(_valid)
        {
        }

        virtual ObjectPtr & Get ()
        {
            return dynamic_cast<Object *>(*listItr);
        }

        virtual bool IsValid ()
        {
            return valid;
        }
    };

public:
    ListEnumerator () 
    {
    }

    virtual ~ListEnumerator () {}

    virtual int GetCount ()
    {
        return (int)m_List.size();
    }

    virtual void GetFirst (Itr &itr)
    {
        ListItr listItr = m_List.begin();
        itr = new ItrCore(listItr, listItr != m_List.end());
    }

    virtual void GetLast (Itr &itr)
    {
        ListItr listItr = m_List.end();
        bool valid = false;
        if (m_List.size() != 0)
        {
            --listItr;
            valid = true;
        }
        itr = new ItrCore(listItr, valid);
    }

    virtual void GetNext (Itr &itr)
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        if (base->listItr != m_List.end())
        {
            ++base->listItr;
        }
        base->valid = base->listItr != m_List.end();
    }

    virtual void GetNext (Itr &itr) const
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        if (base->listItr != m_List.end())
        {
            ++base->listItr;
        }        
        base->valid = base->listItr != m_List.end();
    }

    virtual void GetPrev (Itr &itr)
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        if (base->listItr != m_List.begin())
        {
            --base->listItr;
            base->valid = true;
        }
    }

    virtual void GetPrev (Itr &itr) const
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        if (base->listItr != m_List.begin())
        {
            --base->listItr;
            base->valid = true;
        }
    }

    virtual void Insert (Itr &itr, const ObjectPtr &obj)
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        m_List.insert(base->listItr, obj);
    }

    virtual void Remove (Itr &itr)
    {
        if (itr->IsValid())
        {
            ItrCore *core = (ItrCore *)itr.get_pointer();
            m_List.erase(core->listItr);
        }
    }

    virtual bool Find (const ObjectPtr &obj, Itr &itr)
    {
        for (GetFirst(itr); itr->IsValid(); GetNext(itr))
        {
            if (itr->Get() == obj)
            {
                return true;
            }
        }
        return false;
    }

    virtual bool IsValid (Itr &itr)
    {
        ItrCore *base = (ItrCore *)itr.get_pointer();
        return base->valid;
    }

protected:
    List    m_List;
    
};  /* class ListEnumerator */

_QGL_END


#endif  /* ifndef _object_H */

/*------------------------------ End of Module -------------------------------*/

