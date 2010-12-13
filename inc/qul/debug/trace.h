#ifndef _trace_H
#define _trace_H


#include <stdarg.h>
#include <stdio.h>

#include "trace_intf.h"

#define STR_BUF_SIZE    128
#define TRACE           g_Trace

struct Tracer
{
    TraceProc   proc;

    Tracer (TraceProc p = NULL) : proc (p)
    {
    }

    void operator() (const char *fmt, ...)
    {
        char    buf[STR_BUF_SIZE];
        va_list args;
        va_start(args, fmt);
        vsprintf(buf, fmt, args);
        va_end(args);

        proc(buf);
    }
};

extern Tracer   g_Trace;


#endif  /* ifndef _trace_H */

