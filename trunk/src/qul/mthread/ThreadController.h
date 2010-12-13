/*
* === Thread Utility ===
*	Unit Name:	ThreadController
*	Author:		quanben
*	Purpose:	Simple thread controller for Windows
*	Dependency:	[:libthreadctrl.lib:] or [:libthreadctrl_mfc.lib:]
* == History ==
*  2005-12-31:	Created.
*  2006-11-01:	Add [:SuspendThread:] and [:ResumeThread:] methods.
*
*/

#if !defined(_ThreadController_H)
#define _ThreadController_H		"200611010225"

#include <windows.h>
#include <vector>

#include <qul/all/predef.h>


_QUL_BEGIN

using namespace std;

class ThreadController
	{
private:
	struct ThreadInfo
		{
		HANDLE			m_Handle;
		unsigned int	m_Id;

		ThreadInfo() : m_Handle(0), m_Id(0)
			{
			}
		
		};

	typedef vector<ThreadInfo>	ThreadInfoVec;

public:

	typedef unsigned int __stdcall ThreadFuncPtr (void *);

	ThreadController (void);
	ThreadController (int numThread);
	~ThreadController (void);


	bool Call (ThreadFuncPtr func, void *pParam);
	void CloseAll ();

	int GetIdle ();
	void BeginThread (int idx, ThreadFuncPtr func, LPVOID pParam);
	void SuspendThread (int idx);
	void ResumeThread (int idx);
	static void EndThread ();

protected:
	ThreadInfoVec	m_Threads;

	};

_QUL_END

#endif	// ifndef _ThreadController_H
