/*
 * === Sound Utility ===
 *  Unit Name:  file_mspcm 
 *  Author:     quanben
 *  Purpose:    Microsoft PCM (non-compressed WAV file) header format specifier
 * == History ==
 *  2007-01-03: Created.
 *
 */

#if !defined(_file_mspcm_H)
#define _file_mspcm_H


#include <string.h>

#include "../all/prefix.h"
#include "../all/types.h"
#include "../all/endians.h"
#include "../filefmt/filechunk.h"


_QUL_BEGIN

struct riff_hdr : public FileChunk
    {
    struct Dump
        {
        char    id[4];  // identifier string = "RIFF"
        LE32    len;    // remaining length after this header
        } dump;

    virtual UInt32 Parse (UInt8 *pBuf) 
        { 
        memcpy(&dump, pBuf, sizeof(Dump));
        return (sizeof(Dump));
        }

    virtual bool Validate ()
        {
        return (dump.id[0] == 'R' && dump.id[1] == 'I' && dump.id[2] == 'F' && dump.id[3] == 'F');
        }

    };


struct wav_id : public FileChunk
    {
    struct Dump
        {
        char    id[4];  // WAVE file identifier = 'WAVE'
        } dump;

    virtual UInt32 Parse (UInt8 *pBuf)
        {
        memcpy(&dump.id, pBuf, sizeof(char[4]));
        return (sizeof(char[4]));
        }

    virtual bool Validate ()
        {
        return (dump.id[0] == 'W' && dump.id[1] == 'A' && dump.id[2] == 'V' && dump.id[3] == 'E');
        }
    };
    

struct format_chunk : public FileChunk      // CHUNK 8-byte header
    {
#pragma pack(2) // alignment
    struct Dump
        {
        char    id[4];  // 'fmt '
        LE32    len;

        LE16    wFormatTag;
        LE16    wChannels;
        LE32    dwSamplesPerSec;
        LE32    dwAvgBytesPerSec;
        LE16    wBlockAlign;
        LE16    wBitsPerSample;
        } dump;
#pragma pack()

    virtual UInt32 Parse (UInt8 *pBuf) 
        { 
        memcpy(&dump, pBuf, sizeof(Dump));
        return (UInt32)(dump.len + 4 * 2);
        }

    virtual bool Validate ()
        {
        return (dump.id[0] == 'f' && dump.id[1] == 'm' && dump.id[2] == 't' && dump.id[3] == ' ');
        }

    };
    

/*
 * mono:        
 * stereo:      left            right
 * 3 channel:   left            right           center
 * quad:        front left      front right     rear left       real right
 * 4 channel:   left            center          right           surround
 * 6 channel:   left center     left            center          right center        right       surround
 *
 */



_QUL_END

#endif  // if !defined(_file_mspcm_H)