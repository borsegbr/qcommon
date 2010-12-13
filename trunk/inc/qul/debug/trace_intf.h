#ifndef _trace_intf_H
#define _trace_intf_H

#ifndef EXPORT_TYPE
#define EXPORT_TYPE __declspec(dllexport)
#endif

typedef void (*TraceProc) (const char *str);

#ifdef __cplusplus
extern "C" {
#endif

void EXPORT_TYPE SetTracer (TraceProc proc);

#ifdef __cplusplus
}
#endif


#endif  /* _trace_intf_H */
