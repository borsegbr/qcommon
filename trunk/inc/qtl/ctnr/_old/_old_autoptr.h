/**
 * === Containers Library ===
 *  Unit Name:  autoptr
 *  Author:     Lincoln Yu
 *  Purpose:    implementation of auto pointer, a reference counting container
 * == History ==        
 *  2005-11-06: Created.
 *  2005-11-08: Some improvements on [:newly_creation:].
 *	2005-12-06:	Some toolbox funcitons may cause confusion are removed, e.g. [:newly_creation:].
 * == Comments ==
 *
 */

#ifndef _QTL_autoptr_H
#define _QTL_autoptr_H

#include "../all/predef.h"

_QTL_BEGIN

// class autoptr_target

class autoptr_target
	{
private:
	int	_M_ref_count;

public:
	const int &_add_ref () { return (++_M_ref_count); }
	const int &_sub_ref () { return (--_M_ref_count); }
	const int &_get_ref () { return (_M_ref_count); }

	autoptr_target () : _M_ref_count(1)
		/*
		 *	the constructor should always be called in every derived target
		 *	objects to make the autoptr counting work properly!
		 */
		{
		}	

	autoptr_target ( const autoptr_target &__copy ) : _M_ref_count(1)
		{
		}
	
	virtual ~autoptr_target() 
		{
		}

};


// class autoptr

template <typename _Target>
	class autoptr
	{
private:
	typedef autoptr<_Target>		_Myt;
	typedef autoptr_target *		AutoTgtPtr;
	typedef const autoptr_target	*AutoTgtCPtr;
	typedef _Target *				TgtPtr;
	typedef const _Target *			TgtCPtr;

	AutoTgtPtr						_M_target;		// target is dynamic

protected:

	friend class autoptr;

	/**
	 * Derived class should call these functions cautiously, esp. [:_target:] and [:_newly_attach_:]
	 */

// Atom functions
	bool _alone () const
		{
		return ( _M_target->_get_ref() == 1 );
		}

	void _attach()
		{
		if(_M_target != NULL)
			_M_target->_add_ref();
		}

	void _detach ()
		{
		if (_M_target != NULL && _M_target->_sub_ref() <= 0)
			delete _M_target;
		}

	void _target ( autoptr_target *__new_tgt )
		{
		_M_target = __new_tgt;
		}

	const AutoTgtPtr & _access () const 
		{ 
		return (_M_target); 
		}

	AutoTgtPtr & _access () 
		{ 
		return (_M_target); 
		}

// Toolbox
	void _release ()
		{
		_detach();
		_target(NULL);
		}

	void _retarget ( autoptr_target *__new_tgt )
		{
		_detach();
		_target(__new_tgt);
		}


	void _redirect ( autoptr_target *__tgt )
		{
		_detach();
		_target(__tgt);
		_attach();
		}

	void _redirect ( const _Myt &__other )
		{
		_redirect(__other._access());
		}

	template <typename _OtherTgt>
		void _redirect ( const autoptr<_OtherTgt> &__other )
		{
		_redirect(__other._access());
		}
	
public:
	autoptr () : _M_target(NULL) {}

	autoptr ( const _Myt &__copy ) : _M_target(__copy._access())
		{
		_attach();
		}

	template <typename _OtherTgt>
		autoptr ( const autoptr<_OtherTgt> &__copy ) : _M_target(__copy._access())
		{
		_attach();
		}

	autoptr (const _Target &__tgt)
		{
		_target(new _Target(__tgt));
		}

	virtual ~autoptr ()
		{
		_detach();
		}

	_Myt &operator= ( const _Target &__tgt )
		{
		_redirect(new _Target(__tgt));
		return (*this);
		}

	_Myt &operator= ( const _Myt &__right )
		{
		_redirect(__right);
		return (*this);
		}

	template <typename _OtherTgt>
		_Myt &operator= ( const autoptr<_OtherTgt> &__right )
		/*
		 *	assign other autoptr type, which is almost always compatible to this one
		 */
		{
		_redirect<_OtherTgt>(__right);
		return (*this);
		}

	template <typename _Other>
		bool operator== ( const _Other &__right ) const
		{
		return ( _access() == __right._access() );
		}

	operator bool () const
		{
		return ( _M_target != NULL );
		}
	
	TgtCPtr operator-> () const
		{
		return ( (TgtCPtr)_access() );
		}

	TgtPtr operator-> ()
		{
		return ( (TgtPtr)_access() );
		}

	const _Target & operator* () const
		{
		return ( *(TgtPtr)_access() );
		}

	_Target & operator* ()
		{
		return ( *(TgtPtr)_access() );
		}

	};	// class autoptr

_QTL_END

#endif	// ifndef _QTL_autoptr_H
