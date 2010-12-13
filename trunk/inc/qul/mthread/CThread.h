#ifndef _CThread_H
#define _CThread_H


namespace qul {

#include <windows.h>

class CThread
{
public:
	CThread()
		: m_pThreadProc(0), m_pParam(0), m_hThread(0), m_ThreadId(0)
	{
	}

	CThread(LPTHREAD_START_ROUTINE pThreadProc, LPVOID pParam)
		: m_pThreadProc(pThreadProc), m_pParam(pParam), m_hThread(0), m_ThreadId(0)
	{
	}

	~CThread()
	{
		if (m_hThread)
		{
			TerminateThread();
		}
	}

    operator HANDLE()
    {
        return m_hThread;
    }

	void SetThread(LPTHREAD_START_ROUTINE pThreadProc, LPVOID pParam)
	{
		if (!m_hThread)
		{
			m_pThreadProc = pThreadProc;
			m_pParam = pParam;
		}
	}

	void CreateThread(DWORD dwCreateFlags = 0, UINT nStackSize = 0, 
		LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL)
	{
		if (!m_hThread)
		{
			m_hThread = ::CreateThread(lpSecurityAttrs, nStackSize, m_pThreadProc, m_pParam, dwCreateFlags, &m_ThreadId);
		}
    }

	DWORD TerminateThread()
	{
		DWORD rc = ::TerminateThread(m_hThread, 0);
		m_hThread = 0;
		return rc;
	}

	DWORD SuspendThread()
	{
		return ::SuspendThread(m_hThread);
	}

	DWORD ResumeThread()
	{
		return ::ResumeThread(m_hThread);
	}

protected:
	LPTHREAD_START_ROUTINE  m_pThreadProc;
	LPVOID			m_pParam;
	DWORD			m_ThreadId;
	HANDLE			m_hThread;

};	/* class CThread */

}	/* namespace qul */

#endif	/* ifndef _CThread_H */
