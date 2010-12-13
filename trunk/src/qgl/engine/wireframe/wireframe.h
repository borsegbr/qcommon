/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : wireframe.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-11-16  quanben     Created.
 *
 ******************************************************************************/

#ifndef _wireframe_H
#define _wireframe_H

#include "screen.h"
#include "shape.h"


_QGL_BEGIN

class WireframeStyle
{
public:
    WireframeStyle ()
    {
    }

    virtual ~WireframeStyle ()
    {
    }
};

class SimpleWireframeStyle : public WireframeStyle
{
public:
    typedef RectScreen::PixelColor   PixelColor;
    typedef RectScreen::Style        ScreenSpecStyle;

public:
    SimpleWireframeStyle () : m_Style(PixelColor(0xff, 0xff, 0xff))
    {
    }

    SimpleWireframeStyle (const PixelColor &color)
    {
        SetColor(color);
    }

    virtual ~SimpleWireframeStyle ()
    {
    }

    void SetColor (const PixelColor &color)
    {
        m_Style.SetPixelColor(color);
    }

    ScreenSpecStyle * GetScreenSpecStyle ()
    {
        return &m_Style;
    }

protected:
    ScreenSpecStyle  m_Style;
};

class WireframedShape : virtual public Shape
{
public:
    WireframedShape () {}
    virtual ~WireframedShape () {}

    virtual void DrawWireframe (Screen *pScreen, WireframeStyle *pStyle = NULL) = 0;

};  /* class WireframeDrawer */

_QGL_END


#endif /* ifndef _wireframe_H */

