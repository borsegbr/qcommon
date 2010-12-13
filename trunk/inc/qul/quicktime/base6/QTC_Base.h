/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_Base
 *  Author:     Lincoln Yu
 *  Purpose:    Base class of Quick-time time components
 * == History ==        
 *	2006-03-05:	Created.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

#ifndef _QTC_Base_H
#define _QTC_Base_H

#include <qul/quicktime/base6/QTC_Def.h>


typedef struct _Storage
	{
	ComponentInstance	ciDelegate;
	void				*pEmbody;
	} Storage, *StoragePtr, **StorageHdl;


#ifdef __cplusplus

_QTC_BEGIN

struct BaseComponent
	{
	BaseComponent(StoragePtr pStorage) : m_pStorage(pStorage) {}
	virtual ~BaseComponent() {}

// Core Selector Range

	virtual Result Target (ComponentInstance ciTarget) { return 0; }
	virtual Result Register () { return 0; }
	virtual Result Version () { return 0; }
	// Place reserved for [:CanDo:] ...
	virtual Result Close (ComponentInstance ciSelf) { return 0; }
	virtual Result Open (ComponentInstance ciSelf) 
		{ return 0; }

protected:
	StoragePtr	m_pStorage;

	};

_QTC_END

#endif	// ifdef __cplusplus

#endif