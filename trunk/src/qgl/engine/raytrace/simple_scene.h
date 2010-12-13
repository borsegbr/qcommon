#ifndef _simple_raytrace_scene
#define _simple_raytrace_scene

#include "raytrace_scene.h"
#include "simple_ether.h"


_QGL_BEGIN

class SimpleScene : public RaytraceScene
{
public:
    SimpleScene (SimpleEther *pEther = NULL) 
    {
        SetEther(pEther);
    }
    
    virtual ~SimpleScene () {}

    void SetEther (SimpleEther *pEther)
    {
        m_pEther = pEther;
    }

    SimpleEther * GetEther ()
    {
        return m_pEther;
    }

    ListEnumerator & GetPointLights ()
    {
        return m_PointLights;
    }

    ListEnumerator & GetOpticals ()
    {
        return m_Opticals;
    }

    void Trace (RaytraceRenderer *pRenderer, const Ray &ray, int depth, Light &light);

    SET_GET(Light, AmbientLight)
        
protected:
    ListEnumerator      m_PointLights;
    ListEnumerator      m_Opticals;
    SimpleEther         *m_pEther;

    Light               m_AmbientLight;
};

_QGL_END

#endif

