#include "./threadcontroller.h"

#include <process.h>


_QUL_BEGIN

ThreadController::ThreadController (void)
	{
	}

ThreadController::ThreadController (int numThread) : m_Threads(numThread)
	{
	}

ThreadController::~ThreadController (void)
	{
	CloseAll();
	}

void ThreadController::CloseAll ()
	{
	for (unsigned int i = 0; i < m_Threads.size(); ++i)
		if (m_Threads[i].m_Handle != 0)
			{
			WaitForSingleObject(m_Threads[i].m_Handle, INFINITE);
			CloseHandle(m_Threads[i].m_Handle);
			m_Threads[i].m_Handle = 0;
			}
	}

int ThreadController::GetIdle ()
	{
	for (unsigned int i = 0; i < m_Threads.size(); ++i )
		{
		if (m_Threads[i].m_Handle != 0)
			{
			DWORD res = WaitForSingleObject(m_Threads[i].m_Handle, 0);	// test its status
			if (res == WAIT_OBJECT_0)
				{
				CloseHandle(m_Threads[i].m_Handle);
				m_Threads[i].m_Handle = 0;
				return i;
				}
			}
		else
			{
			return i;
			}
		}
	return -1;
	}

void ThreadController::BeginThread (int idx, ThreadFuncPtr func, LPVOID pParam)
	{
	m_Threads[idx].m_Handle = (HANDLE)_beginthreadex(
		NULL, 
		0, 
		func, 
		pParam, 
		0, 
		&m_Threads[idx].m_Id
		);
	}

void ThreadController::SuspendThread (int idx)
	{
	::SuspendThread(m_Threads[idx].m_Handle);
	}

void ThreadController::ResumeThread (int idx)
	{
	::ResumeThread(m_Threads[idx].m_Handle);
	}

bool ThreadController::Call (ThreadFuncPtr func, LPVOID pParam)
	{
	// search for an available thread
	int idxAvailable = GetIdle();
	if (idxAvailable >= 0)
		{
		BeginThread(idxAvailable, func, pParam);
		return true;
		}
	else
		return false;
	}

void ThreadController::EndThread ()
	{
	_endthreadex(0);
	}



_QUL_END
