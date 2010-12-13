#include "raytrace_optical.h"
#include "raytrace_luminant.h"


_QGL_BEGIN


void RaytraceOptical::Travel (RaytraceRenderer *pRenderer, const Ray &ray, int depth, Light &light)
{
	if (depth >= pRenderer->GetTotalDepth())
	{
		return;
	}

    RaytraceOptical *adjacentRo = NULL;
	Intersection    intersection;
	RaytraceSurface	*pSurface = NULL;

	if (!Export(pRenderer, ray, intersection, pSurface, adjacentRo))
	{
		return;
	}

    Light 	            tempLight;
	RaytraceLuminant    *adjacentLum = dynamic_cast<RaytraceLuminant *>(adjacentRo);
	if (adjacentLum)
	{
		adjacentLum->GetLight(ray, intersection, tempLight);
		light += tempLight;
	}

	if (pSurface)
	{
        pSurface->Distribute(ray, pRenderer, intersection, this, adjacentRo, depth + 1, tempLight);
		light += tempLight;
	}

}

_QGL_END




