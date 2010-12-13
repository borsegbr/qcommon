/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : zbuf.cpp
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-06-07  quanben     Created.
 *
 ******************************************************************************/

/*--------------------------------- Includes ---------------------------------*/


#include "zbuf.h"


/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*---------------------------------- Types -----------------------------------*/

/*-------------------------------- Constants ---------------------------------*/

/*-------------------------------- Variables ---------------------------------*/

/*----------------------------- Member Functions -----------------------------*/

ZBuffer::ZBuffer (int width, int height)
    : m_Width(width), m_Height(height)
{
    m_Buf = new Dim[width * height];
}

ZBuffer::~ZBuffer ()
{
    delete m_Buf;
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