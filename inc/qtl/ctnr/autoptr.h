/**
 * === Containers Library ===
 *  Unit Name:  autoptr
 *  Author:     Lincoln Yu
 *  Purpose:    implementation of auto pointer, a reference counting container
 * == History ==        
 *  2005-11-06: Created.
 *  2005-11-08: Some improvements on [:newly_creation:].
 *  2005-12-06: Some toolbox function may cause confusion are removed, e.g. [:newly_creation:].
 *  2005-12-08: Upgraded to target-free style, older version is reserved as unit autoptr_old.
 *  2006-05-01: Implicit friend class declaration is not allowed by GNU compiler. So conditional
 *              compilation is performed.
 *              Wrote a more eligible equal operator overloading.
 *  2007-07-26: Compatible pointer conversion with reinterpret cast.
 *  2007-12-09: Use _OtherAutoPtr instead of autoptr<_OtherTgt> as template parameter.
 *  2008-10-18: Pointer conversion uses default conversion, untested
 *  2008-10-19: Add not-equal operator `!=', and not operator `!' to autoptr, untested.
 *  2008-10-20: noticing that operator*() doesn't change the content of the autoptr object, i 
 *              removed the unnecessary version returning a const reference.
 *  2008-10-22: _detach() doens't set the pointer pair to zero. Some methods may make errors with
 *              this point neglected.
 *  2008-10-22: Add method get_pointer() for explicitly obtaining the pointer to the object data.
 * == Comments ==
 *
 */

#ifndef _QTL_autoptr_H
#define _QTL_autoptr_H

#include "../all/predef.h"


_QTL_BEGIN

// class autoptr

template <class _Target, class _Count = int>
        class autoptr 
    {
private:
    typedef autoptr<_Target>	_Myt;
    
    #if defined(_MSC_VER)
        friend class autoptr;
    #endif

protected:
    typedef _Target		Target;
    typedef _Count		Count;

    typedef Target *	TargetPtr;
    typedef Count *		CountPtr;
    
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
        if (_M_pCount != NULL)
			{
			--*_M_pCount;
			if (*_M_pCount <= 0)
			    {
			    delete _M_pTarget;
			    delete _M_pCount;
			    }
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
            : _M_pTarget(__copy._get_pTarget()), _M_pCount(__copy._get_pCount())
        {
        _attach();
        }

    /* bind to target pointer to the underlying object, always set the counter to 1 */
    autoptr (TargetPtr __pTarget) 
        {
        _M_pTarget = __pTarget;
        if (_M_pTarget) /* value check */
            {
            _M_pCount  = new Count(1);
            if (!_M_pCount)
                {
                delete _M_pTarget;
                _M_pTarget = NULL;
                }
            }
        else
            {
            _M_pCount = NULL;
            }
        }

    /* assign according the underlying object, allocate the space to hold the object data 
     * always set the counter to 1 */
    autoptr (const Target &__target)
        : _M_pTarget(new Target(__target)), _M_pCount(new Count(1))
        {
        }

    ~autoptr ()
        {
        _detach();
        }

    _Myt &operator= ( TargetPtr __pTarget )
        {
        _detach();
        _M_pTarget = __pTarget;
        if (__pTarget)  /* value check */
            {
            _M_pCount  = new Count(1);
            if (!_M_pCount)
                {
                delete _M_pTarget;
                _M_pTarget = NULL;
                }
            }
        else
            {
            _M_pCount = NULL;
            }
        return (*this);
        }

    _Myt &operator= ( const Target &__target )
        {
        return this->operator=(new Target(__target));
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
    bool operator!= ( const _Myt &__right ) const
        {
        return !(this->operator==(__right));
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
    template < typename _OtherTgt >
        bool operator!= ( const autoptr<_OtherTgt> &__right ) const
        {
        return !(this->operator==<_OtherTgt>(__right));
        }

    template <typename _Other>
        bool operator== ( const _Other &__right ) const
        {
        return ( _M_pTarget == __right._get_pTarget() );
        }
    template <typename _Other>
        bool operator!= ( const _Other &__right ) const
        {
        return !(this->operator==<_Other>(__right));
        }

    operator bool () const
        {
        return ( _M_pTarget != NULL );
        }

    bool operator! () const
        {
        return (_M_pTarget == NULL);
        }
    
    TargetPtr operator-> () const
        {
        return ( _M_pTarget );
        }

    Target & operator* () const
        {
        /* !!! no pointer checking */
        return ( *_M_pTarget );
        }

    template <class _PointerType>
        operator _PointerType () const
        {
        return (_PointerType)(_M_pTarget);
        }

    TargetPtr get_pointer() const
        {
        return _M_pTarget;
        }

private:
    TargetPtr	_M_pTarget;     // pointer to target
    CountPtr	_M_pCount;      // pointer to reference counter

    };  // class autoptr

_QTL_END

#endif  // ifndef _QTL_autoptr_H
