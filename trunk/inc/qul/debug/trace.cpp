#include "trace.h"

Tracer   g_Trace;

void SetTracer (TraceProc proc)
{
    g_Trace.proc = proc;
}


