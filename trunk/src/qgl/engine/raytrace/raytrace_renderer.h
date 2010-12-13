#ifndef _raytrace_renderer_H
#define _raytrace_renderer_H

#include "renderer.h"
#include "raytrace_scene.h"


_QGL_BEGIN



class RaytraceRenderer : public Renderer
{
public:
    typedef Vector3f::Dim   Dim;

public:
    RaytraceRenderer ();
    virtual ~RaytraceRenderer ();

    virtual void Render ();

    SET_GET2(RectScreenPtr, Screen, pScreen);
    SET_GET2(RaytracScenePtr, Scene, pScene)

    SET_GET(int, TotalDepth)
    
protected:
    RaytraceScene   *m_pScene;
    RectScreen      *m_pScreen;
    int             m_TotalDepth;
    
};


_QGL_END



#endif  /* ifndef _raytrace_renderer_H */
