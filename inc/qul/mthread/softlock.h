/*
 * === Thread Utility ===
 *	Unit Name:	softlock
 *	Author:		quanben
 *	Purtage:	Soft lock for thread synchronizatoin
 * == History ==
 *  2005-12-31:	Created.
 *
 */

#ifndef _softlock_H
#define _softlock_H

#include "../all/predef.h"

_QUL_BEGIN

// Dekker and Peerson, 2 processes (threads)

class DPLocker 
	{
public:
	DPLocker()
		{
		m_WantedIn[0] = m_WantedIn[1] = false;
		}

	void EnterCriticalSection (int id)
		{
		int other = 1 - id;
		m_WantedIn[id] = true;
		m_Observer = id;
		while (m_Observer == id && m_WantedIn[other]) {}
		}

	void LeaveCriticalSection (int id)
		{
		m_WantedIn[id] = false;
		}
	
private:
	bool	m_WantedIn[2];
	int		m_Observer;
	};	// class DPLocker

_QUL_END

#endif	// ifndef _softlock_H