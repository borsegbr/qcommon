/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : raytrace_surface.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-09-02  quanben     Created.
 *
 ******************************************************************************/

#ifndef _raytrace_surface_H
#define _raytrace_surface_H

/*--------------------------------- Includes ---------------------------------*/

#include "ray.h"
#include "raytrace_renderer.h"

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*--------------------------------- Classes ----------------------------------*/

class RaytraceOptical;

class RaytraceSurface
{
public:
    /* ray distribution */
    virtual void Distribute (const Ray &ray, RaytraceRenderer *pRenderer, 
    	const Intersection &intersection, RaytraceOptical *obj1, RaytraceOptical *obj2, 
    	int depth, Light &light) = 0;

};

_QGL_END

#endif  /* ifndef _raytrace_surface_H */

/*------------------------------ End of Module -------------------------------*/

