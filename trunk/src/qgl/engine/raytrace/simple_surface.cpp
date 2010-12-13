/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : simple_surface.cpp
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 * References   : 01. Cambridge - 3D Computer Graphics - A Mathematical 
 *                    Introduction with OpenGL - 2005
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-09-19  quanben     Created.
 *
 ******************************************************************************/

/*--------------------------------- Includes ---------------------------------*/

#include "simple_surface.h"

#include "raytrace_optical.h"
#include "simple_scene.h"

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN


/*---------------------------------- Types -----------------------------------*/

/*-------------------------------- Constants ---------------------------------*/

/*-------------------------------- Variables ---------------------------------*/

/*----------------------------- Member Functions -----------------------------*/

SimpleSurface::State::State ()
{
    Set(ColorComponents(0, 0, 1));
}

SimpleSurface::State::State (ColorComponentsCref clr, OpticType opticType /* = k_Opaque */)
{
    Set(clr, opticType);
}

SimpleSurface::State::State (ColorComponentsCref clr, PhqCref s, PhqCref d, PhqCref st, 
                             PhqCref dt, PhqCref si, PhqCref di, PhqCref a, 
                             PhqCref rg, PhqCref tg, PhqCref ri, PhqCref f)
{
	Set(clr, s, d, st, dt, si, di, a, rg, tg, ri, f);
}

SimpleSurface::State::State (ColorComponentsCref s, ColorComponentsCref d, ColorComponentsCref st, 
                             ColorComponentsCref dt, ColorComponentsCref si, ColorComponentsCref di, 
                             ColorComponentsCref a, ColorComponentsCref rg, ColorComponentsCref tg, 
                             ColorComponentsCref ri, PhqCref f)
{
	Set(s, d, st, dt, si, di, a, rg, tg, ri, f);
}

void SimpleSurface::State::Set (ColorComponentsCref clr, OpticType opticType /* = k_Opaque */)
{
    switch (opticType)
    {
    case k_Opaque:
        Set(clr, (PhqCref)0.95, (PhqCref)0.20, (PhqCref)0.0, (PhqCref)0.0, (PhqCref)0.95, 
            (PhqCref)0.20, (PhqCref)0.05, (PhqCref)0.95, (PhqCref)0.0, (PhqCref)0.95, (PhqCref)50);
        break;
    case k_Transparent:
        Set(clr, (PhqCref)0.20, (PhqCref)0.04, (PhqCref)0.75, (PhqCref)0.16, (PhqCref)0.95, 
            (PhqCref)0.20, (PhqCref)0.05, (PhqCref)0.20, (PhqCref)0.75, (PhqCref)0.95, (PhqCref)50);
        break;
    }
}

void SimpleSurface::State::Set (ColorComponentsCref clr, PhqCref s, PhqCref d, PhqCref st, 
                                PhqCref dt, PhqCref si, PhqCref di, PhqCref a, 
                                PhqCref rg, PhqCref tg, PhqCref ri, PhqCref f)
{
    m_Rou_s = clr;  m_Rou_s *= s;
    m_Rou_d = clr;  m_Rou_d *= d;
    m_Rou_st = clr; m_Rou_st *= st;
    m_Rou_dt = clr; m_Rou_dt *= dt;
    m_Rou_si = clr; m_Rou_si *= si;
    m_Rou_di = clr; m_Rou_di *= di;
    m_Rou_a = clr;  m_Rou_di *= a;
    m_Rou_rg = clr; m_Rou_rg *= rg;
    m_Rou_tg = clr; m_Rou_tg *= tg;
    m_Rou_ri = clr; m_Rou_ri *= ri;
    m_f = f;
}

void SimpleSurface::State::Set (ColorComponentsCref s, ColorComponentsCref d, ColorComponentsCref st, 
                                ColorComponentsCref dt, ColorComponentsCref si, ColorComponentsCref di, 
                                ColorComponentsCref a, ColorComponentsCref rg, ColorComponentsCref tg, 
                                ColorComponentsCref ri, PhqCref f)
{
	m_Rou_s = s;
	m_Rou_d = d;
	m_Rou_st = st;
	m_Rou_dt = dt;
	m_Rou_si = si;
	m_Rou_di = di;
	m_Rou_a = a;
	m_Rou_rg = rg;
	m_Rou_tg = tg;
	m_Rou_ri = ri;
    m_f = f;
}

void SimpleSurface::State::Validate ()
{
    m_Rou_rg = m_Rou_d;     m_Rou_rg += m_Rou_s;
    m_Rou_tg = m_Rou_dt;    m_Rou_tg += m_Rou_st;
    m_Rou_ri = m_Rou_di;    m_Rou_ri += m_Rou_si;
}

SimpleSurface::SimpleSurface (State *pState) : m_pState(pState)
{
}

void SimpleSurface::SetState (State *pState)
{
	m_pState = pState;
}

void SimpleSurface::SetEta (PhqCref eta)
{
    m_Eta = eta;
}

void SimpleSurface::Distribute (const Ray &ray, RaytraceRenderer *pRenderer, 
	const Intersection &intersection, RaytraceOptical *pObj1, RaytraceOptical *pObj2, 
	int depth, Light &light)
{

	Direction dirIn_u = ray.GetDirection();
    dirIn_u.Unitize();
	CountPointLight(dirIn_u, pRenderer, intersection, pObj1, pObj2, light);

	Ray rayOut;
	rayOut.SetSource(intersection.GetPosition());
	Direction dirOut;
	Light lightRefl, lightRefr;

	/* tracing reflect */
	CalcReflect(intersection, dirIn_u, dirOut);
	rayOut.SetDirection(dirOut);
	pObj1->Travel(pRenderer, rayOut, depth, lightRefl);

	/* tracing refract */
    if (m_pState->m_Rou_tg == ColorComponents(0))
    {   /* Opaque */
        lightRefl *= m_pState->m_Rou_rg;
    }
	else if (CalcRefract(intersection, m_Eta, dirIn_u, dirOut))
	{
		rayOut.SetDirection(dirOut);
		pObj2->Travel(pRenderer, rayOut, depth, lightRefr);
		lightRefr *= m_pState->m_Rou_tg;
		light += lightRefr;

		lightRefl *= m_pState->m_Rou_rg;
	}
	else
	{	/* internal reflect */
		lightRefl *= m_pState->m_Rou_ri;
	}

	light += lightRefl;
	
}


void SimpleSurface::CountPointLight (const Direction &dirIn_u, RaytraceRenderer *pRenderer, 
	const Intersection &intersection, RaytraceOptical *pObj1, RaytraceOptical *pObj2, Light &light)
{
	Ray rayToLight;
	rayToLight.SetSource(intersection.GetPosition());

	RaytraceSurface *pDummySurface;
	RaytraceOptical *pDummyOptical;


	SimpleScene *pScene = dynamic_cast<SimpleScene *>(pRenderer->GetScene());

	// assert(pScene);
    // assert: [:dirIn_u:] is unitized

	/* enumerate point light */
    ListEnumerator &pointLights = pScene->GetPointLights();
    
	Itr itr;
    for (pointLights.GetFirst(itr); itr->IsValid(); pointLights.GetNext(itr))
	{
        PointLight *pointLight = itr->GetDynPtr<PointLight *>();
		Direction   dirToLight(pointLight->GetPosition());
		dirToLight -= intersection.GetPosition();

		const Dim sqrDistIntfToLight = dirToLight.GetSquaredLength_Nn();

		/* Dot product of normal and direction for ray in is negative in this situation of entering */
		Dim dotOut;
		dirToLight.Dot(intersection.GetNormal(), dotOut);

		/* directions */
		Direction   dirTemp;
		Direction   dirFromLight = -dirToLight;

		/* intersection test */
		Intersection    intsTest;

		/* physical */
		ColorComponents    degreeDr(0), degreeSr(0);
		Phq         phqTemp;

		bool        lighted = false;

        dirFromLight.Unitize();
        dirToLight.Unitize();
        rayToLight.SetDirection(dirToLight);
		if (dotOut >= 0)
		{	/* light on the same side */
            lighted = !pObj1->Export(pRenderer, rayToLight, intsTest, pDummySurface, pDummyOptical);
			if (!lighted)
			{
				Dim sqrDistIntfToOut = Vector3f::GetSquaredDistance_Nn(rayToLight.GetSource(), intsTest.GetPosition());
				lighted = sqrDistIntfToOut >= sqrDistIntfToLight;   /* if the irradiation is immediate */
			}
			if (lighted)
			{
				ColorComponents rou_d = m_pState->m_Rou_d, rou_s = m_pState->m_Rou_s;

                bool bInRefl = (bool)m_pState->m_Rou_dt || (bool)m_pState->m_Rou_st;  // test whether transparent
                bInRefl = bInRefl && m_Eta > Phq(1) && !CalcRefract(intersection, m_Eta, dirFromLight, dirTemp);  /* internal reflection test */
                if (bInRefl)
                {   /* internal reflection */
                    rou_d = m_pState->m_Rou_di;
                    rou_s = m_pState->m_Rou_si;
                }
                else
                {   
                    rou_d = m_pState->m_Rou_d;
                    rou_s = m_pState->m_Rou_s;
                }          


				/* local diffusive reflect */
				dirToLight.Dot_Nn(intersection.GetNormal(), phqTemp);
				degreeDr = phqTemp;
				degreeDr *= rou_d;

				/* local specular reflect */
				Direction dirEyeRefl;
				CalcReflect(intersection, dirIn_u, dirTemp);  /* get specularly reflected eye ray */
				dirToLight.Dot_Nn(dirTemp, phqTemp);
                if (phqTemp > 0)
                {
                    phqTemp = pow(phqTemp, m_pState->m_f);
                    degreeSr = phqTemp;
                    degreeSr *= rou_s;
                }
                else
                {
                    degreeSr = 0;
                }
			}
		}
		else if ((bool)m_pState->m_Rou_dt || (bool)m_pState->m_Rou_st)
		{   /* light on the other side */
			Phq  n21 = Phq(1) / m_Eta;
#if USE_REF_01
            bool bRefr = n21 <= Phq(1) || CalcRefract(intersection, n21, dirToLight, dirTemp);
#else
            bool bRefr = n21 <= Phq(1);
            bool bTemp = CalcRefract(intersection, n21, dirToLight, dirTemp);
            bRefr = bRefr || bTemp;
#endif
			if (bRefr)
			{
				lighted = !pObj2->Export(pRenderer, rayToLight, intsTest, pDummySurface, pDummyOptical);
				if (!lighted)
				{
					Dim sqrDistIntfToOut = Vector3f::GetSquaredDistance_Nn(rayToLight.GetSource(), intsTest.GetPosition());
					lighted = sqrDistIntfToOut >= sqrDistIntfToLight;   /* if the irradiation is immediate */
				}
				if (lighted)
				{
					/* local diffuse refract */
					dirFromLight.Dot_Nn(intersection.GetNormal(), phqTemp);
					degreeDr = phqTemp;
					degreeDr *= m_pState->m_Rou_dt;

					/* local specular refract */
#if USE_REF_01
                    /* This is same as what is specified in Ref.01 */
					CalcRefract(intersection, m_Eta, dirIn_u, dirTemp); /* get specularly refracted eye ray */
					dirToLight.Dot_Nn(dirTemp, phqTemp);
#else
                    /* this may reduce the refraction calculation by making use of the result already given */
                    dirIn_u.Dot_Nn(dirTemp, phqTemp);
#endif
                    if (phqTemp > 0)
                    {
                        phqTemp = pow(phqTemp, m_pState->m_f);
                        degreeSr = phqTemp;
                        degreeSr *= m_pState->m_Rou_st;
                    }
                    else
                    {
                        degreeSr = 0;
                    }
				}
			}
		}

		if (lighted)
		{
		    Light   light_s, light_d;

            /* get light from source */ 
			light_s = pointLight->GetLight_s();   
			light_d = pointLight->GetLight_d();   

			/* by point light source */
			light_s *= degreeSr;
            light_d *= degreeDr;

			/* ambient */
			Light lightAmbient = pScene->GetAmbientLight();
			lightAmbient *= m_pState->m_Rou_a;

			/* add up */
            light += light_s;
            light += light_d;
            light += lightAmbient;
		}
	}
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
 *******************************************************************************/

void SimpleSurface::CalcReflect (const Intersection &intersection, 
	const Direction &dirIn, Direction &dirOut)
{
    Vector3f normal = intersection.GetNormal();
    Vector3f::Dim a = dirIn.Dot_Nn(normal);
    a *= 2;
    normal *= a;
    dirOut = dirIn;
    dirOut -= normal;
}

bool SimpleSurface::CalcRefract (const Intersection &intersection, PhqCref eta12, 
	const Direction &dirIn_u, Direction &dirOut_u)
{
    const Vector3f &normal = intersection.GetNormal();

    Dim negCosTheta1 = dirIn_u.Dot_Nn(normal);
    Dim sinTheta1 = sqrt(1 - negCosTheta1 * negCosTheta1);

    Vector3f normalOut = normal;
    normalOut *= negCosTheta1;

    dirOut_u = dirIn_u;
    dirOut_u -= normalOut;

    Dim sinTheta2 = sinTheta1 * eta12;
    if (sinTheta2 >= 1) /* Is it necessary to change it to occur when close to 1? */
    {
        return false;
    }
    Dim cosTheta2 = sqrt(1 - sinTheta2 * sinTheta2);
    Dim tanTheta2 = sinTheta2 / cosTheta2;

    dirOut_u.Unitize();
    dirOut_u *= tanTheta2;
    dirOut_u -= normal;

    dirOut_u.Unitize();

    return true;
}


_QGL_END

/*------------------------------ End of Module -------------------------------*/
