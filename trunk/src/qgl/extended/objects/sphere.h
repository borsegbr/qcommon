/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : sphere.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-05-31  quanben     Created.
 *
 ******************************************************************************/

#ifndef _sphere_H
#define _sphere_H

/*--------------------------------- Includes ---------------------------------*/

#include "shape.h"
#include "simple_optical.h"
#include "raytrace_luminant.h"
#include "wireframe.h"
#include "csg.h"

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*--------------------------------- Classes ----------------------------------*/

class Sphere : public Primitive
{
public:
    typedef Vector3f::Dim       Dim;
    typedef Vector3f::DimRef    DimRef;
    typedef Vector3f::DimCref   DimCref;

public:
    friend class SphericWfDrawer;

public:
    Sphere () 
        : Primitive(), m_Center(), m_Radius(3)
    {
    }

    Sphere (DimCref cx, DimCref cy, DimCref cz, DimCref radius)
        : m_Center(cx, cy, cz), m_Radius(radius)
    {
    }

    virtual ~Sphere ()
    {
    }

	bool IsInside (const Vector3f &point) const;

protected:
    Vector3f    m_Center;
    Dim         m_Radius;

};      /* class Sphere */

class WireframedSphere : virtual public Sphere, public WireframedShape
{
public:
    typedef Vector3f::Dim       Dim;

public:
    WireframedSphere ()
        : m_NumHalfWoofs(7), m_NumLongitudes(18)
    {   // do nothing about [:Sphere:] to make the possible multiple inheritance work
    }

    void SetNumHalfWoofs (int n)
    {
        m_NumHalfWoofs = n;
    }

    void SetNumLongitudes (int n)
    {
        m_NumLongitudes = n;
    }

    virtual void DrawWireframe (Screen *pScreen, WireframeStyle *pStyle /* = NULL */);

protected:
    int m_NumHalfWoofs;
    int m_NumLongitudes;

};

class SimpleOpticalSphere : virtual public Sphere, public SimpleOptical
{
public:
    SimpleOpticalSphere ();
    SimpleOpticalSphere (DimCref cx, DimCref cy, DimCref cz, DimCref radius);

    virtual ~SimpleOpticalSphere () {}

    virtual bool IntersectNearest (const Ray &ray, Intersection &intersection, RayStartSpec rss = k_RssUnkown);
    virtual bool IsRayInside (const Ray &ray);

protected:
    bool IntersectEnter (const Ray &ray, Intersection &intersection);
    bool IntersectLeave (const Ray &ray, Intersection &intersection);
};

class RaytraceLuminantSphere : virtual public Sphere, public RaytraceLuminant
{
public:
    virtual void GetLight (const Ray &ray, const Intersection &intersection, Light &light)
    {
        // TODO:
    }
};

class UsefulSphere : public SimpleOpticalSphere, public WireframedSphere
{
public:
    UsefulSphere (DimCref cx, DimCref cy, DimCref cz, DimCref radius) 
        : Sphere(cx, cy, cz, radius)    
        /* Initialize the [:Sphere:] by calling the constructor of itself to obtain 
         * expected behavior of multi-inheritance
         */
    {
    }

    UsefulSphere ()
    {
    }
};

class CsgSphere : public CsgRaytraceLeaf, public UsefulSphere
{
public:
    typedef Vector3f::Dim                       Dim;
    typedef CsgRaytraceSupport::IntersectData   BaseIntersectData;

    struct IntersectData : public BaseIntersectData
    {
        Intersection    ints2;
        bool            have2;
    };

public:
    CsgSphere (DimCref cx, DimCref cy, DimCref cz, DimCref radius)
        : Sphere(cx, cy, cz, radius)
    {
    }

    virtual BaseIntersectData * IntersectNext (BaseIntersectData *pData, const Ray &ray, 
        Intersection &intersection, RaytraceSurface *&pSurface, Dim &distance, bool &bEnter);

protected:    
    bool IntersectBoth (const Ray &ray, Intersection &intsA, Intersection &intsB);

};


_QGL_END

#endif  /* ifndef _sphere_H */

/*------------------------------ End of Module -------------------------------*/

