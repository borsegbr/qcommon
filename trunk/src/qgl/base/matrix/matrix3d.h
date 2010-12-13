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
 * 1.00     2005-05-31  yuyiwei     Created.
 *
 ******************************************************************************/

#ifndef _matrix3d_H
#define _matrix3d_H

/*--------------------------------- Includes ---------------------------------*/

#include <qtl/math/matrix.h>

#include "qgl.h"

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

using namespace qtl;

/*--------------------------------- Classes ----------------------------------*/

typedef QglReal     _Matrix3dElem;

class Matrix3f : public square_matrix<_Matrix3dElem, 4>
{
    typedef square_matrix<_Matrix3dElem, 4> _Mybase;

    typedef _Matrix3dElem                   Dim;

public:
    friend class Vector3f;

    Matrix3f () : _Mybase()
    {
    }

    Matrix3f (_LinearCPtr rawData) : _Mybase(rawData)
    {
    }

    virtual ~Matrix3f ()
    {
    }
   
protected:
    
private:
    
};      /* class Matrix */


_QGL_END

#endif  /* ifndef _matrix3d_H */

/*------------------------------ End of Module -------------------------------*/
