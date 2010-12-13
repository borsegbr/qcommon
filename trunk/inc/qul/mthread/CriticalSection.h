/*
* === Thread Utility ===
*	Unit Name:	CriticalSection
*	Author:		quanben
*	Purtage:	Critical section for thread synchronization
* == History ==
*  2006-01-04:	Created.
*
*/

#ifndef _CriticalSection_H
#define _CriticalSection_H

#include "../all/predef.h"

#include <windows.h>

_QUL_BEGIN

class CriticalSection
	{
public:
	CriticalSection()
		{
		::InitializeCriticalSection(&m_CriticalSection);
		}

#if (_WIN32_WINNT >= 0x0403)
	CriticalSection(DWORD dwSpinCount)
		{
		::InitializeCriticalSectionAndSpinCount(&m_CriticalSection, dwSpinCount);
		}
#endif

	~CriticalSection()
		{
		::DeleteCriticalSection(&m_CriticalSection);
		}

	void Enter()
		{
		::EnterCriticalSection(&m_CriticalSection);
		}

	void Leave()
		{
		::LeaveCriticalSection(&m_CriticalSection);
		}

private:
	CRITICAL_SECTION	m_CriticalSection;

	};

_QUL_END

#endif	// ifndef _CriticalSection_H
