#ifndef _csg_H
#define _csg_H

#include "shape.h"
#include "simple_optical.h"
#include "wireframe.h"

_QGL_BEGIN


/*
 * Usually using CSG to represent combined shapes
 * For more on CSG (Constructive Solid Geometry), refer to the following sites:
 *   http://en.wikipedia.org/wiki/Constructive_solid_geometry
 *   http://www.povray.org/documentation/view/3.6.1/302/
 * 
 */

class CsgShape : public Combined
{
public:
    CsgShape () {}
    virtual ~CsgShape () {}

};

class CsgNode : virtual public CsgShape
{
public:
    enum Operation 
    {
        k_Union,
        k_Intersection,
        k_Subtraction,
    };

    CsgNode () : m_pL(NULL), m_pR(NULL), m_Oper(k_Union) {}
    CsgNode (CsgShape *pL, CsgShape *pR, Operation oper = k_Union) : m_pL(pL), m_pR(pR), m_Oper(oper) {}
    virtual ~CsgNode () {}

    void SetUnderling (CsgShape *pL, CsgShape *pR, Operation oper = k_Union)
    {
        m_pL = pL;
        m_pR = pR;
        m_Oper = oper;
    }

protected:
    CsgShape *  m_pL;
    CsgShape *  m_pR;

    Operation   m_Oper;
};

class CsgLeaf : virtual public CsgShape
{
public:
    CsgLeaf () {}
    virtual ~CsgLeaf () {}
};

class CsgRaytraceSupport : virtual public CsgShape
{
public:
    typedef Vector3f::Dim   Dim;

    class IntersectData
    {
    public:
        IntersectData () {}
        virtual ~IntersectData () {}
    };

public:
    virtual IntersectData * IntersectNext (IntersectData *pData, const Ray &ray, 
        Intersection &intersection, RaytraceSurface *&pSurface, Dim &distance, bool &bEnter) = 0;

};

class CsgRaytraceOpticalNode : public CsgNode, public CsgRaytraceSupport, public RaytraceOptical
{
protected:
    class CsgIntersectData : public IntersectData
    {
    public:
        IntersectData   *pL;
        IntersectData   *pR;

    public:
        CsgIntersectData () : pL(NULL), pR(NULL)
        {
        }

        virtual ~CsgIntersectData ()
        {
            if (pL)
            {
                delete pL;
            }
            if (pR)
            {
                delete pR;
            }
        }
    };

public:
	CsgRaytraceOpticalNode ()
	{
	}

	CsgRaytraceOpticalNode (CsgShape *pL, CsgShape *pR, Operation oper = k_Union)
		: CsgNode(pL, pR, oper)
	{
	}

    virtual bool Import (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
        RaytraceSurface *&pSurface);
    virtual bool Export (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
        RaytraceSurface *&pSurface, RaytraceOptical *&pAdjacent);

    virtual IntersectData * IntersectNext (IntersectData *pData, const Ray &ray, 
        Intersection &intersection, RaytraceSurface *&pSurface, Dim &distance, bool &bEnter);

    virtual RaytraceSurface * GetInnerSurface () { return NULL; }
    virtual RaytraceSurface * GetOuterSurface () { return NULL; }

};

class CsgRaytraceLeaf : public CsgLeaf, public CsgRaytraceSupport
{
public:
    CsgRaytraceLeaf () {}
    virtual ~CsgRaytraceLeaf () {}
};



class CsgUsefulNode : public CsgRaytraceOpticalNode, public WireframedShape
{
public:
    CsgUsefulNode () {}
	CsgUsefulNode (CsgShape *pL, CsgShape *pR, Operation oper = k_Union)
		: CsgRaytraceOpticalNode(pL, pR, oper)
	{
	}

    virtual ~CsgUsefulNode () {}

    virtual void DrawWireframe (Screen *pScreen, WireframeStyle *pStyle /* = NULL */)
    {
        WireframedShape *pWs = dynamic_cast<WireframedShape *>(m_pL);
        if (pWs)
        {
            pWs->DrawWireframe(pScreen, pStyle);
        }
        pWs = dynamic_cast<WireframedShape *>(m_pR);
		if (pWs)
		{
			pWs->DrawWireframe(pScreen, pStyle);
		}
    }
};


_QGL_END

#endif /* ifndef _csg_H */
