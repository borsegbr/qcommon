#include "simple_optical.h"

#include "simple_scene.h"


_QGL_BEGIN

SimpleOptical::SimpleOptical ()
{
    m_SurfaceState.Set(ColorComponents(0, 0, 1));
    m_InnerSurface.SetState(&m_SurfaceState);
    m_OuterSurface.SetState(&m_SurfaceState);
    m_InnerSurface.SetEta(1.5);
    m_OuterSurface.SetEta((PhqCref)(1/1.5));
}

SimpleOptical::SimpleOptical (ColorComponentsCref clr, PhqCref s, PhqCref d, PhqCref st, PhqCref dt, 
							  PhqCref si, PhqCref di, PhqCref a, PhqCref rg, PhqCref tg, 
							  PhqCref ri, PhqCref eta, PhqCref f)
{
    Set(clr, s, d, st, dt, si, di, a, rg, tg, ri, eta, f);
}

void SimpleOptical::Set (ColorComponentsCref clr, PhqCref s, PhqCref d, PhqCref st, PhqCref dt, 
          PhqCref si, PhqCref di, PhqCref a, PhqCref rg, PhqCref tg, PhqCref ri, PhqCref eta,
          PhqCref f)
{
    m_SurfaceState.Set(clr, s, d, st, dt, si, di, a, rg, tg, ri, f);

    m_InnerSurface.SetState(&m_SurfaceState);
    m_OuterSurface.SetState(&m_SurfaceState);
    m_InnerSurface.SetEta(eta);
    m_OuterSurface.SetEta(1/eta);
}

bool SimpleOptical::Import (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
    RaytraceSurface *&pSurface)
{
    bool res = IntersectNearest(ray, intersection, k_RssOutside);
    if (res)
    {
        pSurface = &m_OuterSurface;
    }
    return res;
}

bool SimpleOptical::Export (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
    RaytraceSurface *&pSurface, RaytraceOptical *&pAdjacent)
{
    bool res = IntersectNearest(ray, intersection, k_RssInside);
    if (res)
    {
        pSurface = &m_InnerSurface;
        SimpleScene *pScene = dynamic_cast<SimpleScene *>(pRenderer->GetScene());
        pAdjacent = pScene->GetEther();
    }
    return res;
}

_QGL_END
    
