#ifndef _transformer_H
#define _transformer_H


#include "vector3d.h"
#include "matrix3d.h"


_QGL_BEGIN

/* Transformer */

template <class _TypeIn = const Vector3f &, class _TypeOut = Vector3f &>
class Transformer
{
public:
    typedef _TypeIn   TypeIn;
    typedef _TypeOut  TypeOut;

public:

    Transformer () {}
    virtual ~Transformer () {}

    virtual bool Transform (TypeIn in, TypeOut out) = 0;

    bool operator() (TypeIn in, TypeOut out)
    {
        return Transform(in, out);
    }

    template <class _PtrType>
    _PtrType PtrCast ()
    {
        return dynamic_cast<_PtrType>(this);
    }

};


typedef class Transformer<>   Vector3fTransformer;

class Vector3fAffineTransformer : public Vector3fTransformer
{
    typedef Vector3fTransformer     _Mybase;

public:
    Vector3fAffineTransformer () {}
    
    Vector3fAffineTransformer (const Matrix3f &matrix)
    {
        SetMatrix(matrix);
    }

    virtual ~Vector3fAffineTransformer ()
    {

    }

    SET_GET(Matrix3f, Matrix)

    virtual bool Transform (TypeIn in, TypeOut out)
    {
        out.TransformFrom(m_Matrix, in);
        return true;
    }

    virtual bool LinearTransform (TypeIn in, TypeOut out)
    {
        out.LinearTransformFrom(m_Matrix, in);
        return true;
    }

protected:
    Matrix3f    m_Matrix;
};


/* TransformPair */

template <class _TypeA = Vector3f, class _TypeB = _TypeA, 
    class _TypeAOut = _TypeA &, class _TypeBOut = _TypeB &,
    class _TypeAIn = const _TypeA &, class _TypeBIn = const _TypeB &>
class TransformPair
{
public:
    typedef _TypeA      TypeA;
    typedef _TypeB      TypeB;
    typedef _TypeAIn    TypeAIn;
    typedef _TypeBIn    TypeBIn;
    typedef _TypeAOut   TypeAOut;
    typedef _TypeBOut   TypeBOut;

public:
    TransformPair () {}
    virtual ~TransformPair () {}

    virtual bool Forward (TypeAIn in, TypeBOut out) = 0;
    virtual bool Backward (TypeBIn in, TypeAOut out) = 0;

    template <class _PtrType>
    _PtrType PtrCast ()
    {
        return dynamic_cast<_PtrType>(this);
    }

};

typedef TransformPair<>     Vector3fTransformPair;

class Vector3fAffineTransformPair : public Vector3fTransformPair
{
    typedef Vector3fTransformPair   _Mybase;

public:
    Vector3fAffineTransformPair () {}
    virtual ~Vector3fAffineTransformPair () {}

    SET_GET(Matrix3f, Forward)
    SET_GET(Matrix3f, Backward)

    virtual bool Forward (TypeAIn in, TypeBOut out)
    {
        out.TransformFrom(m_Forward, in);
        return true;
    }

    virtual bool Backward (TypeBIn in, TypeAOut out)
    {
        out.TransformFrom(m_Backward, in);
        return true;
    }

    virtual bool LinearForward (TypeAIn in, TypeBOut out)
    {
        out.LinearTransformFrom(m_Forward, in);
        return true;
    }

    virtual bool LinearBackward (TypeBIn in, TypeAOut out)
    {
        out.LinearTransformFrom(m_Backward, in);
        return true;
    }

protected:
    Matrix3f    m_Forward;
    Matrix3f    m_Backward;
};

_QGL_END


#endif  /* ifndef _transformer_H */
