/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : point_light.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-06-08  quanben     Created.
 *
 ******************************************************************************/

#ifndef _point_light_H
#define _point_light_H

/*--------------------------------- Includes ---------------------------------*/

#include "luminant.h"


/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*--------------------------------- Classes ----------------------------------*/



/* Although it is not a typical luminant object, we still derive it from the class 
 * Luminant for conceptual reason */

class PointLight : public Luminant  
{
public:
	typedef Vector3f::Dim	Dim;
	typedef Dim &			DimRef;
	typedef const Dim & 	DimCref;

public:	
	
    PointLight () : m_Light_d((Phq)0.3), m_Light_s((Phq)0.7)
	{
	}

    PointLight (const Vector3f &position, const Light &light_d = Light((Phq)0.3), 
        const Light &light_s = Light((Phq)0.7)) 
        : m_Position(position), m_Light_d(light_d), m_Light_s(light_s)
    {
    }

    PointLight (DimCref x, DimCref y, DimCref z, 
        const Light &light_d = Light((Phq)0.3), const Light &light_s = Light((Phq)0.7)) 
        : m_Position(x, y, z), m_Light_d(light_d), m_Light_s(light_s)
    {
    }


	virtual ~PointLight ()
	{
	}


	/* .. properties */

	SET_GET(Vector3f, Position)
    SET_GET(Light, Light_d)
    SET_GET(Light, Light_s)

protected:
    /* Phong light model */
	Light		m_Light_d;		/* intensity of diffuse component of light */
    Light       m_Light_s;      /* intensity of specular component of light */
	Vector3f	m_Position;
		
private:
    
};      /* class DotLamp */


_QGL_END

#endif  /* ifndef _dot_lamp_H */

/*------------------------------ End of Module -------------------------------*/

