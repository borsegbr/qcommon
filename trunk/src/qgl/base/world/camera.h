/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : xxx.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-06-02  quanben     Created.
 *          2007-06-07  quanben     Projection settled, NTT.
 *          2007-11-23  quanben     Use vector transformer instead of mere matrix.
 *
 ******************************************************************************/

#ifndef _camera_H
#define _camera_H

/*--------------------------------- Includes ---------------------------------*/

#include "qgl.h"

#include "transformer.h"


/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

using namespace std;

/*--------------------------------- Classes ----------------------------------*/



class OrthographicVec3TransformPair : public Vector3fAffineTransformPair
{
    typedef Vector3fAffineTransformPair _Mybase;
public:
    virtual bool Backward (TypeBIn in, TypeAOut out)
    {
        _Mybase::Backward(in, out);
        return (out[2] > 0);
    }

};

class PerspectiveVec3TransformPair : public Vector3fAffineTransformPair
{
    typedef Vector3fAffineTransformPair _Mybase;
public:
    virtual bool Backward (TypeBIn in, TypeAOut out)
    {
        _Mybase::Backward(in, out);
        return (out[3] > 0);
    }
};

class Camera
{
public:
    /* .. Types */

    typedef Vector3f        Direction;
    typedef Vector3f::Dim   Dim;
    typedef const Dim &     _ElemCref;

    enum TranMode
    {
        k_TmOrthographic = 0,
        k_TmPerspective,
        k_TmFrustumic,
        k_TmMax
    };

    /*
     * Mapping scheme:
     *   px = kx * x / z + dx
     *¡¡ py = ky * y / z + dy
     *   pz = kz     / z + dz
     * 
     *   (x0,  ?, z0) -> (px0,   ?, pz0), (x1,  ?, z0) -> (px1,   ?, pz0)
     *¡¡ ( ?, y0, z0) -> (  ?, py0, pz0), ( ?, y1, z0) -> (  ?, py1, pz0)
     *   ( ?,  ?, z0) -> (  ?,   ?, pz0), ( ?,  ?, z1) -> (  ?,   ?, pz1)
     *
     * Reference: http://blog.csdn.net/quanben/archive/2007/04/08/1556688.aspx
     *
     */
    struct Frustum
    {
        /* .. coordinates on near plane */
        Dim       x0, px0;      /* `x' here is `U' in some convention */
        Dim       y0, py0;      /* `y' here is `V' in some convention */
        Dim       x1, px1;
        Dim       y1, py1;

        /* .. `z' parameter makes sense only for order maintaining */
        Dim       z0, pz0;      /* near plane, recommended although not compulsively as the screen */
        Dim       z1, pz1;      /* far plane */
    };

    struct Screen
    {
        Dim       x0;           /* left */
        Dim       y0;           /* right */
        Dim       x1;           /* top */
        Dim       y1;           /* bottom */
        Dim       d;            /* perpendicular distance between eye and screen plane */

        Screen ()
        {
        }

        Screen (Dim _x0, Dim _y0, Dim _x1, Dim _y1, Dim _d)
            : x0(_x0), y0(_y0), x1(_x1), y1(_y1), d(_d)
        {
        }

        bool operator== (const Screen &other) const
        {
            return (x0 ==  other.x0 && y0 == other.y0 && x1 == other.x1 && y1 == other.y1 && d == other.d);
        }

        bool operator!= (const Screen &other) const
        {
            return !((*this) == other);
        }
    };

    struct Viewer
    {
        Vector3f    eye;        /* position of viewer's eye in scenic coordinate */  
        Vector3f    x;          /* vector to left in scenic coordinate */
        Vector3f    y;          /* vector to bottom in scenic coordinate */
        Vector3f    z;          /* vector from eye to its target via screen plane in scenic coordinate */

        Viewer ()
        {
        }

        Viewer (const Vector3f &_eye, const Vector3f &_x, const Vector3f &_y, const Vector3f &_z)
        {
            Set(_eye, _x, _y, _z);
        }

        Viewer (const Vector3f &_eye, const Vector3f &_z, Dim tilt)    /* rotate by [:tilt:] clockwise viewed from photographer  */
        {
            Set(_eye, _z, tilt);
        }

        void SetEye (const Vector3f &_eye)
        {
            eye = _eye;
        }

        void SetDir (const Vector3f &_x, const Vector3f &_y, const Vector3f &_z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        void Set (const Vector3f &_eye, const Vector3f &_x, const Vector3f &_y, const Vector3f &_z)
        {
            SetEye(_eye);
            SetDir(_x, _y, _z);
        }

        void Set (const Vector3f &_eye, const Vector3f &_z, Dim tilt);

        bool operator== (const Viewer &other) const
        {
            return (eye == other.eye && x == other.x && y == other.y && z == other.z);
        }

        bool operator!= (const Viewer &other) const
        {
            return !((*this) == other);
        }

        void Regularize ()
        {
            eye.Normalize();
            x.Unitize();
            y.Unitize();
            z.Unitize();
        }

        void Rotate (const Vector3f &axis, Dim alpha)
        {
            Matrix3f    rotator;
            axis.GetRotator(alpha, rotator);
            x *= rotator;
            y *= rotator;
            z *= rotator;
        }

        void TurnUpright ()
        {
            if (z[Vector3f::k_x] == 0 && z[Vector3f::k_y] == 0)
            {
                return;
            }

            Set(eye, z, 0);
        }

    };

public:
    /* .. Member functions */

    Camera ();
    virtual ~Camera ();


    void SetScreen (const Screen &screen);
    const Screen & GetScreen () const;

    void SetViewer (const Viewer &viewer);
    const Viewer & GetViewer () const;

    void UpdateOrthographic ();

    void SetPerspectiveByFrustum (const Frustum &frustum);
    void DerivePerspectiveFromScreen (_ElemCref pz0, _ElemCref z1, _ElemCref pz1);
    void DerivePerspectiveFromScreen ();
    void DeriveScreenFromPerspective ();

    Vector3fTransformPair * GetTranPair (TranMode mode)
    {
        return m_TranPair[mode];
    }

    template <class _PtrType>
    _PtrType GetTranPair (TranMode mode)
    {
        return dynamic_cast<_PtrType>(m_TranPair[mode]);
    }

protected:

    /* .. Member variables */
    Screen      m_Screen;
    Viewer      m_Viewer;       /* the only factor affecting the orthographic transformation */

    /* .... Projection */
    Vector3fTransformPair   *m_TranPair[k_TmMax];  /* transformers in different projection modes */

protected:
    /* .. Member functions */

    void InitTranPairs ();
    void DeinitTranPairs ();
    void SetToDefaultProj ();

private:
    
};      /* class Camera */


_QGL_END

#endif  /* ifndef _camera_H */

/*------------------------------ End of Module -------------------------------*/
