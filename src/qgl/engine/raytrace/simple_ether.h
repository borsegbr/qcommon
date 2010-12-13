#ifndef _simple_ether_H
#define _simple_ether_H

#include <cassert>

#include "raytrace_optical.h"


_QGL_BEGIN

class SimpleScene;

class SimpleEther : public RaytraceOptical
{
public:
    typedef Vector3f::Dim   Dim;

public:
    SimpleEther (SimpleScene *pScene = NULL);
    virtual ~SimpleEther ();

    void SetScene (SimpleScene *pScene);

    virtual bool Import (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
        RaytraceSurface *&pSurface) 
    { 
        assert(0);
        return true;    /* Never call this function which doesn't make sense. */
    }

    virtual bool Export (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
        RaytraceSurface *&pSurface, RaytraceOptical *&pAdjacent);

    virtual bool IsRayInside (const Ray &ray)
    {
        assert(0);
        return true;    /* Never call this function which doesn't make sense. */
    }

protected:
    SimpleScene *   m_pScene;
};

_QGL_END

#endif  /* ifndef _simple_ether_H */
