/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : clip.cpp
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-06-30  quanben     Created.
 *          2007-07-01  quanben     Roughly debugged.
 *
 ******************************************************************************/

/*--------------------------------- Includes ---------------------------------*/

#include "clip.h"

/*---------------------------------- Macros ----------------------------------*/

#define LINE_CLIP_MODE  LINE_CLIP_COHEN_SUTHERLAND

/*-------------------------------- Namespaces --------------------------------*/

using namespace qgl;

/*---------------------------------- Types -----------------------------------*/

struct Polygon
{
public:

    struct _Vertex
    {
        QglVertex2f    *state;

        _Vertex (QglVertex2f *p = NULL) : state(p)
        {
        }

        ~_Vertex ()
        {
        }

        void create ()
        {
            state = new QglVertex2f();
        }

        void destroy ()
        {
            if (state)
            {
                delete state;
            }
        }

        bool operator== (const _Vertex &other) const
        {
            return state == other.state;
        }

        QglFloat get_x () const
        {
            return state->x;
        }

        void set_x (QglFloat x)
        {
            state->x = x;
        }

        QglFloat get_y () const
        {
            return state->y;
        }

        void set_y (QglFloat y)
        {
            state->y = y;
        }
    };

    typedef QglFloat    _VTp;

public:

    /* the data */
    union
    {
        QglPolygonf         *state;
        const QglPolygonf   *const_state;
    };

public:

    Polygon (QglPolygonf *p = NULL) : state(p)
    {
    }

    Polygon (const QglPolygonf *p) : const_state(p)
    {
    }

    void create ()
    {
        state = new QglPolygonf;
        state->count = 0;
        state->vertices = NULL;
    }

    void destroy ()
    {
        if (state)
        {
            if (state->vertices)
            {
                delete[] state->vertices;
            }
            delete state;
        }
    }

    Polygon & operator= (const Polygon &copy)
    {
        resize(copy.const_state->count);

        state->count = copy.const_state->count;
        for (int i = 0; i < state->count; i++)
        {
            state->vertices[i] = copy.const_state->vertices[i];
        }

        return *this;
    }

    _Vertex first () const
    {
        return _Vertex(const_state->vertices + 0);
    }

    _Vertex last () const
    {
        return _Vertex(const_state->vertices + state->count - 1);
    }

    _Vertex next (const _Vertex &cur) const
    {
        return _Vertex(cur.state + 1);
    }

    int count () const
    {
        return state->count;
    }

    void clear ()
    {
        state->count = 0;
    }

    void resize (int cnt)
    {
        if (state->vertices)
        {
            delete[] state->vertices;
        }
        state->vertices = new QglVertex2f[cnt];
        state->count = 0;   /* All data cleared */
    }

    void push_back (const _Vertex &vtx)
    {
        state->vertices[state->count++] = *vtx.state;
    }
};

/*-------------------------------- Constants ---------------------------------*/

/*-------------------------------- Variables ---------------------------------*/

/*----------------------------- Member Functions -----------------------------*/

/*------------------------- Static Member Functions --------------------------*/


/*------------------- Functions with C Calling Convention --------------------*/

/* 
 * .. Line Clipping 
 */

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

#if LINE_CLIP_MODE == LINE_CLIP_COHEN_SUTHERLAND
QglBool Qgl_LineClip (QglReal *x0, QglReal *y0, QglReal *x1, QglReal *y1,
                      QglReal xL, QglReal xR, QglReal yT, QglReal yB)
{
    return ShLineClip<QglReal>::Clip(*x0, *y0, *x1, *y1, xL, xR, yT, yB)? k_True : k_False;
}

#elif LINE_CLIP_MODE == LINE_CLIP_LIANG_BARSKEY
QglBool Qgl_LineClip (QglReal *x0, QglReal *y0, QglReal *x1, QglReal *y1,
                      QglReal xL, QglReal xR, QglReal yT, QglReal yB)
{
    return LbLineClip<QglReal>::Clip(*x0, *y0, *x1, *y1, xL, xR, yT, yB)? k_True : k_False;
}

#else

#   error "Bad mode!"

#endif

/* 
 * .. Polygon Clipping
 */


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
void Qgl_PolygonClip (const QglPolygonf *polygonIn, QglPolygonf *polygonOut, 
                      QglReal xL, QglReal xR, QglReal yT, QglReal yB)
{
    const Polygon in(polygonIn);
    Polygon out(polygonOut);
    Polygon temp;
    temp.create();
    temp.resize(in.count() * 2);
    ShPolygonClip<Polygon>::Clip(in, temp, out, xL, xR, yT, yB);
}

/*------------------------------ End of Module -------------------------------*/
