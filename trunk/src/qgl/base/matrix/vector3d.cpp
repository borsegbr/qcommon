/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : vector3d.cpp
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2005-05-31  yuyiwei     Created.
 *
 ******************************************************************************/

/*--------------------------------- Includes ---------------------------------*/

#include "vector3d.h"


/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*---------------------------------- Types -----------------------------------*/

/*-------------------------------- Constants ---------------------------------*/

/*-------------------------------- Variables ---------------------------------*/

/*----------------------------- Member Functions -----------------------------*/


void Vector3f::GetRotator (DimCref theta, Matrix3f &rotator) const
{
    Dim c = cos(theta);
    Dim s = sin(theta);
    Dim cc = (Dim)1 - c;

    Dim x = m_Data[k_x];
    Dim y = m_Data[k_y];
    Dim z = m_Data[k_z];

    /* Unitize */
    Dim u = Dim(1) / sqrt(x * x + y * y + z * z);
    x *= u;
    y *= u;
    z *= u;

    /*
     * Rotate arround an axis
     * Refer to http://blog.csdn.net/quanben/archive/2007/07/24/1704767.aspx for detail.
     */

    Dim rotator_data[] = 
    {
        x * x * cc + c, x * y * cc - z * s, x * z * cc + y * s, 0,
        x * y * cc + z * s, y * y * cc + c, y * z * cc - x * s, 0,
        z * x * cc - y * s, y * z * cc + x * s, z * z * cc + c, 0,
        0,                  0,                  0,              1
    };

    rotator.set_values(rotator_data);
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
