/*
 * === MIDI Utility ===
 *	Unit Name:	bitmap 
 *	Author:		quanben
 *	Purpose:	To deal with MIDI files.
 * == History ==
 *  2006-01-29:	Created.
 *
 */

#ifndef _midi_fileio_H
#define _midi_fileio_H

#include <qul/all/endians.h>
#include <qul/stream/filechunk.h>


_QUL_BEGIN

class MidiFile : public FileChunk
	{
public:
	struct HeaderChunk : public FileChunk
		{
#pragma pack(2)	// alignment
		struct Dump
			{
			BE32	mhRiff;
			BE32	mhSize;
			BE16	mhFileFmt;
			BE16	mhNumTracks;
			BE16	mhTicksPerQNote;
			
			Dump (int fmt = 0, int numTracks = 0, int ticks = 0) 
				: mhRiff('MThd'), mhSize(sizeof(Dump)-8), 
				mhFileFmt(fmt), mhNumTracks(numTracks), mhTicksPerQNote(ticks)
				{
				}

			} dump;
#pragma pack()
		
		HeaderChunk(int fmt = 0, int numTracks = 0, int ticks = 0) : dump(fmt, numTracks, ticks)
			{
			}

		virtual UInt32 Parse (UInt8 *pBuf)
			{
			memcpy(&dump, pBuf, sizeof(Dump));
			return (8+dump.mhSize);		// skip the unexpected
			}
		
		virtual UInt32 Make (UInt8 *pBuf)
			{
			memcpy(pBuf, &dump, sizeof(Dump));
			return (sizeof(Dump));
			}

		};


	struct EventChunk : public FileChunk
		{
#pragma pack(1)
		struct Dump
			{
			BE16	eventHead;
			UInt8	command;
			UInt8	length;

			Dump(int cmd = 0, int len = 0) : eventHead(0xff), command(cmd), length(len)
				{
				}
			}	dump;
#pragma pack()

		UInt8	*chunkData;
		
		EventChunk (int cmd = 0, int len = 0) : dump(cmd, len), chunkData(NULL)
			{
			}

		virtual UInt32 Parse (UInt8 *pBuf)
			{
			memcpy(&dump, pBuf, sizeof(Dump));
			return sizeof(Dump);
			}

		virtual UInt32 Make (UInt8 *pBuf)
			{
			int sizeData = dump.length;
			memcpy(pBuf, &dump, sizeof(Dump));
			memcpy(pBuf + sizeof(Dump), chunkData, sizeData);
			return (sizeof(Dump) + sizeData);
			}

		UInt32 GetSize()
			{
			return dump.length;
			}

		};


	struct TrackChunk : public FileChunk
		{
		struct Dump
			{
			BE32	mtRiff;
			BE32	mtLength;
			Dump (int len = 0) : mtRiff('MTrk'), mtLength(len)
				{
				}

			}	dump;
		
		TrackChunk (int len = 0) : dump(len)
			{
			}

		virtual UInt32 Parse (UInt8 *pBuf)
			{
			memcpy(&dump, pBuf, sizeof(Dump));
			return (sizeof(Dump));
			}

		virtual UInt32 Make (UInt8 *pBuf)
			{
			memcpy(pBuf, &dump, sizeof(Dump));
			return (sizeof(Dump));
			}

		UInt32 GetSize()
			{
			return dump.mtLength;
			}

		};


	MidiFile() : m_HeaderChunk(), m_TrackChunk(), m_EventChunk()
		{
		
		}

	UInt32 ParseHeaderChunk (UInt8 *pBuf)
		{
		memcpy(&m_HeaderChunk, pBuf, sizeof(HeaderChunk::Dump));
		return sizeof(TrackChunk::Dump);
		}

	UInt32 ParseTrackChunk (UInt8 *pBuf)
		{
		memcpy(&m_TrackChunk, pBuf, sizeof(TrackChunk::Dump));
		return sizeof(TrackChunk::Dump);
		}

	UInt32 ParseEventChunk (UInt8 *pBuf)
		{
		memcpy(&m_EventChunk, pBuf, sizeof(EventChunk::Dump));
		return sizeof(EventChunk::Dump);
		}

	UInt32 MakeHeaderChunk (UInt8 *pBuf, int fmt = 1, int numTracks = 2, int ticks = 0x78) const
		{
		HeaderChunk headerChunk(fmt, numTracks, ticks);
		return headerChunk.Make(pBuf);
		}

	UInt32 MakeTrackChunk (UInt8 *pBuf, int size)
		{
		TrackChunk trackChunk(size);
		return trackChunk.Make(pBuf);
		}

	UInt32 MakeEvent_SetTempo (UInt8 *pBuf, int tempo = 600000)
		{
		EventChunk	eventChunk(0x51, 3);
		UInt32 uTempo = tempo;
		uTempo = EndianU32_NtoB(uTempo);
		eventChunk.chunkData = (UInt8*)&uTempo;
		eventChunk.chunkData++;
		return eventChunk.Make(pBuf);
		}

	UInt32 MakeEvent_TrackEnd (UInt8 *pBuf)
		{
		EventChunk	eventChunk(0x2f, 0);
		return eventChunk.Make(pBuf);
		}

	UInt32 MakeEvent_KeySignature (UInt8 *pBuf)
		{
		// default key, C major
		EventChunk	eventChunk(0x59, 2);
		BYTE buf[2] = {0, 0};
		eventChunk.chunkData = buf;
		return eventChunk.Make(pBuf);
		}

	UInt32 MakeEvent_TimeSignature (UInt8 *pBuf, int numerator, int denominator)
		{
		EventChunk	eventChunk(0x58, 4);
		int denomLevel = 0;
		while(denominator>1)
			{
			denominator>>=1;
			denomLevel++;
			}
		BYTE buf[4] = {numerator, denomLevel, 0x18, 0x08};
		eventChunk.chunkData = buf;
		return eventChunk.Make(pBuf);
		}

	UInt32 MakeEvent_String (UInt8 *pBuf, BYTE cmd, int length, UInt8 *pString)
		{
		EventChunk	eventChunk(cmd, length);
		eventChunk.chunkData = pString;
		return eventChunk.Make(pBuf);
		}

	UInt32 MakeEvent_NoteOn (UInt8 *pBuf, int channel, int note, int velocity)
		{
		pBuf[0] = 0x00;
		pBuf[1] = 0x90|channel;
		pBuf[2] = note;
		pBuf[3] = velocity;
		return 4;
		}

	UInt32 MakeEvent_NoteOff (UInt8 *pBuf, int channel, int note, int velocity)
		{
		pBuf[0] = 0x00;
		pBuf[1] = 0x80|channel;
		pBuf[2] = note;
		pBuf[3] = velocity;
		return 4;
		}

	UInt32 SimpleNoteStart (UInt8 *pBuf, int channel, int wait)
		{
		m_LenTimeTick = MakeTimeTick(wait, m_TimeTick);
		CopyTimeTick(m_TimeTick, pBuf, m_LenTimeTick);
		pBuf[m_LenTimeTick] = 0x90 | channel;
		return (m_LenTimeTick+1);
		}

	UInt32 SimpleNoteOn (UInt8 *pBuf, int note, int velocity, int duration)
		{
		pBuf[0] = note;
		pBuf[1] = velocity;
		m_LenTimeTick = MakeTimeTick(duration, m_TimeTick);
		CopyTimeTick(m_TimeTick, pBuf+2, m_LenTimeTick);
		return (m_LenTimeTick+2);
		}

	UInt32 SimpleNoteOff (UInt8 *pBuf, int note, int duration)
		{
		pBuf[0] = note;
		pBuf[1] = 0;
		m_LenTimeTick = MakeTimeTick(duration, m_TimeTick);
		CopyTimeTick(m_TimeTick, pBuf+2, m_LenTimeTick);
		return (m_LenTimeTick+2);
		}

	UInt32 MakeEvent_ProgramChange(UInt8 *pBuf, int channel, int program)
		{
		pBuf[0] = 0xC0|channel;
		pBuf[1] = program;
		}

private:

	static void CopyTimeTick (UInt8 *pAr, UInt8 *pDst, int len)
		{
		pAr += len-1;
		for (int i = 0; i < len; pAr--, pDst++, i++)
			*pDst = *pAr;
		}

	static UInt32 MakeTimeTick (UInt32 val, UInt8 *pAr)
		{
		int b;

		b = val & 0x7f;
		pAr[0] = b;
		val >>= 7;

		int i;
		for (i = 1; val > 0; i++, val>>=7)
			{
			b = val & 0x7f;
			b |= 0x80;
			pAr[i] = b;
			}
		return i;
		}

	static UInt32 ParseTimeTick (UInt8 *pAr, UInt32 len)
		{
		int b;
		int val = 0;

		int i;
		for ( i = len - 1; i > 0; i-- ) 
			{
			b = pAr[i];
			b -= 0x80;
			val |= b;
			val <<= 7;
			}
		b = pAr[0];
		val |= b;
		return val;
		}

private:
	HeaderChunk		m_HeaderChunk;
	TrackChunk		m_TrackChunk;
	EventChunk		m_EventChunk;

	UInt8			m_TimeTick[20];
	UInt32			m_LenTimeTick;


	};	// MidiFileIn

_QUL_END

#endif	// ifndef _midi_fileio_H