/*
 * === Performance Bench Utility ===
 *	Unit Name:	csep 
 *	Author:		quanben
 *	Purpose:	applied to segment frames indicated by specific headers from the stream
 *              and put it in the buffer ready for processing.
 *	Comments:	Only for Windows-Intel system.
 * == History ==
 *  2007-12-28:	Created.
 *
 */


#ifndef _csep_H
#define _csep_H

/* TYPES */

typedef unsigned char   uchar_t;

typedef enum
{
    k_false = 0,
    k_true,
} bool_t;

typedef struct _csep_st
{
    bool_t  need_sbytes;

    const uchar_t *sync_bytes;
    int *next_match;
    int len_sbytes;

    int match_phase;

    uchar_t *buffer;
    int len_filled;

} csep_st;


/* API */
void csep_init (csep_st *csep_info);
void csep_deinit (csep_st *csep_info);
void csep_set (csep_st *csep_info, const uchar_t *sbytes, int len_sbytes, uchar_t *buf, bool_t need_sbytes);
void csep_reset (csep_st *csep_info);
bool_t csep_sync (csep_st *csep_info, uchar_t **pp_stream, uchar_t *p_end);
bool_t csep_get_one (csep_st *csep_info, uchar_t **pp_stream, uchar_t *p_end);

#endif  /* ifndef _csep_H */
