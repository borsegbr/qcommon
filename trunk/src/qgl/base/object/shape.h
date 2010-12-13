/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : shape.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-05-31  quanben     Created.
 *
 ******************************************************************************/

#ifndef _shape_H
#define _shape_H

/*--------------------------------- Includes ---------------------------------*/

#include "object.h"
#include "screen.h"

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*--------------------------------- Classes ----------------------------------*/

class Shape : virtual public Object
{
public:
    typedef RectScreen::PixelColor   PixelColor;

    Shape ()
    {
    }

    virtual ~Shape ()
    {
    }

protected:
    
private:

};      /* class Shape */

class Primitive : virtual public Shape
{
public:
    

public:
    Primitive () : Shape()
    {
    }

    virtual ~Primitive ()
    {
    }

protected:
    
private:
    
};      /* class Primitive */


/** 
 * Combined shapes, indicating different treatment and 
 * probably a combination of primitives */

class Combined : virtual public Shape
{
public:
    Combined () : Shape ()
    {
    }

    virtual ~Combined ()
    {
    }

protected:
    
private:
    
};      /* class Combined */


_QGL_END

#endif  /* ifndef _shape_H */

/*------------------------------ End of Module -------------------------------*/
