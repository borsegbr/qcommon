/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : screen.cpp
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

#include "screen.h"

#include "line.h"


/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN


/*---------------------------------- Types -----------------------------------*/

typedef Screen::Dim Dim;

typedef RectScreen::ImageBuf     ImageBuf;

struct LinePixieData
{
    typedef RectScreen::PixelColor   Color;
    typedef QglUint8                ClrComp;

    ClrComp     color[4];   /* BGRA */
    ImageBuf    *buf;

    LinePixieData (Color _color, ImageBuf *_buf)
        : buf(_buf)
    {
        color[buf->colorPattern[0]] = _color.red;
        color[buf->colorPattern[1]] = _color.green;
        color[buf->colorPattern[2]] = _color.blue;
        color[buf->colorPattern[3]] = _color.alpha;
    }
};

/*-------------------------------- Constants ---------------------------------*/

/*-------------------------------- Variables ---------------------------------*/

/*----------------------------- Member Functions -----------------------------*/

/* Ray creator */

bool OrthographicRayCreator::Transform (const RayStart &rs, Ray &ray)
{
    Vector3f                scrnCntrScreen((Dim)rs.x, (Dim)rs.y, 0);
    Vector3f                scrnCntrScene;
    Vector3f                dirScreen(0, 0, 1);
    Vector3f                dirScene;

    scrnCntrScene.TransformFrom(m_Matrix, scrnCntrScreen);
    dirScene.LinearTransformFrom(m_Matrix, dirScreen);

    ray.SetSource(scrnCntrScene);
    ray.SetDirection(dirScene);

    return true;
}


bool PerspectiveRayCreator::Transform (const RayStart &rs, Ray &ray)
{
    Vector3f                eye = m_pViewer->eye;
    Vector3f                scrnCntrScreen((Dim)rs.x, (Dim)rs.y, 0);
    Vector3f                scrnCntrScene;
    Vector3f                dirScene;

    scrnCntrScene.TransformFrom(m_Matrix, scrnCntrScreen);
    scrnCntrScene.Normalize();

    // it is more reasonable to set the eye as source
    ray.SetSource(eye);
    dirScene = scrnCntrScene - eye;
    ray.SetDirection(dirScene);

    return true;

}


/* Screen */

RectScreen::RectScreen () : m_pCamera(NULL), m_ProjMode(k_PmOrthographic), m_Width(40), m_Height(30)
{
}

RectScreen::RectScreen (DimCref width, DimCref height, Camera *pCamera)
    : m_Width(width), m_Height(height), m_pCamera(pCamera)
{
    SetProjMode(k_PmOrthographic);
}

RectScreen::~RectScreen ()
{
}


void RectScreen::PutPixel (Screen::Style *pStyle, int x, int y)
{
    Style *rsStyle = dynamic_cast<Style *>(pStyle);

    int             pos = y * m_Buf.lineByteWidth + x * 4;
    QglUint8        *bufData = m_Buf.data + pos;
    PixelColor      &color = rsStyle->color;

    bufData[m_Buf.colorPattern[0]] = color.red;
    bufData[m_Buf.colorPattern[1]] = color.green;
    bufData[m_Buf.colorPattern[2]] = color.blue;
    bufData[m_Buf.colorPattern[3]] = color.alpha;

}

void RectScreen::DrawLine (Screen::Style *pStyle, DimCref x0, DimCref y0, DimCref x1, DimCref y1)
{
    int x0i = (int)(x0 + 0.5);
    int y0i = (int)(y0 + 0.5);
    int x1i = (int)(x1 + 0.5);
    int y1i = (int)(y1 + 0.5);

    Style *rsStyle = dynamic_cast<Style *>(pStyle);
    LinePixieData   pixieData(rsStyle->color, &m_Buf);

    QglPixie pixie;
    pixie.data = &pixieData;
    pixie.set = QglLineFill;

    Qgl_Line(x0i, y0i, x1i, y1i, &pixie);

}

void RectScreen::ProjLine (Screen::Style *pStyle, const Vector3f &v0, const Vector3f &v1)
{
    /* convert */
    Dim x0, y0;
    Dim x1, y1;

    Vector3f v0t, v1t;

    bool b0 = m_pTranPair->Backward(v0, v0t);
    bool b1 = m_pTranPair->Backward(v1, v1t);
    if (!b0 && !b1)
    {
        return;
    }

    Vector3f temp, temp1;

#define	USE_ON_SCREEN	1

#if USE_ON_SCREEN
/* It needs to be proved that this method improves the drawing and right carried out by the code */
#define GET_ON_SCREEN(a, b)   \
    m_OrthographicTranPair.Backward(a, a##t);   \
    m_OrthographicTranPair.Backward(b, temp);   \
    Dim a##tz = a##t[Vector3f::k_z];    \
    Dim b##tz = temp[Vector3f::k_z];    \
    a##t *= b##tz;  \
    temp *= a##tz;  \
    temp -= a##t;   \
    a##tz -= b##tz; \
    if (a##tz < 0.001) return; /* throw away instable solution */ \
    temp *= 1 / a##tz;  \
    m_OrthographicTranPair.Forward(temp, temp1);    \
    m_pTranPair->Backward(temp1, a##t);
#else
	GET_ON_SCREEN(a, b)		return;
#endif

    if (!b0)
    {
        GET_ON_SCREEN(v0, v1)
    }
    else if (!b1)
    {
        GET_ON_SCREEN(v1, v0)
    }

#undef GET_ON_SCREEN

    v0t.ToVertex2d(x0, y0);
    v1t.ToVertex2d(x1, y1);

    /* clip */
    if (Qgl_LineClip(&x0, &y0, &x1, &y1, Dim(0), m_Width, Dim(0), m_Height))
    {
        /* draw */
        DrawLine(pStyle, x0, y0, x1, y1);
    }
}

void RectScreen::ProjPolygon (Screen::Style *pStyle, const Polygon3f &polygon)
{

    /* convert */
    int count = polygon.GetCount();
    Polygon2f polygonIn(count);

    Vector3f v3d0 = polygon[count - 1];
    Vector3f v3d1;
    for (int i = 0; i < count; i++)
    {
        v3d1 = polygon[i];
        ProjLine(pStyle, v3d0, v3d1);
        v3d0 = v3d1;
    }

}

void RectScreen::ConformWithCamera ()
{
    if (m_pCamera == NULL)
    {
        return;
    }

    const Camera::Screen  &screen = m_pCamera->GetScreen();
    
    Dim kx = m_Width / (screen.x1 - screen.x0);
    Dim ky = m_Height / (screen.y1 - screen.y0);

    Dim dx = -kx * screen.x0;
    Dim dy = -ky * screen.y0;

    Dim ssInit[] = {
        kx, 0,  0, dx,
        0, ky,  0, dy,
        0,  0,  1,  0,
        0,  0,  0,  1
    };

    Matrix3f        screenScaler(ssInit);
    Matrix3f        temp;
    Vector3fAffineTransformPair *pCamPair;

    // Set for orthographic
    pCamPair = m_pCamera->GetTranPair<Vector3fAffineTransformPair *>(Camera::k_TmOrthographic);
    temp.product(screenScaler, pCamPair->GetBackward());

    m_OrthographicTranPair.SetBackward(temp);

    temp.inverse();

    m_OrthographicTranPair.SetForward(temp);
    m_OrthographicRayCreator.SetMatrix(temp);

    // Set for perspective
    pCamPair = m_pCamera->GetTranPair<Vector3fAffineTransformPair *>(Camera::k_TmPerspective);
    temp.product(screenScaler, pCamPair->GetBackward());

    m_PerspectiveTranPair.SetBackward(temp);

    temp.inverse();

    m_PerspectiveTranPair.SetForward(temp);
    m_PerspectiveRayCreator.SetMatrix(temp);
    m_PerspectiveRayCreator.SetViewer(&m_pCamera->GetViewer());

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

void RectScreen::QglLineFill (void *data, int x, int y)
{
    LinePixieData   *pixieData = (LinePixieData *)data;
    ImageBuf        *buf = (ImageBuf *)pixieData->buf;
    int             pos = y * buf->lineByteWidth + x * 4;
    QglUint8        *bufData = buf->data + pos;

    bufData[0] = pixieData->color[0];
    bufData[1] = pixieData->color[1];
    bufData[2] = pixieData->color[2];
    bufData[3] = pixieData->color[3];
}

_QGL_END

/*------------------------------ End of Module -------------------------------*/
