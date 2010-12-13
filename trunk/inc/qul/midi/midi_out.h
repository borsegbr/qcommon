/**
* === MIDI Utility ===
*  Unit Name:  midi_out
*  Author:     Lincoln Yu
*  Purpose:    Midi function encapsulation for Win32 programs
* == History ==        
*  2006-01-14: Created.
* == Comments ==
*
*/

#ifndef _midi_out_H
#define _midi_out_H

#include "./midi_def.h"

#include <mmsystem.h>

#pragma comment(lib, "WINMM.LIB")

_QUL_BEGIN

class MidiOut
	{
protected:
	struct SimpleMsgr
		{
		DWORD dwMsg;

		SimpleMsgr(BYTE b = 0)
			{
			SetMsg(b);
			}
		SimpleMsgr(BYTE b1, BYTE b2)
			{
			SetMsg(b1, b2);
			}
		SimpleMsgr(BYTE b1, BYTE b2, BYTE b3)
			{
			SetMsg(b1, b2, b3);
			}

		void SetMsg(BYTE b = 0)
			{
			dwMsg = b;
			}
		void SetMsg(BYTE b1, BYTE b2)
			{
			dwMsg = b2;
			push(b1);
			}
		void SetMsg(BYTE b1, BYTE b2, BYTE b3)
			{
			dwMsg = b3;
			push(b2);
			push(b1);
			}

		void push(BYTE b)
			{
			dwMsg <<= 8;
			dwMsg |= b;
			}
		};

public:

//	elementary functions

	static UINT GetNumDevs (VOID)
		{
		return midiOutGetNumDevs();
		}

	MMRESULT Open (UINT wDeviceID = MIDIMAPPER, DWORD_PTR dwCallback = NULL, 
		DWORD_PTR dwInstance = NULL, DWORD fdwOpen = 0)
		{
		return midiOutOpen(&m_hMidiOut, wDeviceID, dwCallback, dwInstance, fdwOpen);
		}

	MMRESULT Close ()
		{
		return midiOutClose(m_hMidiOut);
		}

	DWORD Message (UINT uMsg, DWORD dw1, DWORD dw2)
		{
		return midiOutMessage(m_hMidiOut, uMsg, dw1, dw2);
		}

	MMRESULT ShortMsg (IN DWORD dwMsg)
		{
		return midiOutShortMsg(m_hMidiOut, dwMsg);
		}

	MMRESULT LongMsg (IN LPMIDIHDR pmh, IN UINT cbmh)
		{
		return midiOutLongMsg(m_hMidiOut, pmh, cbmh);
		}

	MMRESULT Reset ()
		{
		return midiOutReset(m_hMidiOut);
		}

	MMRESULT PrepareHeader(IN OUT LPMIDIHDR pmh, IN UINT cbmh)
		{
		return midiOutPrepareHeader(m_hMidiOut, pmh, cbmh);
		}

	MMRESULT UnprepareHeader(IN OUT LPMIDIHDR pmh, IN UINT cbmh)
		{
		return midiOutUnprepareHeader(m_hMidiOut, pmh, cbmh);
		}

	MMRESULT GetID (OUT LPUINT puDeviceID)
		{
		return midiOutGetID(m_hMidiOut, puDeviceID);
		}

	MMRESULT CachePatches (IN UINT uBank, OUT LPWORD pwpa, IN UINT fuCache)
		{
		return midiOutCachePatches(m_hMidiOut, uBank, pwpa, fuCache);
		}

	MMRESULT CacheDrumPatches (IN UINT uPatch, OUT LPWORD pwkya, IN UINT fuCache)
		{
		return midiOutCacheDrumPatches(m_hMidiOut, uPatch, pwkya, fuCache);
		}

	MMRESULT GetVolume (OUT LPDWORD pdwVolume)
		{
		return midiOutGetVolume(m_hMidiOut, pdwVolume);
		}

	MMRESULT SetVolume (IN DWORD dwVolume)
		{
		return midiOutSetVolume(m_hMidiOut, dwVolume);
		}

//	useful

	void NoteOn (BYTE channel, BYTE key, BYTE velocity)
		{
		SimpleMsgr msgr( 0x90 | channel, key, velocity );
		m_LastErr = ShortMsg(msgr.dwMsg);
		}

	void NoteOff (BYTE channel, BYTE key, BYTE velocity)
		{
		SimpleMsgr msgr( 0x80 | channel, key, velocity );
		m_LastErr = ShortMsg(msgr.dwMsg);
		}

	void ProgramChange (BYTE channel, MidiInstrument instrument)
		{
		SimpleMsgr msgr( 0xC0 | channel, instrument );
		m_LastErr = ShortMsg(msgr.dwMsg);
		}

	const MMRESULT & GetLastError () const
		{
		return m_LastErr;
		}

protected:
	HMIDIOUT	m_hMidiOut;
	MMRESULT	m_LastErr;

	};	// class MidiOut

_QUL_END

#endif	// ifndef _midi_out_H