#ifndef _simple_optical_H
#define _simple_optical_H

#include "simple_surface.h"
#include "raytrace_optical.h"


_QGL_BEGIN

class SimpleOptical : virtual public RaytraceOptical
{
public:
	typedef SimpleSurface::ColorComponentsCref	ColorComponentsCref;

public:
    SimpleOptical ();
	SimpleOptical (ColorComponentsCref clr, PhqCref s, PhqCref d, PhqCref st, PhqCref dt, 
        PhqCref si, PhqCref di, PhqCref a, PhqCref rg, PhqCref tg, PhqCref ri, 
        PhqCref eta, PhqCref f = 70);

    virtual ~SimpleOptical ()
    {
    }

    void Set (ColorComponentsCref clr, PhqCref s, PhqCref d, PhqCref st, PhqCref dt, 
        PhqCref si, PhqCref di, PhqCref a, PhqCref rg, PhqCref tg, PhqCref ri, 
        PhqCref eta, PhqCref f = 70);

    virtual bool Import (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
        RaytraceSurface *&pSurface);
    virtual bool Export (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
        RaytraceSurface *&pSurface, RaytraceOptical *&pAdjacent);

    virtual bool IntersectNearest (const Ray &ray, Intersection &intersection, RayStartSpec rss = k_RssUnkown) = 0;
    virtual bool IsRayInside (const Ray &ray) = 0;


    SimpleSurface * GetInnerSurface ()
    {
        return &m_InnerSurface;
    }

    SimpleSurface * GetOuterSurface ()
    {
        return &m_OuterSurface;
    }
    
protected:
    SimpleSurface           m_InnerSurface;
    SimpleSurface           m_OuterSurface;
    SimpleSurface::State    m_SurfaceState;

};

_QGL_END

#endif  /* ifndef _simple_optical_H */

