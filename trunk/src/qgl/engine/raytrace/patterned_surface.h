#ifndef _patterned_surface_H
#define _patterned_surface_H

#include "raytrace_surface.h"
#include "screen.h"
#include "light.h"


_QGL_BEGIN

class PatternedSurface : public RaytraceSurface
{
public:
    typedef unsigned char           ColorByte;
    typedef RectScreen::PixelColor  PixelColor;

    void AttachPattern (PixelColor *pPattern);

    void SetPattern (ColorByte  *pBmp, int *colorOrder, int bpp, int width, int height);
    void Release ();

    virtual void Distribute (const Ray &ray, RaytraceRenderer *pRenderer, 
        const Intersection &intersection, RaytraceOptical *obj1, RaytraceOptical *obj2, 
        int depth, Light &light);

protected:
    PixelColor  *m_pPattern;

};

_QGL_END

#endif	/* ifndef _patterned_surface_H */
