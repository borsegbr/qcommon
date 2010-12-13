/*
 * === Performance Bench Utility ===
 *	Unit Name:	clock 
 *	Author:		quanben
 *	Purpose:	Estimate the execution time for performance evaluation.
 *	Comments:	Only for Windows-Intel system.
 * == History ==
 *  2005-12-28:	Created.
 *
 */

#ifndef _QUL_clock_H
#define _QUL_clock_H

#include <qul/all/predef.h>

#include <vector>
#include <iostream>
#include "windows.h"


_QUL_BEGIN

using namespace std;

template <class _Time>
	class Clock
	{
public:
	typedef _Time	TimeType;

	virtual void Visit (TimeType &time) = 0;
	virtual double GetTimeDiff (const TimeType &tLate, const TimeType &tEarly) const = 0;
	};	// class Clock

class TimeAPI_Clock : Clock<DWORD>
	{
	typedef Clock<DWORD>	_Mybase;
public:
	typedef _Mybase::TimeType	TimeType;

	DWORD GetTime () const;
	
	virtual void Visit (TimeType &time);
	virtual double GetTimeDiff (const TimeType &tLate, const TimeType &tEarly) const;

	};	// class TimeAPI_Clock


class RDTSC_Clock : public Clock<unsigned __int64>
	{
	typedef Clock<unsigned __int64>	_Mybase;

public:
	typedef _Mybase::TimeType	TimeType;

	TimeType GetCycleCount () const
		{
		// RDTSC the "Read Timestamp Counter" instruction 
		__asm _emit 0x0F
		__asm _emit 0x31
		}
	

	RDTSC_Clock (unsigned __int64 freq = 1000000000 /* 1G CPU period */ ) : m_MainFreq(freq) {}
	void SetMainFreq (unsigned __int64 freq) { m_MainFreq = freq; }


	virtual void Visit (TimeType &time);
	virtual double GetTimeDiff (const TimeType &tLate, const TimeType &tEarly) const;

private:
	TimeType	m_MainFreq;

	};	// class RDTSC_Clock


class PerfCounter_Clock : public Clock<LARGE_INTEGER>
	{
	typedef Clock<LARGE_INTEGER>	_Mybase;

public:
	typedef _Mybase::TimeType	TimeType;

	PerfCounter_Clock();
	
	void RefreshFrequency ();
	void GetCounter (TimeType &counter) const;
	void GetCounterLongLong (LONGLONG &quadPart) const;
	
	virtual void Visit (TimeType &time);
	virtual double GetTimeDiff (const TimeType &tLate, const TimeType &tEarly) const;

private:
	LARGE_INTEGER	m_PerfFreq;
	};	// class PerfCounter_Clock

ostream & operator<< (ostream &out, const PerfCounter_Clock::TimeType &val);


// class Clock_Observer

template <class _Clock>
	class Clock_Observer
	{
	typedef Clock_Observer<_Clock>		_Myt;
	typedef typename _Clock::TimeType	TimeType;

public:
	void AddTime (int tag);
	void Conclude (ostream &out, const char *fmt = "%0 %1 %2\n");
	void Clear ()
		{
		m_Notes.clear();
		}
	_Clock & GetClock () { return m_Clock; }

private:
	struct Note 
		{
		int			m_Tag;
		TimeType	m_Time;
		};

	_Clock			m_Clock;
	vector<Note>	m_Notes;
	};	// class Clock_Reserver


template <class _Clock>
	void Clock_Observer<_Clock>::AddTime(int tag)
	{
	Note note;
	m_Clock.Visit(note.m_Time);
	note.m_Tag = tag;
	m_Notes.push_back(note);
	}

template <class _Clock>
	void Clock_Observer<_Clock>::Conclude(ostream &out, const char *fmt)
	{
	for (int i = 0; i < m_Notes.size(); i++)
		{
		for (unsigned int j = 0; j < strlen(fmt); j++)
			{
			double timeElapsed;
			char ch = fmt[j];
			if (ch == '%')
				{
				j++;
				ch = fmt[j];
				switch(ch)
					{
					case '0':
						out << m_Notes[i].m_Tag;
						break;
					case '1':
						out << m_Notes[i].m_Time;
						break;
					case '2':
						timeElapsed = m_Clock.GetTimeDiff(m_Notes[i].m_Time, m_Notes[0].m_Time);
						out << timeElapsed;
						break;
					default:
						out << ch;
					}
				}
			else
				out << ch;
			}
		}
	}


_QUL_END

#endif	// ifndef _QUL_clock_H
