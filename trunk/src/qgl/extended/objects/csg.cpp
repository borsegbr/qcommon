#include "csg.h"

#include "simple_scene.h"


_QGL_BEGIN


const Vector3f::Dim k_AlmostOn = (Vector3f::Dim)0.01;

bool CsgRaytraceOpticalNode::Import (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
    RaytraceSurface *&pSurface)
{
    CsgRaytraceSupport::IntersectData *pData = NULL;
    bool    bEnter;
	Dim		distance;

    pData = IntersectNext(pData, ray, intersection, pSurface, distance, bEnter);
    if (pData == NULL)
    {
        return false;
    }
    if (!bEnter)
    {
		if (distance < k_AlmostOn)
		{
			pData = IntersectNext(pData, ray, intersection, pSurface, distance, bEnter);
			if (pData == NULL)
			{
				return false;
			}
			delete pData;

			return bEnter;
		}
		
        delete pData;
        return false;
    }

    if (!pSurface)
    {   // use unified
        pSurface = this->GetOuterSurface();
    }

    delete pData;
    return true;
}

bool CsgRaytraceOpticalNode::Export (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
    RaytraceSurface *&pSurface, RaytraceOptical *&pAdjacent)
{
    CsgRaytraceSupport::IntersectData *pData = NULL;
    bool    bEnter;
    Dim     distance;

    pData = IntersectNext(pData, ray, intersection, pSurface, distance, bEnter);
    if (pData == NULL)
    {
        return false;
    }
    if (bEnter)
    {
		if (distance < k_AlmostOn)
		{
			pData = IntersectNext(pData, ray, intersection, pSurface, distance, bEnter);
			if (pData == NULL)
			{
				return false;
			}
			delete pData;
			return !bEnter;
		}

        delete pData;
        return false;
    }

    if (!pSurface)
    {   // use unified
        pSurface = this->GetInnerSurface();
    }

    SimpleScene *pScene = dynamic_cast<SimpleScene *>(pRenderer->GetScene());
    pAdjacent = pScene->GetEther();

    delete pData;
    return true;
}


struct IntersectSet
{
    bool                bEnter;
    bool                bIn;
    bool                bSelected;
    Vector3f::Dim       distance;
    Intersection        intersection;
    CsgRaytraceSupport::IntersectData     **ppData;
    CsgRaytraceSupport  *pOptical;
    RaytraceSurface     *pSurface;

    IntersectSet () : bSelected(false), ppData(NULL)
    {
    }

    operator bool ()
    {
        return bSelected;
    }

    bool IsSelected ()
    {
        return bSelected;
    }

    bool IsFinite ()
    {
        return (*ppData != NULL);
    }

    bool IsIn ()
    {
        return bIn;
    }

    void GetFirst (const Ray &ray)
    {
        if (!pOptical)
        {
            bSelected = false;
            *ppData = NULL;
            return;
        }
        *ppData = pOptical->IntersectNext(*ppData, ray, intersection, pSurface, distance, bEnter);
        bIn = !bEnter;
    }

    void GetNext (const Ray &ray)
    {
        *ppData = pOptical->IntersectNext(*ppData, ray, intersection, pSurface, distance, bEnter);
    }

    static bool Select (IntersectSet &a, IntersectSet &b)
    {
        if (!*a.ppData)
        {
            if (!*b.ppData)
            {
                return false;
            }
            else
            {
                a.bSelected = false;
                b.bSelected = true;     b.bIn = b.bEnter;
                return true;
            }
        }
        else if (!*b.ppData)
        {
            a.bSelected = true;         a.bIn = a.bEnter;
            b.bSelected = false;
            return true;
        }

        // never both at infinite
        if (a.distance < b.distance)
        {
            a.bSelected = true;         a.bIn = a.bEnter;
            b.bSelected = false;
            return true;
        }
        if (b.distance < a.distance)
        {
            a.bSelected = false;
            b.bSelected = true;         b.bIn = b.bEnter;
            return true;
        }
        a.bSelected = true;             a.bIn = a.bEnter;
        b.bSelected = true;             b.bIn = b.bEnter;
        return true;
    }
};


CsgRaytraceSupport::IntersectData * CsgRaytraceOpticalNode::IntersectNext (IntersectData *pData, 
    const Ray &ray, Intersection &intersection, RaytraceSurface *&pSurface, Dim &distance, bool &bEnter)
{
    bool bFirst = false;
    if (pData == NULL)
    {
        bFirst = true;
        pData = new CsgIntersectData();
    }

    IntersectSet    setL, setR;

    CsgIntersectData    *pCsgData = dynamic_cast<CsgIntersectData *>(pData);

    setL.ppData = &pCsgData->pL;
    setR.ppData = &pCsgData->pR;
    setL.pOptical = dynamic_cast<CsgRaytraceSupport *>(m_pL);
    setR.pOptical = dynamic_cast<CsgRaytraceSupport *>(m_pR);

    setL.GetFirst(ray);
    setR.GetFirst(ray);


    switch (m_Oper)
    {
    case k_Union:
        bEnter = setL.IsIn() || setR.IsIn();
        break;
    case k_Intersection:
        bEnter = setL.IsIn() && setR.IsIn();
        break;
    case k_Subtraction:
        bEnter = setL.IsIn() && !setR.IsIn();
        break;
    }

    if (!IntersectSet::Select(setL, setR))
    {
        delete pData;
        pData = NULL;
        return pData;
    }

    bEnter = !bEnter;

    while (1)
    {
        switch (m_Oper)
        {
        case k_Union:
            if ((setL.IsIn() || setR.IsIn()) == bEnter)
            {
                if (setL)
                {
                    pSurface = setL.pSurface;
                    intersection = setL.intersection;
                }
                else
                {
                    pSurface = setR.pSurface;
                    intersection = setR.intersection;
                }
                return pData;
            }
            if (!bEnter)
            {   /* wanna get out */
                if (!setL.IsFinite() && setL.IsIn())
                {
                    delete pData;
                    return NULL;
                }
                if (!setR.IsFinite() && setR.IsIn())
                {
                    delete pData;
                    return NULL;
                }
            }
            break;
        case k_Intersection:
            if ((setL.IsIn() && setR.IsIn()) == bEnter)
            {
                if (setL)
                {
                    pSurface = setL.pSurface;
                    intersection = setL.intersection;
                }
                else
                {
                    pSurface = setR.pSurface;
                    intersection = setR.intersection;
                }
                return pData;
            }
            if (bEnter)
            {   /* wanna get in */
                if (!setL.IsFinite() && !setL.IsIn())
                {
                    delete pData;
                    return NULL;
                }
                if (!setR.IsFinite() && !setR.IsIn())
                {
                    delete pData;
                    return NULL;
                }
            }
            break;
        case k_Subtraction:
            if ((setL.IsIn() && !setR.IsIn()) == bEnter)
            {
                if (setL)
                {
                    pSurface = setL.pSurface;
                    intersection = setL.intersection;
                }
                else
                {
                    pSurface = setR.pSurface;
                    intersection = setR.intersection;
                }
                return pData;
            }
            if (bEnter)
            {
                if (!setL.IsFinite() && !setL.IsIn())
                {
                    delete pData;
                    return NULL;
                }
                if (!setR.IsFinite() && setR.IsIn())
                {
                    delete pData;
                    return NULL;
                }
            }
            break;
        }

        if (setL)
        {
            setL.GetNext(ray);
        }
        if (setR)
        {
            setR.GetNext(ray);
        }

        if (!IntersectSet::Select(setL, setR))
        {
            delete pData;
            pData = NULL;
            return pData;
        }
    }

}

_QGL_END

