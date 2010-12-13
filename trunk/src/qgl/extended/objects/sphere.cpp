/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : sphere.cpp
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-06-02  quanben     Created.
 *
 ******************************************************************************/

/*--------------------------------- Includes ---------------------------------*/

#include "sphere.h"

#include "screen.h"

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN


/*---------------------------------- Types -----------------------------------*/

/*-------------------------------- Constants ---------------------------------*/

/*-------------------------------- Variables ---------------------------------*/

/*----------------------------- Member Functions -----------------------------*/


/* Shpere */

bool Sphere::IsInside (const Vector3f &point) const
{
	Vector3f r = point - m_Center;
	Dim dist = r.GetLength_Nn();
	return (dist < m_Radius);
}


/* WireframedSphere */
void WireframedSphere::DrawWireframe (Screen *pScreen, WireframeStyle *pStyle /* = NULL */)
{
    SimpleWireframeStyle    *pSimpleStyle = dynamic_cast<SimpleWireframeStyle *>(pStyle);
    RectScreen::Style        *pScreenSpecStyle = pSimpleStyle->GetScreenSpecStyle();
   
    int i, j;
    int woofPolyOrder = m_NumLongitudes * 2;
    int longitudePolyOrder = (m_NumHalfWoofs + 1) * 4;
    Polygon3f   polygon;
    polygon.Resize(max(woofPolyOrder, longitudePolyOrder));

    /* Draw woofs */
    Dim angleInc = k_RealHlfPi / (Dim)m_NumHalfWoofs;
    Dim thetaInc = k_RealDblPi / (Dim)woofPolyOrder;
    polygon.SetCount(woofPolyOrder);
    for (i = 0; i < m_NumHalfWoofs; i++)
    {
        Dim woofAngle = angleInc * (Dim)i;
        Dim cr = m_Radius * cos(woofAngle);
        Dim h  = m_Radius * sin(woofAngle);

        for (j = 0; j < woofPolyOrder; j++)
        {
            Dim woofTheta = thetaInc * (Dim)j;
            Dim x = m_Center[Vector3f::k_x] + cr * cos(woofTheta);
            Dim y = m_Center[Vector3f::k_y] + cr * sin(woofTheta);
            Dim z = m_Center[Vector3f::k_z] + h;
            polygon[j].Set(x, y, z);
        }

        /* Draw */
        pScreen->ProjPolygon(pScreenSpecStyle, polygon);

        for (j = 0; j < woofPolyOrder; j++)
        {
            polygon[j][Vector3f::k_z] = m_Center[Vector3f::k_z] - h;
        }
        pScreen->ProjPolygon(pScreenSpecStyle, polygon);
    }

    /* Draw longitudes */
    angleInc = k_RealPi / (Dim)m_NumLongitudes;
    thetaInc = k_RealDblPi / (Dim)longitudePolyOrder;
    polygon.SetCount(longitudePolyOrder);
    for (i = 0; i < m_NumLongitudes; i++)
    {
        Dim longitudeAngle = angleInc * (Dim)i;
        Dim xr = m_Radius * cos(longitudeAngle);
        Dim yr = m_Radius * sin(longitudeAngle);

        for (j = 0; j < longitudePolyOrder; j++)
        {
            Dim longitudeTheta = thetaInc * (Dim)j;
            Dim cosLongitudeTheta = cos(longitudeTheta);
            Dim x = m_Center[Vector3f::k_x] + xr * cosLongitudeTheta;
            Dim y = m_Center[Vector3f::k_y] + yr * cosLongitudeTheta;
            Dim z = m_Center[Vector3f::k_z] + m_Radius * sin(longitudeTheta);
            polygon[j].Set(x, y, z);
        }

        /* Draw */
        pScreen->ProjPolygon(pScreenSpecStyle, polygon);
    }
}

/* SimpleOpticalSphere */

SimpleOpticalSphere::SimpleOpticalSphere ()
{   // do nothing about [:Sphere:] to make the possible multiple inheritance work
}

SimpleOpticalSphere::SimpleOpticalSphere (DimCref cx, DimCref cy, DimCref cz, DimCref radius)
    : Sphere(cx, cy, cz, radius)
{
}


bool SimpleOpticalSphere::IntersectNearest (const Ray &ray, Intersection &intersection, RayStartSpec rss)
{
	if (rss == k_RssUnkown)
	{
		rss = IsRayInside(ray)? k_RssInside : k_RssOutside;
	}
	switch (rss)
	{
		case k_RssInside:
			return IntersectLeave(ray, intersection);
		case k_RssOutside:
			return IntersectEnter(ray, intersection);
        default:
            return false;
	}
}

bool SimpleOpticalSphere::IsRayInside (const Ray &ray)
{
    const Dim           k_Hull = Dim(0.01);
    const Vector3f &    raySource = ray.GetSource();
    Vector3f            r = raySource - m_Center;
    Dim                 dist = r.GetLength_Nn();

    if (dist > m_Radius + k_Hull)
    {
        return false;
    }
    else
    {
        return true;
    }
#if 0

    if (dist < m_Radius - k_Hull)
    {
        return true;
    }
    else if (dist > m_Radius + k_Hull)
    {
        return false;
    }

    const Vector3f &    rayDirection = ray.GetDirection();
    Dim dot = r.Dot_Nn(rayDirection);

    if (dist < m_Radius && dot < 0)
    {
        return true;
    }
    return false;
#endif

}


bool SimpleOpticalSphere::IntersectEnter (const Ray &ray, Intersection &intersection)
{
    /* assuming the ray position and direction vectors are normalized */
    
    const Vector3f &p0 = ray.GetSource();
    Vector3f v1 = ray.GetDirection();
    
    Vector3f p = m_Center - p0;

    Dim crPv = p.Dot_Nn(v1);
    if (crPv <= 0)
    {
        return false;
    }

    Dim d = v1.GetSquaredLength_Nn();
    Dim sqrtLenV1 = sqrt(d);

    Vector3f v = v1;
    v *= (crPv / d);
    p -= v;

    d = p.GetSquaredLength_Nn();
    
    d = m_Radius * m_Radius - d;
    
    if (d < 0)
    {
        return false;
    }

    d = sqrt(d);

    v1 *= d / sqrtLenV1;
    p += v1;
    Vector3f point = m_Center;
    point -= p;

    intersection.SetPosition(point);
    intersection.SetNormal(point - m_Center);
    intersection.GetNormal().Unitize();

    return true;

}


bool SimpleOpticalSphere::IntersectLeave (const Ray &ray, Intersection &intersection)
{
    /* 
     * Assuming the ray position and direction vectors are normalized 
     * The ray start is assumed to be inside the sphere
     */
    const Vector3f &p0 = ray.GetSource();
    Vector3f v1 = ray.GetDirection();

    Vector3f p = m_Center - p0;

    Dim crPv = p.Dot_Nn(v1);
    Dim d = v1.GetSquaredLength_Nn();
    Dim sqrtLenV1 = sqrt(d);

    Vector3f v = v1;
    v *= (crPv / d);
    p -= v;

    d = p.GetSquaredLength_Nn();

    d = m_Radius * m_Radius - d;
    
    // [:d:] is always positive if the ray is actually emitted from inside

    d = sqrt(d);

    v1 *= d / sqrtLenV1;
    v1 -= p;
    Vector3f point = m_Center;
    point += v1;

    intersection.SetPosition(point);
    intersection.SetNormal(m_Center - point);
    intersection.GetNormal().Unitize();

    return true;

}


/* CsgSphere */

CsgSphere::BaseIntersectData * CsgSphere::IntersectNext (BaseIntersectData *pData, const Ray &ray, 
        Intersection &intersection, RaytraceSurface *&pSurface, Dim &distance, bool &bEnter)
{
    IntersectData   *pLocal = dynamic_cast<IntersectData *>(pData);
    if (pLocal == NULL)
    {
        if (IsRayInside(ray))
        {
            if (SimpleOpticalSphere::IntersectLeave(ray, intersection))
            {
                pLocal = new IntersectData();
                pLocal->have2 = false;
                pSurface = &this->m_InnerSurface;
                bEnter = false;
            }
            else
            {
                // fatal error, regard this case as if the ray has nothing to do with the sphere
                // and so it enter the sphere at the infintite postition.
                bEnter = true;
            }
        }
        else
        {
            Intersection    intsLeave;
            if (IntersectBoth(ray, intersection, intsLeave))
            {
                pLocal = new IntersectData();
                pLocal->ints2 = intsLeave;
                pLocal->have2 = true;
                pSurface = &this->m_OuterSurface;
            }
            bEnter = true;
        }
    }
    else
    {
        if (pLocal->have2)
        {
            intersection = pLocal->ints2;
            pLocal->have2 = false;
            pSurface = &this->m_InnerSurface;
            bEnter = false;
        }
        else
        {
            delete pLocal;
            pLocal = NULL;
            bEnter = true;
        }
    }
    if (pLocal)
    {
        distance = Vector3f::GetDistance_Nn(ray.GetSource(), intersection.GetPosition());
    }
    return pLocal;
}

bool CsgSphere::IntersectBoth (const Ray &ray, Intersection &intsA, Intersection &intsB)
{
    // The ray start is assumed to be outside the sphere

    const Vector3f &p0 = ray.GetSource();
    Vector3f v1 = ray.GetDirection();
    
    Vector3f p = m_Center - p0;

    Dim crPv = p.Dot_Nn(v1);
    if (crPv <= 0)
    {
        return false;
    }

    Dim d = v1.GetSquaredLength_Nn();
    Dim sqrtLenV1 = sqrt(d);

    Vector3f v = v1;
    v *= (crPv / d);
    p -= v;

    d = p.GetSquaredLength_Nn();
    
    d = m_Radius * m_Radius - d;
    if (d < 0)
    {
        return false;
    }

    d = sqrt(d);

    v1 *= d / sqrtLenV1;


    v = p;      // store p
    v += v1;
    Vector3f point = m_Center;
    point -= v;

    intsA.SetPosition(point);
    intsA.SetNormal(point - m_Center);
    intsA.GetNormal().Unitize();


    v1 -= p;
    point = m_Center;
    point += v1;

    intsB.SetPosition(point);
    intsB.SetNormal(m_Center - point);
    intsB.GetNormal().Unitize();

    return true;

}


/*------------------------- Static Member Functions --------------------------*/

/*******************************************************************************
 * Prototype    : 
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/

_QGL_END

/*------------------------------ End of Module -------------------------------*/

