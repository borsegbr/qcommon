#include "simple_scene.h"


_QGL_BEGIN


void SimpleScene::Trace (RaytraceRenderer *pRenderer, const Ray &ray, int depth, Light &light)
{
	// It is assumed that the eye is always in the air, 
	// which needs collision detection or positioning
    m_pEther->Travel(pRenderer, ray, depth, light);
}

_QGL_END

