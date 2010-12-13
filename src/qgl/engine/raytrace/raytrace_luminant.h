#ifndef _ray_trace_luminant_H
#define _ray_trace_luminant_H

#include "luminant.h"


_QGL_BEGIN

class RaytraceLuminant : public Luminant
{
public:
    virtual void GetLight (const Ray &ray, const Intersection &intersection, Light &light) = 0;

};

_QGL_END

#endif /* if _ray_trace_luminant_H */

