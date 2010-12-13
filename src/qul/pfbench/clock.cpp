#include "clock.h"

#pragma comment(lib, "winmm.lib")

_QUL_BEGIN

// class RDTSC_Clock

void RDTSC_Clock::Visit (TimeType &time)
	{
	time = GetCycleCount();
	}

double RDTSC_Clock::GetTimeDiff (const TimeType &tLate, const TimeType &tEarly) const
	{
	return (double)(tLate - tEarly) / m_MainFreq;
	}

// class TimeAPI_Clock

DWORD TimeAPI_Clock::GetTime() const
	{
	return timeGetTime();
	}

void TimeAPI_Clock::Visit (TimeType &time)
	{
	time = timeGetTime ();
	}

double TimeAPI_Clock::GetTimeDiff (const TimeType &tLate, const TimeType &tEarly) const
	{
	return ((double)(tLate - tEarly) / 1000.0);
	}

// class PerfCounter_Clock

PerfCounter_Clock::PerfCounter_Clock ()
	{
	RefreshFrequency();
	}

void PerfCounter_Clock::RefreshFrequency ()
	{
	QueryPerformanceFrequency(&m_PerfFreq);
	}

void PerfCounter_Clock::GetCounter (TimeType &counter) const
	{
	QueryPerformanceCounter(&counter);
	}

void PerfCounter_Clock::GetCounterLongLong (LONGLONG &counter) const
	{
	QueryPerformanceCounter((TimeType *)&counter);
	}

void PerfCounter_Clock::Visit(TimeType &time)
	{
	GetCounter(time);
	}

double PerfCounter_Clock::GetTimeDiff (const TimeType &tLate, const TimeType &tEarly) const
	{
	return ((double)(tLate.QuadPart - tEarly.QuadPart) / m_PerfFreq.QuadPart);
	}

// global definition

ostream & operator<< (ostream &out, const PerfCounter_Clock::TimeType &val)
	{
	out << val.QuadPart;
	return out;
	}

_QUL_END


