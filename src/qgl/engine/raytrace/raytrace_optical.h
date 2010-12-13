#ifndef _raytrace_optical_H
#define _raytrace_optical_H

#include "light.h"
#include "optical.h"
#include "raytrace_surface.h"


_QGL_BEGIN


class RaytraceOptical : public Optical
{
public:
    enum RayStartSpec
    {
        k_RssUnkown,
        k_RssInside,
        k_RssOutside,
    };

public:
    virtual void Travel (RaytraceRenderer *pRenderer, const Ray &ray, int depth, Light &light);
    virtual bool Import (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
            RaytraceSurface *&pSurface) = 0;
    virtual bool Export (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
            RaytraceSurface *&pSurface, RaytraceOptical *&pAdjacent) = 0;

};

_QGL_END

#endif  /* ifndef _raytrace_optical_H */

