/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : ray.h
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

#ifndef _ray_H
#define _ray_H

/*--------------------------------- Includes ---------------------------------*/

#include "qgl_common.h"
#include "vector3d.h"

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*--------------------------------- Classes ----------------------------------*/

class Ray
{
public:

    typedef Vector3f            Direction;
    typedef Vector3f::Dim       Dim;
    typedef Vector3f::DimRef    DimRef;
    typedef Vector3f::DimCref   DimCref;

public:

    Ray ()
    {
    }

    virtual ~Ray ()
    {
    }

    void SetSource (DimCref x, DimCref y, DimCref z, DimCref w = 1)
    {
        m_Source.Set(x, y, z, w);
        m_Source.Normalize();
    }

    void SetSource (const Vector3f &pos)
    {
        m_Source = pos;
        m_Source.Normalize();
    }

    void SetDirection (const Direction &direction)
    {
        m_Direction = direction;
        m_Direction.Normalize();
    }

    Vector3f & GetSource ()
    {
        return m_Source;
    }

    const Vector3f & GetSource () const
    {
        return m_Source;
    }
    
    Direction & GetDirection ()
    {
        return m_Direction;
    }

    const Direction & GetDirection () const
    {
        return m_Direction;
    }

protected:
    
    Vector3f    m_Source;
    Direction   m_Direction;
    
private:
    
};      /* class Ray */

class Intersection
{
public:
    const Vector3f & GetPosition () const { return m_Position; }
    Vector3f & GetPosition () { return m_Position; }
    void SetPosition (const Vector3f & position) { m_Position = position; }

    const Vector3f & GetNormal () const { return m_Normal; }
    Vector3f & GetNormal () { return m_Normal; }
    void SetNormal (const Vector3f & normal) { m_Normal = normal; }

    int GetIdxDir () const { return m_IdxDir; }
    int & GetIdxDir () { return m_IdxDir; }
    void SetIdxDir (int idxDir) { m_IdxDir = idxDir; }

private:
    Vector3f    m_Position;
    Vector3f    m_Normal;		/* unitized normal */
    int         m_IdxDir;       /* This value is set for surface approaching distinction */
};


_QGL_END

#endif  /* ifndef _ray_H */

/*------------------------------ End of Module -------------------------------*/

