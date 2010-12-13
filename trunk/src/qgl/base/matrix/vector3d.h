/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : vector3d.h
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

#ifndef _vector3d_H
#define _vector3d_H

/*--------------------------------- Includes ---------------------------------*/

#include <cmath>

#include "qgl_common.h"
#include "matrix3d.h"

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*--------------------------------- Classes ----------------------------------*/

class Vector3f
{
public:
    typedef QglReal     Dim;

    typedef Dim &       DimRef;
    typedef const Dim & DimCref;

    enum
    {
        k_x = 0,
        k_y,
        k_z,
        k_w
    };

public:

    Vector3f ()
    {
        Clear();
    }

    Vector3f (const Vector3f &other)
    {
        *this = other;
    }

    Vector3f (DimCref x, DimCref y, DimCref z, DimCref w = 1)
    {
        Set(x, y, z, w);
    }

    virtual ~Vector3f ()
    {
    }

    void Clear ()
    {
        m_Data[k_x] = m_Data[k_y] = m_Data[k_z] = Dim(0);
        m_Data[k_w] = Dim(1);
    }

    void Set (DimCref x, DimCref y, DimCref z, DimCref w = 1)
    {
        m_Data[k_x] = x;
        m_Data[k_y] = y;
        m_Data[k_z] = z;
        m_Data[k_w] = w;
    }

    void Get_x (DimRef x) const
        /* Normalization-free */
    {
        x = m_Data[k_x] / m_Data[k_w];
    }

    void Get_y (DimRef y) const
        /* Normalization-free */
    {
        y = m_Data[k_y] / m_Data[k_w];
    }

    void Get_z (DimRef z) const
        /* Normalization-free */
    {
        z = m_Data[k_z] / m_Data[k_w];
    }

    const Dim & operator[] (int idx) const
    {
        return m_Data[idx];
    }

    Dim & operator[] (int idx)
    {
        return m_Data[idx];
    }

    Dim GetSquaredLength_Nn () const
        /* Normalization NEEDED */
    {
        return m_Data[k_x] * m_Data[k_x] + m_Data[k_y] * m_Data[k_y] + m_Data[k_z] * m_Data[k_z];
    }

    Dim GetSquaredLength () const
        /* Normalization-free */
    {
        return GetSquaredLength_Nn() / (m_Data[k_w] * m_Data[k_w]);
    }

    Dim GetLength_Nn () const
        /* Normalization NEEDED */
    {
        return sqrt(GetSquaredLength_Nn());
    }

    Dim GetLength () const
        /* Normalization-free */
    {
        return GetLength_Nn() / ((m_Data[k_w] < Dim(0))? -m_Data[k_w] : m_Data[k_w]);
    }

    bool Normalized () const
    {
        return m_Data[k_w] == Dim(1);
    }

    Vector3f & Normalize ()
    {
        Dim f = Dim(1) / m_Data[k_w];
        m_Data[k_x] *= f;
        m_Data[k_y] *= f;
        m_Data[k_z] *= f;
        m_Data[k_w] = Dim(1);

        return *this;
    }

    Vector3f & Unitize ()
    {
        Dim u = Dim(1) / GetLength_Nn();
        if (m_Data[k_w] < 0)
        {
            u = -u;
        }
        m_Data[k_x] *= u;
        m_Data[k_y] *= u;
        m_Data[k_z] *= u;
        m_Data[k_w] = 1;

        return *this;
    }

    bool operator== (const Vector3f &other) const
        /* Normalization NEEDED, accuracy sensitive */
    {
        return (m_Data[0] == other.m_Data[0] 
                && m_Data[1] == other.m_Data[1] 
                && m_Data[2] == other.m_Data[2]);
    }

    bool operator!= (const Vector3f &other) const
        /* Normalization NEEDED, accuracy sensitive */
    {
        return !(*this == other);
    }

    Vector3f operator- () const
        /* Normalization free */
    {
        Vector3f temp;
        temp.m_Data[0] = -m_Data[0];
        temp.m_Data[1] = -m_Data[1];
        temp.m_Data[2] = -m_Data[2];
        temp.m_Data[3] = m_Data[3];

        return temp;
    }

    Vector3f & operator= (const Vector3f &other)
        /* Normalization free */
    {
        m_Data[0] = other.m_Data[0];
        m_Data[1] = other.m_Data[1];
        m_Data[2] = other.m_Data[2];
        m_Data[3] = other.m_Data[3];

        return *this;
    }

    Vector3f & operator+= (const Vector3f &other)
        /* Normalization NEEDED */
    {
        m_Data[0] += other.m_Data[0];
        m_Data[1] += other.m_Data[1];
        m_Data[2] += other.m_Data[2];
        return *this;
    }

    Vector3f & operator-= (const Vector3f &other)
        /* Normalization NEEDED */
    {
        m_Data[0] -= other.m_Data[0];
        m_Data[1] -= other.m_Data[1];
        m_Data[2] -= other.m_Data[2];
        return *this;
    }

    Vector3f operator+ (const Vector3f &other) const
        /* Normalization NEEDED */
    {
        Vector3f temp = *this;
        temp += other;
        return temp;
    }

    Vector3f operator- (const Vector3f &other) const
        /* Normalization NEEDED */
    {
        Vector3f temp = *this;
        temp -= other;
        return temp;
    }

    Dim Dot_Nn (const Vector3f &other) const
        /* Normalization NEEDED */
    {
    	Dim result;
    	return Dot_Nn(other, result);
    }

    DimRef Dot_Nn (const Vector3f &other, DimRef dot) const
        /* Normalization NEEDED */
    {
        dot = (m_Data[0] * other.m_Data[0] + m_Data[1] * other.m_Data[1] + m_Data[2] * other.m_Data[2]);
		return dot;
    }

    Dim Dot (const Vector3f &other) const
        /* Normalization-free */
    {
    	Dim result;
		return Dot(other, result);
    }

    DimRef Dot (const Vector3f &other, DimRef dot) const
        /* Normalization-free */
    {
        dot = Dot_Nn(other) / (m_Data[3] * other.m_Data[3]);
		return dot;
    }

    Vector3f & product (const Vector3f &a, const Vector3f &b)
    {
        Dim ax = a.m_Data[k_x];
        Dim ay = a.m_Data[k_y];
        Dim az = a.m_Data[k_z];
        Dim aw = a.m_Data[k_w];

        Dim bx = b.m_Data[k_x];
        Dim by = b.m_Data[k_y];
        Dim bz = b.m_Data[k_z];
        Dim bw = b.m_Data[k_w];

        m_Data[k_x] = ay * bz - az * by;
        m_Data[k_y] = az * bx - ax * bz;
        m_Data[k_z] = ax * by - ay * bx;
        m_Data[k_w] = aw * bw;

        return (*this);
    }

    Vector3f & operator*= (const Vector3f &other)
        /* cross product right multiplied by other */
        /* Normalization-free */
    {
        return product(*this, other);
    }

    Vector3f operator* (const Vector3f &other) const
        /* Normalization-free */
    {
        return Vector3f().product(*this, other);
    }

    Vector3f & TransformFrom (const Matrix3f &tr, const Vector3f &v)
    {
        Dim x = v.m_Data[0];
        Dim y = v.m_Data[1];
        Dim z = v.m_Data[2];
        Dim w = v.m_Data[3];

        m_Data[0] = tr[0][0] * x + tr[0][1] * y + tr[0][2] * z + tr[0][3] * w;
        m_Data[1] = tr[1][0] * x + tr[1][1] * y + tr[1][2] * z + tr[1][3] * w;
        m_Data[2] = tr[2][0] * x + tr[2][1] * y + tr[2][2] * z + tr[2][3] * w;
        m_Data[3] = tr[3][0] * x + tr[3][1] * y + tr[3][2] * z + tr[3][3] * w;

        return (*this);
    }

    Vector3f & LinearTransformFrom (const Matrix3f &tr, const Vector3f &v)
    {
        Dim x = v.m_Data[0];
        Dim y = v.m_Data[1];
        Dim z = v.m_Data[2];
        Dim w = v.m_Data[3];

        m_Data[0] = tr[0][0] * x + tr[0][1] * y + tr[0][2] * z;
        m_Data[1] = tr[1][0] * x + tr[1][1] * y + tr[1][2] * z;
        m_Data[2] = tr[2][0] * x + tr[2][1] * y + tr[2][2] * z;
        m_Data[3] = tr[3][3] * w;

        return (*this);
    }

    Vector3f & operator*= (const Matrix3f &tr)
        /* left multiplied by a matrix */
        /* Normalization-free */
    {
        return TransformFrom(tr, *this);
    }

    
    Vector3f & operator*= (Dim factor)
        /* Normalization-free */
    {
        m_Data[0] *= factor;
        m_Data[1] *= factor;
        m_Data[2] *= factor;
        
        return *this;
    }

#if 0
    template <class T>
        Vector3f & operator*= (T factor)
        /* Normalization-free */
    {
        m_Data[0] *= factor;
        m_Data[1] *= factor;
        m_Data[2] *= factor;
        
        return *this;
    }
#endif

    void ToVertex2d (Dim &x, Dim &y) const
    {
        x = m_Data[0] / m_Data[3];
        y = m_Data[1] / m_Data[3];
    }

    void ToVertex3d (Dim &x, Dim &y, Dim &z) const
    {
        ToVertex2d(x, y);
        z = m_Data[2] / m_Data[3];
    }

    void GetRotator (DimCref theta, Matrix3f &rotator) const;

    static Dim GetSquaredDistance_Nn (const Vector3f &a, const Vector3f &b)
    {
        Vector3f d = a - b;
        return d.GetSquaredLength_Nn();
    }

    static Dim GetDistance_Nn (const Vector3f &a, const Vector3f &b)
    {
        Vector3f d = a - b;
        return d.GetLength_Nn();
    }


protected:
    Dim m_Data[4];
    
private:
    
};      /* class Vector3f */


static inline Vector3f operator* (const Matrix3f &tr, const Vector3f &v)
{
    return Vector3f().TransformFrom(tr, v);
}


_QGL_END

#endif  /* ifndef _vector3d_H */



/*------------------------------ End of Module -------------------------------*/
