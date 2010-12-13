/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : luminant.h
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

#ifndef _luminant_H
#define _luminant_H

/*--------------------------------- Includes ---------------------------------*/

#include "qgl.h"

#include "object.h"
#include "vector3d.h"
#include "ray.h"
#include "light.h"

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*--------------------------------- Classes ----------------------------------*/

class Luminant : virtual public Object
{
public:
    Luminant () {}
    virtual ~Luminant () {}
    
protected:
    
private:
    
};      /* class Luminant */


_QGL_END

#endif  /* ifndef _luminant_H */

/*------------------------------ End of Module -------------------------------*/
