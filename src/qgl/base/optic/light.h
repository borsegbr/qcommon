/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : light.h
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

#ifndef _light_H
#define _light_H

/*--------------------------------- Includes ---------------------------------*/

#include "qgl.h"

#include "qgl_common.h"
#include "screen.h"

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN


/*--------------------------------- Classes ----------------------------------*/

struct ColorComponents
{
    Phq	r;
    Phq g;
    Phq	b;

    ColorComponents (PhqCref val = 0) : r(val), g(val), b(val)
    {
    }

    ColorComponents (PhqCref __r, PhqCref __g, PhqCref __b)
        : r(__r), g(__g), b(__b)
    {
    }

    ColorComponents (const ColorComponents &copy)
        : r(copy.r), g(copy.g), b(copy.b)
    {
    }

    void Set (PhqCref __r = 0, PhqCref __g = 0, PhqCref __b = 0)
    {
        r = __r, g = __g, b = __b;
    }

    bool operator== (const ColorComponents &other) const
    {
        return (r == other.r && g == other.g && b == other.b);
    }

    ColorComponents & operator= (PhqCref val)
    {
        r = g = b = val;
        return (*this);
    }

    ColorComponents & operator= (const ColorComponents &copy)
    {
        r = copy.r;
        g = copy.g;
        b = copy.b;
        return (*this);
    }

    ColorComponents & operator+= (const ColorComponents &other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        return (*this);
    }

    ColorComponents & operator-= (const ColorComponents &other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        return (*this);
    }

    ColorComponents & operator*= (const ColorComponents &other)
    {
        r *= other.r;
        g *= other.g;
        b *= other.b;
        return (*this);
    }

    ColorComponents & operator*= (PhqCref rate)
    {
        r *= rate;
        g *= rate;
        b *= rate;
        return (*this);
    }

    ColorComponents & operator/= (const ColorComponents &other)
    {
        r /= other.r;
        g /= other.g;
        b /= other.b;
        return (*this);
    }

    ColorComponents & Rescale (PhqCref rr, PhqCref gr, PhqCref br)
    {
        r *= rr;
        g *= gr;
        b *= br;
        return (*this);
    }
    
    operator bool () const
    {
        return (r > 0 || g > 0 || b > 0);
    }

};

class Light : public ColorComponents
{
private:
    typedef ColorComponents    _Mybase;

public:
    typedef Phq		ColorComponent;

    Light () : _Mybase()
    {
    }

    Light (PhqCref value) : _Mybase(value)
    {
    }

    Light (PhqCref __r, PhqCref __g, PhqCref __b) : _Mybase(__r, __g, __b)
    {
    }

    Light (const Light &other) : _Mybase(other)
    {
    }

    virtual ~Light ()
    {
    }

    void ToPixelColor (RectScreen::PixelColor &color)
    {
        unsigned int temp = (unsigned int)(r * 255 + 0.5);
        color.red = temp > 255? 255 : temp;
        temp = (unsigned int)(g * 255 + 0.5);
        color.green = temp > 255? 255 : temp;
        temp = (unsigned int)(b * 255 + 0.5);
        color.blue = temp > 255? 255 : temp;
    }

protected:
    _Mybase::r;
    _Mybase::g;
    _Mybase::b;

};      /* class Light */


_QGL_END

#endif  /* ifndef _light_H */

/*------------------------------ End of Module -------------------------------*/
