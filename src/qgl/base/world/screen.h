/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : screen.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-06-xx  <Persons>   Created.
 *
 ******************************************************************************/

#ifndef _screen_H
#define _screen_H

/*--------------------------------- Includes ---------------------------------*/

#include <vector>

#include "qgl_common.h"
#include "camera.h"
#include "ray.h"


/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

using namespace std;

/*--------------------------------- Classes ----------------------------------*/

/* Polygons for wireframe */
class Polygon2f
{
public:

    Polygon2f (int size = 0)
    {
        m_Data.count = 0;
        m_Size = size;
        if (size > 0)
        {
            m_Data.vertices = new QglVertex2f[size];
        }
    }

    ~Polygon2f ()
    {
        if (m_Data.vertices)
        {
            delete[] m_Data.vertices;
        }
    }

    const int & GetCount () const
    {
        return m_Data.count;
    }

    void SetCount (int count)
    {
        m_Data.count = count;
    }

    QglVertex2f & operator[] (int index)
    {
        return m_Data.vertices[index];
    }

    const QglVertex2f & operator[] (int index) const
    {
        return m_Data.vertices[index];
    }

    QglPolygonf * GetPtr ()
    {
        return &m_Data;
    }

    const QglPolygonf * GetPtr () const
    {
        return &m_Data;
    }

protected:
    QglPolygonf m_Data;
    int         m_Size;

};


class Polygon3f
{
protected:
    typedef vector<Vector3f>    VecVector;

public:
    typedef Vector3f::Dim       Dim;
    typedef Vector3f::DimCref   DimCref;

public:
    int GetCount () const
    {
        return m_Count;
    }

    void SetCount (int count)
    {
        m_Count = count;
    }

    void Resize (int size)
    {
        m_Vertices.resize(size);
    }

    Vector3f & operator[] (int index)
    {
        return m_Vertices[index];
    }

    const Vector3f & operator[] (int index) const
    {
        return m_Vertices[index];
    }

protected:
    VecVector   m_Vertices;
    int         m_Count;

};

/* Ray creator */

struct RayStart
{
    Camera  *pCamera;
    int     x, y;

    RayStart (Camera *_pCamera, int _x, int _y)
        : pCamera(_pCamera), x(_x), y(_y)
    {
    }
};

typedef Transformer<const RayStart &, Ray &>    RayCreator;

class OrthographicRayCreator : public RayCreator
{
public:
    void SetMatrix (const Matrix3f &matrix)
    {
        m_Matrix = matrix;
    }

    virtual bool Transform (const RayStart &rs, Ray &ray);

protected:
    Matrix3f    m_Matrix;
};

class PerspectiveRayCreator : public RayCreator
{
public:
    void SetViewer (const Camera::Viewer *pViewer)
    {
        m_pViewer = pViewer;
    }

    void SetMatrix (const Matrix3f &matrix)
    {
        m_Matrix = matrix;
    }

    virtual bool Transform (const RayStart &rs, Ray &ray);

protected:
    Matrix3f                m_Matrix;
    const Camera::Viewer    *m_pViewer;
};

/* Screen */

class Screen
{
public:
    typedef Vector3f::Dim       Dim;
    typedef Vector3f::DimRef    DimRef;
    typedef Vector3f::DimCref   DimCref;

    struct Style
    {
        Style () {}
        virtual ~Style () {}
    };

public:

    Screen () {}
    virtual ~Screen () {}

    /*
     * Draw 2D elements directly on the screen in its own coordinate, e.g. `(0,0)-(width,height)'.
     */

    virtual void PutPixel (Style *pStyle, int x, int y) = 0;
    virtual void DrawLine (Style *pStyle, DimCref x0, DimCref y0, DimCref x1, DimCref y1) = 0;

    /* Draw 3D elements in scenic coordinate.
     * The screen takes the responsibility to do coordinate conversion.
     * This may leave the possibility of optimization
     */

    virtual void ProjLine (Style *pStyle, const Vector3f &v0, const Vector3f &v1) = 0;  /* Draw a straight line in scenic coordinate. */
    virtual void ProjPolygon (Style *pStyle, const Polygon3f &polygon) = 0;             /* Draw a polygon in scenic coordinate. */

    virtual void CreateRay (DimCref x, DimCref y, Ray &ray) = 0;    /* Create a ray in scenic coordinate to facilitate ray-trace model */

protected:
    
private:
    
};      /* class Screen */


class RectScreen : public Screen
{
private:
    typedef Screen               _Mybase;

public:
    typedef _Mybase::Dim        Dim;
    typedef _Mybase::DimCref    DimCref;
    typedef _Mybase::DimRef     DimRef;

    typedef Camera::TranMode    TranMode;

    enum ProjMode   /* take special care of consistency */
    {
        k_PmOrthographic = 0,
        k_PmPerspective,
        k_PmMax
    };

public:
    struct ImageBuf
    {
        int         lineByteWidth;
        QglUint8    *data;
        QglUint8    colorPattern[4];    /* position of red, green, blue and alpha */
    };

    struct PixelColor
    {
        QglUint8    red;
        QglUint8    green;
        QglUint8    blue;
        QglUint8    alpha;

        PixelColor ()
            : alpha(0xFF)
        {
        }

        PixelColor (QglUint8 r, QglUint8 g, QglUint8 b, QglUint8 a = 0xFF)
            : red(r), green(g), blue(b), alpha(a)
        {
        }
    };

    enum CamConformType
    {
        k_Unconformed,
        k_SameSize,
        k_Completely,
    };

    struct Style : public Screen::Style
    {
        PixelColor  color;

        Style ()
        {
        }

        Style (const PixelColor &clr) : color(clr)
        {
        }

        virtual ~Style ()
        {
        }

        void SetPixelColor (const PixelColor &clr)
        {
            color = clr;
        }

    };

public:
    RectScreen ();
    RectScreen (DimCref width, DimCref height, Camera *pCamera);

    virtual ~RectScreen ();

    virtual void PutPixel (Screen::Style *pStyle, int x, int y);
    virtual void DrawLine (Screen::Style *pStyle, DimCref x0, DimCref y0, DimCref x1, DimCref y1);

    virtual void ProjLine (Screen::Style *pStyle, const Vector3f &v0, const Vector3f &v1);
    virtual void ProjPolygon (Screen::Style *pStyle, const Polygon3f &polygon);

    void SetImageBuf (QglUint8 *data, int lineByteWidth, const QglUint8 *colorPattern)
    {
        m_Buf.data = data;
        m_Buf.lineByteWidth = lineByteWidth;
        m_Buf.colorPattern[0] = colorPattern[0];
        m_Buf.colorPattern[1] = colorPattern[1];
        m_Buf.colorPattern[2] = colorPattern[2];
        m_Buf.colorPattern[3] = colorPattern[3];
    }

    void SetDims (DimCref width, DimCref height)
    {
        m_Width = width;
        m_Height = height;
        ConformWithCamera();
    }

    void GetDims (DimRef width, DimRef height) const
    {
        width = m_Width;
        height = m_Height;
    }

    const Camera * GetCamera () const
    {
        return m_pCamera;
    }

    void AttachCamera (Camera *pCamera)
    {
        m_pCamera = pCamera;
        SetProjMode(m_ProjMode);
    }

    void SetProjMode (ProjMode mode)
    {
        m_ProjMode = mode;
        if (mode == k_PmOrthographic)
        {
            m_pTranPair = &m_OrthographicTranPair;
            m_pRayCreator = &m_OrthographicRayCreator;
        }
        else /* if (mode == k_PmPerspective) */
        {
            m_pTranPair = &m_PerspectiveTranPair;
            m_pRayCreator = &m_PerspectiveRayCreator;
        }
        ConformWithCamera();
    }

    ProjMode GetProjMode ()
    {
        return m_ProjMode;
    }

    void ConformWithCamera ();  /* be sure to update whenever camera changes */

    static Style * CreateStyle (const PixelColor &color = PixelColor())
    {
        return new Style(color);
    }

    static void DestroyStyle (Style *pStyle)
    {
        delete pStyle;
    }

    void CreateRay (DimCref x, DimCref y, Ray &ray)
    {
        m_pRayCreator->Transform(RayStart(m_pCamera, (int)x, (int)y), ray);
    }

protected:
    static void QglLineFill (void *data, int x, int y);

protected:
    Dim             m_Width, m_Height;
    Camera          *m_pCamera;
    CamConformType  m_CamConformType;

    ImageBuf        m_Buf;


    /* current full-access casting transformer */
    ProjMode                        m_ProjMode;
    Vector3fAffineTransformPair     *m_pTranPair;
    OrthographicVec3TransformPair   m_OrthographicTranPair;
    PerspectiveVec3TransformPair    m_PerspectiveTranPair;
    RayCreator                      *m_pRayCreator;
    OrthographicRayCreator          m_OrthographicRayCreator;
    PerspectiveRayCreator           m_PerspectiveRayCreator;

};

typedef RectScreen *     RectScreenPtr;


_QGL_END

#endif  /* ifndef _screen_H */

/*------------------------------ End of Module -------------------------------*/
