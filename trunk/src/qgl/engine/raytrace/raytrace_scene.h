#ifndef _raytrace_scene_H
#define _raytrace_scene_H

#include "scene.h"
#include "light.h"
#include "ray.h"


_QGL_BEGIN

class RaytraceRenderer;

class RaytraceScene : public Scene
{
public:
    virtual void Trace (RaytraceRenderer *pRenderer, const Ray &ray, int depth, Light &light) = 0;
        
};

typedef RaytraceScene * RaytracScenePtr;


_QGL_END


#endif  /* ifndef _raytrace_scene_H */


