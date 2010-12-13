#include "simple_ether.h"

#include "simple_scene.h"
#include "simple_optical.h"

_QGL_BEGIN


SimpleEther::SimpleEther (SimpleScene *pScene /* = NULL */)
{
    SetScene(pScene);
}

SimpleEther::~SimpleEther ()
{

}

void SimpleEther::SetScene (SimpleScene *pScene)
{
    m_pScene = pScene;
}

bool SimpleEther::Export (RaytraceRenderer *pRenderer, const Ray &ray, Intersection &intersection, 
 	RaytraceSurface *&pSurface, RaytraceOptical *&pAdjacent)
{
    Itr itr;
    Dim	minSqrDist = -1;
    pAdjacent = NULL;

    //SimpleScene *pScene = dynamic_cast<SimpleScene *>(pRenderer->GetScene());
    ListEnumerator &opticals = m_pScene->GetOpticals();
    
    for (opticals.GetFirst(itr); itr->IsValid(); opticals.GetNext(itr))
    {
        RaytraceOptical *test = itr->GetDynPtr<RaytraceOptical *>();
        Intersection    intsTest;
        RaytraceSurface *pSfTest;
        if (test && test->Import(pRenderer, ray, intsTest, pSfTest))
        {
            Dim sqrDist = Vector3f::GetSquaredDistance_Nn(ray.GetSource(), intsTest.GetPosition());

            if (sqrDist < minSqrDist || minSqrDist < 0)
            {
                intersection = intsTest;
                minSqrDist = sqrDist;
                pAdjacent = test;
                pSurface = pSfTest;
            }
        }
    }
    
    intersection.SetIdxDir(0);  // entering from ether's side is always taken as of index 0
    return (pAdjacent != NULL);
}

_QGL_END

