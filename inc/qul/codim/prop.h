/**
 * === Coding Improvements ===
 *  Unit Name:  props
 *  Author:     Lincoln Yu
 *  Purpose:    Gadgets for C++ property definition
 * == History ==        
 *  2006-02-15: Created. Use local static variable to hold offset data.
 *  2006-02-16: Make [:InitProperities:] run only once for each type.
 *  2006-05-01: Allocation on stack for computing offset of field was replaced
 *              by computing on the null pointer
 * == Comments ==
 *  As for [:_VIRTUAL_PROPERTY:] and [:_VIRTUAL_PROPERTY_OVERRIDE:], 
 *  property calling in the form of "objectOfDerived.SuperClass::VirtualProperty"
 *  does nothing with the virtual property implementation in the super class
 *
 */

#if !defined (_prop_H)
#define _prop_H

#if !defined (offsetof)
	#define offsetof(T, f)	((int)(&((T*)0)->f))
#endif

#define	CONCAT_DIRECT(a,b)	a##b
#define	CONCAT(a,b)			CONCAT_DIRECT(a,b)
#define	TOSTR_DIRECT(tag)	#tag
#define	TOSTR(tag)			TOSTR_DIRECT(tag)

template <class	Owner> struct Property
	{
	Owner *GetOwner(int	ofs	= -1)
		{
		static int offset =	0;
		if (ofs	!= -1) offset =	ofs;
		return (Owner*)((char*)this	- offset);
		}
	};

#define	_PROPERTIES_BEGIN(prop_owner)													\
	typedef	prop_owner _PropOwner;														\
	struct PropertyWatcher : Property<_PropOwner>										\
		{																				\
		PropertyWatcher()																\
			{																			\
			InitProperties(this);														\
			}																			\
		} PropWatcher;

#define	_PROPERTIES_END(last_prop)														\
	static void	InitProperties(Property<_PropOwner>	*pCaller)							\
		{																				\
		static bool bInited = false;													\
		if (bInited) return;															\
		bInited = true;																	\
		int offset = offsetof(_PropOwner, PropWatcher);									\
		int num = offsetof(_PropOwner, last_prop) - offsetof(_PropOwner, PropWatcher);	\
		num	/= sizeof(Property<_PropOwner>);											\
		Property<_PropOwner> *p, *pEnd = pCaller + num;									\
		for	( p	= pCaller +	1; p <=	pEnd; ++p)											\
			p->GetOwner(offset += sizeof(Property<_PropOwner>));						\
		}

#define	_PROPERTY(val_type,	prop_name, set_proc, get_proc)								\
	public:	struct CONCAT(Prop_, prop_name)	: public Property<_PropOwner>				\
		{																				\
		val_type operator =	(val_type val)												\
			{																			\
			GetOwner()->set_proc(val);													\
			return val;																	\
			}																			\
		operator val_type()																\
			{																			\
			return GetOwner()->get_proc();												\
			}																			\
		} prop_name;

#define	_VIRTUAL_PROPERTY(val_type,	prop_name, set_proc, get_proc)						\
	public:	struct CONCAT(Prop_, prop_name)	: public Property<_PropOwner>				\
		{																				\
		val_type operator =	(val_type val)												\
			{																			\
			GetOwner()->CONCAT(VrtlPropSet_, prop_name)(val);							\
			return val;																	\
			}																			\
		operator val_type()																\
			{																			\
			return GetOwner()->CONCAT(VrtlPropGet_,	prop_name)();						\
			}																			\
		} prop_name;																	\
	virtual	void CONCAT(VrtlPropSet_, prop_name)(val_type val) { set_proc(val);	}		\
	virtual	val_type CONCAT(VrtlPropGet_, prop_name)() { return	get_proc();	}

#define	_VIRTUAL_PROPERTY_OVERRIDE(val_type, prop_name,	set_proc, get_proc)				\
	virtual	void CONCAT(VrtlPropSet_, prop_name)(val_type val) { set_proc(val);	}		\
	virtual	val_type CONCAT(VrtlPropGet_, prop_name)() { return	get_proc();	}

#endif	// if !defined (_prop_H)
