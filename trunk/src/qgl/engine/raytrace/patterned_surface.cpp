#include "patterned_surface.h"


_QGL_BEGIN


void PatternedSurface::AttachPattern (PixelColor *pPattern)
{
    m_pPattern = pPattern;
}

void PatternedSurface::SetPattern (ColorByte  *pBmp, int *colorOrder, int bpp, int width, int height)
{
    
}

void PatternedSurface::Release ()
{
    delete[] m_pPattern;
}

void PatternedSurface::Distribute (const Ray &ray, RaytraceRenderer *pRenderer, 
    const Intersection &intersection, RaytraceOptical *obj1, RaytraceOptical *obj2, 
    int depth, Light &light)
{
    if (intersection.GetIdxDir() == 0)
    {   /* from ether to optical object */
        
    }
    else
    {   /* from optical object to ether */

    }
}

_QGL_END
