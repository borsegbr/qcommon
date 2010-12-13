#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "csep.h"

#define CSEP_DEBUG    0


/* STATIC */

static void csep_set_match (csep_st *csep_info)
{   // Lesser KMP
    int i;
    csep_info->next_match = (int *)realloc(csep_info->next_match, sizeof(int) * csep_info->len_sbytes);
    csep_info->next_match[0] = 0;
    csep_info->next_match[1] = 0;
    for (i = 1; i < csep_info->len_sbytes; i++)
    {
        int j;
        for (j = i - 1; j >= 1; j--)
        {
            if (memcmp(csep_info->sync_bytes, csep_info->sync_bytes + i - j, j) == 0)
            {
                csep_info->next_match[i] = j;
            }
        }
    }
}

static void csep_search (csep_st *csep_info, uchar_t **pp_cur, uchar_t *p_end)
{
    uchar_t *p = *pp_cur;
    for ( ; p < p_end && csep_info->match_phase < csep_info->len_sbytes; )
    {
        if (*p == csep_info->sync_bytes[csep_info->match_phase])
        {
            csep_info->match_phase++;
            p++;
        }
        else
        {
            if (csep_info->match_phase == 0)
            {
                p++;
            }
            else
            {
                csep_info->match_phase = csep_info->next_match[csep_info->match_phase];
            }
        }
    }
    *pp_cur = p;
}

/* API */

void csep_init (csep_st *csep_info)
{
    csep_info->next_match = NULL;
}

void csep_deinit (csep_st *csep_info)
{
    if (csep_info->next_match != NULL)
    {
        free(csep_info->next_match);
    }
}

void csep_set (csep_st *csep_info, const uchar_t *sbytes, int len_sbytes, uchar_t *buf, bool_t need_sbytes)
{
    csep_info->sync_bytes = sbytes;
    csep_info->len_sbytes = len_sbytes;
    csep_info->buffer = buf;
    csep_info->need_sbytes = need_sbytes;
    csep_reset(csep_info);
    csep_set_match(csep_info);
}

void csep_reset (csep_st *csep_info)
{
    csep_info->match_phase = 0;
    csep_info->match_phase = 0;
    csep_info->len_filled = 0;

}

bool_t csep_sync (csep_st *csep_info, uchar_t **pp_stream, uchar_t *p_end)
{
    csep_search(csep_info, pp_stream, p_end);

    if (csep_info->match_phase == csep_info->len_sbytes)
    {
        csep_info->match_phase = -1; // to start with sync bytes on get_one
        return k_true;
    }
    return k_false;
}

bool_t csep_get_one (csep_st *csep_info, uchar_t **pp_stream, uchar_t *p_end)
// at most 1 frame per call
{
    uchar_t *copy_start;
    int copy_size;
    bool_t result = k_false;
    uchar_t *p = *pp_stream;


    if (csep_info->match_phase == -1)
    {
        csep_info->match_phase = 0;
        csep_info->len_filled = 0;
    }

    // copy the sync bytes to the buffer as the beginning of the frame
    if (csep_info->need_sbytes && csep_info->len_filled == 0)
    {
        memcpy(csep_info->buffer, csep_info->sync_bytes, csep_info->len_sbytes);
        csep_info->len_filled += csep_info->len_sbytes;
    }

    // matching the end
    copy_start = p;
    csep_search(csep_info, &p, p_end);
    copy_size = p - copy_start;

    if (csep_info->match_phase == csep_info->len_sbytes)
    {
        csep_info->match_phase = -1;
        copy_size -= csep_info->len_sbytes;
        result = k_true;
    }

    if (copy_size > 0)
    {
        memcpy(csep_info->buffer + csep_info->len_filled, copy_start, copy_size);
    }
    csep_info->len_filled += copy_size;

    *pp_stream = p;
    return result;

}

#if defined(CSEP_DEBUG) && CSEP_DEBUG == 1

#define BUF_SIZE        3245
#define INTN_BUFSIZE    (4096*4)

int main (void)
{
    csep_st   csep_info;

    FILE *fp = fopen("test.dat", "rb");
    FILE *fTxt = fopen("ofs.txt", "w");
    uchar_t buffer[BUF_SIZE];
    uchar_t intn_buffer[INTN_BUFSIZE];
    uchar_t *pbuffer, *pbufend;
    uchar_t match[] = {0x00, 0x00, 0x80, 0x02};
    int     matchSize = sizeof(match) / sizeof(uchar_t);
    int     readLen = 0;
    int     count = 0;
    int     offset = 0;
    bool_t  synced = k_false;
    csep_init(&csep_info);
    csep_set(&csep_info, match, matchSize, intn_buffer, k_true);
    
    do
    {
        readLen = fread(buffer, 1, BUF_SIZE, fp);
        pbuffer = buffer;
        pbufend = buffer + readLen;

        if (readLen == 0)
        {
            break;
        }
        
        if (!synced)
        {
            if (csep_sync(&csep_info, &pbuffer, pbufend))
            {
                fprintf(fTxt, "%08d: 0x%08x\n", count, offset + (pbuffer - buffer) - matchSize);
                synced = k_true;
            }
        }
        if (synced)
        {
            while (csep_get_one(&csep_info, &pbuffer, pbufend))
            {
                char fname[32];

                //printf("grabbing frame %03d\n", count);
                
                //sprintf(fname, "frames\\frame%03d.dat", count);
                //FILE *fp = fopen(fname, "wb");
                //fwrite(csep_info.buffer, 1, csep_info.len_filled, fp);
                //fclose(fp);
                fprintf(fTxt, "%08d: 0x%08x\n", count, offset + (pbuffer - buffer) - matchSize);

                count++;
            }
        }
        offset += readLen;
    } while (readLen == BUF_SIZE);
    
    fclose(fp);
    fclose(fTxt);
    csep_deinit(&csep_info);

    return 0;
}

#endif
