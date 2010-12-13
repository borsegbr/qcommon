/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : simple_surface.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-09-19  quanben     Created.
 *
 ******************************************************************************/

#ifndef _simple_surface_H
#define _simple_surface_H

/*--------------------------------- Includes ---------------------------------*/


#include "raytrace_surface.h"
#include "light.h"


/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*--------------------------------- Classes ----------------------------------*/

class SimpleOptical;

class SimpleSurface : public RaytraceSurface
{
public:
    typedef Vector3f::Dim       Dim;
    typedef Ray::Direction      Direction;
    typedef const ColorComponents &    ColorComponentsCref;

	struct State
	{
		/* Phong lightning model */

		/* point light splitter */
		/* theoretically (simplified), as these coefficients are used on both side,
         * they must observe the law of conversation of energy, i.e.
		 * rou_s + rou_st <= 1, rou_d + rou_dt <= 1
         * rou_rg + rou_tg <= 1
		 */

        enum OpticType
        {
            k_Opaque,
            k_Transparent,
        };

		ColorComponents	m_Rou_s;            /* specular reflection coefficient */
		ColorComponents	m_Rou_d;            /* diffuse reflection coefficient */
		ColorComponents	m_Rou_st;			/* specular refraction (transmission) coefficient */
		ColorComponents	m_Rou_dt;			/* diffuse refraction (transmission) coefficient */
		ColorComponents	m_Rou_si;			/* internal specular reflection coefficient */
		ColorComponents	m_Rou_di;			/* internal diffusive reflection coefficient */
        Phq             m_f;                /* exponent of specular sharpness */

		/* ambient */
		ColorComponents	m_Rou_a;            /* ambient reflection coefficient */

		/* ray-tracing fractions */
		ColorComponents	m_Rou_rg;			/* fraction of reflection for tracing */
		ColorComponents	m_Rou_tg;			/* fraction of transmission for tracing */
		ColorComponents	m_Rou_ri;			/* fraction of internal reflection for tracing */

        State ();

        State (ColorComponentsCref clr, OpticType opticType = k_Opaque);

		State (ColorComponentsCref clr, PhqCref s, PhqCref d, PhqCref st, PhqCref dt, 
			PhqCref si, PhqCref di, PhqCref a, PhqCref rg, PhqCref tg, PhqCref ri, PhqCref f);
		
		State (ColorComponentsCref s, ColorComponentsCref d, ColorComponentsCref st, ColorComponentsCref dt, ColorComponentsCref si, 
            ColorComponentsCref di, ColorComponentsCref a, ColorComponentsCref rg, ColorComponentsCref tg, ColorComponentsCref ri, PhqCref f);

        void Set (ColorComponentsCref clr, OpticType opticType = k_Opaque);

		void Set (ColorComponentsCref clr, PhqCref s, PhqCref d, PhqCref st, PhqCref dt, 
			PhqCref si, PhqCref di, PhqCref a, PhqCref rg, PhqCref tg, PhqCref ri, PhqCref f);

		void Set (ColorComponentsCref s, ColorComponentsCref d, ColorComponentsCref st, ColorComponentsCref dt, ColorComponentsCref si, 
            ColorComponentsCref di, ColorComponentsCref a, ColorComponentsCref rg, ColorComponentsCref tg, ColorComponentsCref ri, PhqCref f);

        void Validate ();
	};

public:
	SimpleSurface (State *pState = NULL);
    virtual ~SimpleSurface () {}

    void SetState (State *pState);
    void SetEta (PhqCref eta);

    /*
     * The normal of the intersection is required to be unitized.
     */
    virtual void Distribute (const Ray &ray, RaytraceRenderer *pRenderer, 
    	const Intersection &intersection, RaytraceOptical *obj1, RaytraceOptical *obj2, 
    	int depth, Light &light);

protected:
    virtual void CountPointLight (const Direction &dirIn_u, RaytraceRenderer *pRenderer, 
        const Intersection &intersection, RaytraceOptical *obj1, RaytraceOptical *obj2, Light &light);
    
    static void CalcReflect (const Intersection &intersection, 
	    const Direction &dirIn, Direction &dirOut);
    
    static bool CalcRefract (const Intersection &intersection, PhqCref eta12, 
	    const Direction &dirIn_u, Direction &dirOut_u);

protected:

	State	*m_pState;
    Phq     m_Eta;          /* index of refraction */

};

_QGL_END

#endif  /* ifndef _simple_surface_H */