/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : qgl.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     xxxx-xx-xx  yuyiwei     Created.
 *
 ******************************************************************************/


#ifndef _qgl_H
#define _qgl_H

/*--------------------------------- Includes ---------------------------------*/

/*---------------------------------- Macros ----------------------------------*/

#if !defined(NULL)
#	define NULL			(0)
#endif

#define QGL_EXPORT		__declspec(dllexport)

/* .. Switches */

#define USE_DOUBLE      0

/* .. Constants */
#if defined(USE_DOUBLE_SWITCH) &&  USE_DOUBLE_SWITCH
#   define k_RealZero   (0.0)
#   define k_RealOne    (1.0)
#   define k_RealPi     (3.14159265359)
#   define k_RealDblPi  (6.28318530718)
#   define k_RealHlfPi  (1.57079632679)
#else
#   define k_RealZero   (0.0f)
#   define k_RealOne    (1.0f)
#   define k_RealPi     (3.14159265359f)
#   define k_RealDblPi  (6.28318530718f)
#   define k_RealHlfPi  (1.57079632679f)
#endif

/* .. For CPP  */

#define _QGL_BEGIN      namespace qgl {
#define _QGL_END        }

/*---------------------------------- Types -----------------------------------*/

/* .. Redefinition */

typedef int				QglBool;

typedef char			QglInt8;
typedef signed char		QglSint8;
typedef unsigned char	QglUint8;
typedef short			QglInt16;
typedef signed short	QglSint16;
typedef unsigned short	QglUint16;
typedef long			QglInt32;
typedef signed long		QglSint32;
typedef unsigned long	QglUint32;
typedef int				QglInt;
typedef	signed int		QglSint;
typedef unsigned int	QglUint;

typedef float			QglFloat;
typedef double			QglDouble;

#if defined(USE_DOUBLE_SWITCH) &&  USE_DOUBLE_SWITCH
typedef double          QglReal;
#else
typedef float           QglReal;
#endif


typedef QglUint32        QglColorRef;    /* ARGB */

/* .. Structures */

typedef struct _QglVertex2i
{
    QglInt		x;
    QglInt		y;
} QglVertex2i;

typedef struct _QglVertex2f
{
    QglFloat	x;
    QglFloat	y;
} QglVertex2f;

typedef struct _QglPolygoni
{
    QglVertex2i	*vertices;
    QglInt		count;
} QglPolygoni;

typedef struct _QglPolygonf
{
    QglVertex2f	*vertices;
    QglInt		count;
} QglPolygonf;

typedef struct _QglRect
{
    QglInt	left;
    QglInt	top;

    QglInt	right;	/* excluded */
    QglInt	bottom;	/* excluded */
} QglRect;

typedef struct _QglPixie
{
    void	*data;
    void	(*set)(void *data, QglInt x, QglInt y);
} QglPixie;

typedef struct _QglSmartPixie
{
    void	*data;
    void	(*set)(void *data, QglInt x, QglInt y, QglInt channel, QglInt value);
    QglInt	(*get)(void *data, QglInt x, QglInt y, QglInt channel);
} QglSmartPixie;

typedef struct _QglOpaqStruct
{
    QglUint8    opaq[1];
} QglOpaqStruct, *QglHandle;

typedef QglHandle   QglObjHandle;

typedef enum _QglBindType
{
    k_Union = 0,
    k_Intersection,
    k_Subtraction,
} QglBindType;


/*-------------------------------- Constants ---------------------------------*/

/* .. Global */
enum
{
    k_False = 0,
    k_True = 1,
};


/* .. Pixie Signals */
/* .... Seed filling */
enum
{
    k_QglSeedSetFill = 0
};

enum
{
    k_QglSeedGetFilled = 0,
    k_QglSeedGetBorder,
    k_QglSeedGetEither,
};

typedef enum _ProjMode
{
    k_QglOrthographic = 0,
    k_QglPerspective,
    k_QglProjModeMax
} ProjMode;

typedef enum _CamAxis
{
    k_CamX = 0,
    k_CamY,
    k_CamZ
} CamAxis;


/*-------------------------------- Variables ---------------------------------*/

/*-------------------------------- Functions ---------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/* .... Filling */
QGL_EXPORT void Qgl_PolygonFill (const QglPolygoni *polygon, QglPixie *pixie);
QGL_EXPORT void Qgl_SeedFill (QglSmartPixie *pixie, QglInt x, QglInt y);	/* Set definition detail  */

/* .... Lining */
QGL_EXPORT void Qgl_Line (QglInt x0, QglInt y0, QglInt x1, QglInt y1, QglPixie *pixie);
QGL_EXPORT void Qgl_Circle (QglInt x0, QglInt y0, QglInt radius, QglPixie *pixie);
QGL_EXPORT void Qgl_Ellipse (QglInt x0, QglInt y0, QglInt a, QglInt b, QglPixie *pixie);

/* .... Clipping */
QGL_EXPORT QglBool Qgl_LineClip (QglFloat *x0, QglFloat *y0, QglFloat *x1, QglFloat *y1, 
								 QglFloat xL, QglFloat xR, QglFloat yT, QglFloat yB);
QGL_EXPORT void Qgl_PolygonClip (const QglPolygonf *polygonIn, QglPolygonf *polygonOut, 
								 QglFloat xL, QglFloat xR, QglFloat yT, QglFloat yB);

/* .... 3D Elements Drawing */
QGL_EXPORT QglHandle Qgl_CreateHandle (void);
QGL_EXPORT void Qgl_DestroyHandle (QglHandle h);
QGL_EXPORT void Qgl_SetBuffer (QglHandle h, QglUint8 *buffer, QglInt bufByteWidth, QglInt bufSize);
/* ...... Screen */
QGL_EXPORT void Qgl_SetProjMode (QglHandle h, ProjMode mode);
QGL_EXPORT void Qgl_SetCameraPose (QglHandle h, QglReal x, QglReal y, QglReal z, QglReal ax, QglReal ay, QglReal az, QglReal tilt);
QGL_EXPORT void Qgl_GetCameraPos (QglHandle h, QglReal *pX, QglReal *pY, QglReal *pZ);
QGL_EXPORT void Qgl_SetCameraPos (QglHandle h, QglReal x, QglReal y, QglReal z);
QGL_EXPORT void Qgl_GetCameraAxis (QglHandle h, CamAxis axis, QglReal *pX, QglReal *pY, QglReal *pZ);
QGL_EXPORT void Qgl_SetCameraAxis (QglHandle h, CamAxis axis, QglReal x, QglReal y, QglReal z);
QGL_EXPORT void Qgl_SetCameraScr (QglHandle h, QglReal width, QglReal height, QglReal d);
QGL_EXPORT void Qgl_RotateCamera (QglHandle h, QglReal ax, QglReal ay, QglReal az, QglReal alpha);
QGL_EXPORT void Qgl_TurnUprightCamera (QglHandle h);
QGL_EXPORT void Qgl_SetScreen (QglHandle h, QglReal width, QglReal height);
QGL_EXPORT void Qgl_Clear (QglHandle h);
/* ...... Wireframe */
QGL_EXPORT void Qgl_DrawWireframe (QglHandle h, QglObjHandle hObj, QglColorRef color);
/* ...... Ray-trace Rendering */
QGL_EXPORT void Qgl_SetTraceDepth (QglHandle h, int depth);
QGL_EXPORT void Qgl_RaytraceRender (QglHandle h);

/* .... Objects */
QGL_EXPORT QglObjHandle Qgl_CreateSphere (QglReal cx, QglReal cy, QglReal cz, QglReal r);
QGL_EXPORT QglObjHandle Qgl_CreateCsg (QglObjHandle hObjL, QglObjHandle hObjR, QglBindType type);
QGL_EXPORT void Qgl_AddShape (QglHandle h, QglObjHandle hObj);
QGL_EXPORT QglObjHandle Qgl_AddPointLight (QglHandle h, QglReal cx, QglReal cy, QglReal cz);

QGL_EXPORT void Qgl_SetObjOpticEasy (QglObjHandle hObj, QglReal r, QglReal g, QglReal b, 
                                     QglReal transparency, QglReal eta);
QGL_EXPORT void Qgl_SetObjOptic (QglObjHandle hObj, QglReal r, QglReal g, QglReal b,
                                 QglReal transparency, QglReal ambientRate, 
                                 QglReal specularRate, QglReal diffuseRate, QglReal eta);
QGL_EXPORT void Qgl_SetPointLightEasy (QglObjHandle hObj, QglReal r, QglReal g, QglReal b);
QGL_EXPORT void Qgl_SetPointLight (QglObjHandle hObj, QglReal rs, QglReal gs, QglReal bs,
                                   QglReal rd, QglReal gd, QglReal bd);


/* .... Debugging */
#include <qul/debug/trace_intf.h>


#ifdef __cplusplus
}
#endif


#endif	/* ifndef _qgl_H */

/*------------------------------ End of Module -------------------------------*/

