/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : xxx.cpp
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
 *
 ******************************************************************************/

/*--------------------------------- Includes ---------------------------------*/

#include "camera.h"

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*---------------------------------- Types -----------------------------------*/

typedef Camera::Screen      Screen;
typedef Camera::Viewer      Viewer;
typedef Camera::TranMode    TranMode;
typedef Camera::Frustum     Frustum;


/*-------------------------------- Constants ---------------------------------*/

/*-------------------------------- Variables ---------------------------------*/

/*----------------------------- Member Functions -----------------------------*/

void Camera::Viewer::Set (const Vector3f &_eye, const Vector3f &_z, Dim tilt)
{
    Vector3f lx, ly, lz;
    Matrix3f rotator;
    if (_z[Vector3f::k_x] == 0 && _z[Vector3f::k_y] == 0)
    {
        if (_z[Vector3f::k_z] > 0)
        {
            lx.Set(1, 0, 0);
            ly.Set(0, 1, 0);
            lz.Set(0, 0, 1);
        }
        else    /* _z[Vector3f::k_z] < 0 */
        {
            lx.Set(1, 0, 0);
            ly.Set(0, -1, 0);
            lz.Set(0, 0, -1);
        }
        lz.GetRotator(tilt, rotator);
    }
    else
    {
        lz.Set(0, 0, 1);    /* z of the scene */

        _z.GetRotator(tilt, rotator);

        lx = _z * lz;
        lx.Unitize();

        lz = _z;
        lz.Unitize();

        ly = lz * lx;
    }
    lx *= rotator;
    ly *= rotator;
    Set(_eye, lx, ly, lz);
}

Camera::Camera ()
{
    InitTranPairs();
    SetToDefaultProj();
}

Camera::~Camera ()
{
}

/*******************************************************************************
 * Prototype    : void Camera::SetScreen (const Screen &screen)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
void Camera::SetScreen (const Screen &screen)
{
    bool bChanged = m_Screen != screen;

    m_Screen = screen;

    if (bChanged)
    {
        /* Change on screen only affects perspective transformation */
        DerivePerspectiveFromScreen();
    }
}

/*******************************************************************************
 * Prototype    : const void Camera::GetScreen () const
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
const Screen & Camera::GetScreen () const
{
    return m_Screen;
}

/*******************************************************************************
 * Prototype    : void Camera::SetViewer (const Viewer &viewer)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
void Camera::SetViewer (const Viewer &viewer)
{
    bool bChanged = m_Viewer != viewer;

    m_Viewer = viewer;
    m_Viewer.Regularize();

    if (bChanged)
    {
        UpdateOrthographic();
        DerivePerspectiveFromScreen();
    }
}

/*******************************************************************************
 * Prototype    : const void Camera::GetViewer () const
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
const Viewer & Camera::GetViewer () const
{
    return m_Viewer;
}

/*******************************************************************************
 * Prototype    : void Camera::UpdateOrthographic ()
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
void Camera::UpdateOrthographic ()
{
    const Vector3f &x = m_Viewer.x;
    const Vector3f &y = m_Viewer.y;
    const Vector3f &z = m_Viewer.z;
    const Vector3f &eye = m_Viewer.eye;

    Dim k_Forward[] = {
          x[0],     y[0],     z[0],   eye[0],
          x[1],     y[1],     z[1],   eye[1],
          x[2],     y[2],     z[2],   eye[2],
        (Dim)0,   (Dim)0,   (Dim)0,   (Dim)1
    };
    

    Vector3fAffineTransformPair *pAffinePair = 
        m_TranPair[k_TmOrthographic]->PtrCast<Vector3fAffineTransformPair *>();

    Matrix3f    temp(k_Forward);
    pAffinePair->SetForward(temp);
    
    temp.inverse();
    pAffinePair->SetBackward(temp);

}

/*******************************************************************************
 * Prototype    : void Camera::SetPerspectiveByFrustum (const Frustum &frustum)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : Use pseudo distance mapping
 ******************************************************************************/
void Camera::SetPerspectiveByFrustum (const Frustum &frustum)
{
    _ElemCref x0 = frustum.x0;
    _ElemCref y0 = frustum.y0;
    _ElemCref px0 = frustum.px0;
    _ElemCref py0 = frustum.py0;

    _ElemCref x1 = frustum.x1;
    _ElemCref y1 = frustum.y1;
    _ElemCref px1 = frustum.px1;
    _ElemCref py1 = frustum.py1;

    _ElemCref z0 = frustum.z0;
    _ElemCref z1 = frustum.z1;
    _ElemCref pz0 = frustum.pz0;
    _ElemCref pz1 = frustum.pz1;

    Dim kx = z0 * (px1 - px0) / (x1 - x0);
    Dim dx = (x1 * px0 - x0 * px1) / (x1 - x0);
    Dim ky = z0 * (py1 - py0) / (y1 - y0);
    Dim dy = (y1 * py0 - y0 * py1) / (y1 - y0);
    Dim kz = z0 * z1 * (pz0 - pz1) / (z1 - z0);
    Dim dz = (pz1 * z1 - pz0 * z0) / (z1 - z0);

    Dim k_Frustum[] = {
        kx, 0,  dx, 0,
        0,  ky, dy, 0,
        0,  0,  dz, kz,
        0,  0,  1,  0
    };


    Vector3fAffineTransformPair *pPerspectiveAffine = 
        m_TranPair[k_TmPerspective]->PtrCast<Vector3fAffineTransformPair *>();

    Vector3fAffineTransformPair *pFrustumicAffine = 
        m_TranPair[k_TmFrustumic]->PtrCast<Vector3fAffineTransformPair *>();

    Vector3fAffineTransformPair *pOrthographicAffine = 
        m_TranPair[k_TmOrthographic]->PtrCast<Vector3fAffineTransformPair *>();

    Matrix3f    temp(k_Frustum);
    pFrustumicAffine->SetBackward(temp);
    temp.inverse();
    pFrustumicAffine->SetForward(temp);

    temp.product(pFrustumicAffine->GetBackward(), pOrthographicAffine->GetBackward());
        /* perspective reverse transformer right multiplied 
         * by scenic transformer forms the final reverse transformer
         * from scene to camera.
         */

    pPerspectiveAffine->SetBackward(temp);
    temp.inverse();
    pPerspectiveAffine->SetForward(temp);

}

/*******************************************************************************
 * Prototype    : void Camera::DerivePerspectiveFromScreen (_ElemCref pz0, 
 *                                                          _ElemCref z1, 
 *                                                          _ElemCref pz1)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
void Camera::DerivePerspectiveFromScreen (_ElemCref pz0, _ElemCref z1, _ElemCref pz1)
{
    Frustum frustum;

    frustum.px0 = frustum.x0 = m_Screen.x0;
    frustum.py0 = frustum.y0 = m_Screen.y0;
    frustum.px1 = frustum.x1 = m_Screen.x1;
    frustum.py1 = frustum.y1 = m_Screen.y1;

    frustum.z0 = m_Screen.d;
    frustum.z1 = z1;

    frustum.pz0 = pz0;
    frustum.pz1 = pz1;

    SetPerspectiveByFrustum(frustum);
}

#define k_FarScrRate    (Dim)5.0
#define k_FarZ          (Dim)1.0

void Camera::DerivePerspectiveFromScreen ()
{
    Dim pz0 = 0;
    Dim z1 = m_Screen.d * k_FarScrRate;
    Dim pz1 = k_FarZ;
    
    DerivePerspectiveFromScreen(pz0, z1, pz1);
}

/*******************************************************************************
 * Prototype    : void Camera::DeriveScreenFromPerspective ()
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : Only derives perpendicular distance from eye to screen.
 *                The dimensions of the screen is not changed as it is not
 *                determined by perspective transform.
 *                The derivation's validity (distorted or not) depends on 
 *                whether `kx == ky'
 ******************************************************************************/
void Camera::DeriveScreenFromPerspective ()
{
    const Vector3fAffineTransformPair *pAffinePair = 
        m_TranPair[k_TmFrustumic]->PtrCast<Vector3fAffineTransformPair *>();

    m_Screen.d = pAffinePair->GetBackward()[0][0];

}

/*******************************************************************************
* Prototype    : void Camera::InitTranPairs ()
* Description  : N/A
* Dependency   : N/A
* Arguments    : 
*  name        type            dir     description
*  ----        ----            ---     -----------
*
* Return Value : N/A
* Annotation   : N/A
******************************************************************************/
void Camera::InitTranPairs ()
{
    m_TranPair[k_TmOrthographic] = new OrthographicVec3TransformPair();
    m_TranPair[k_TmPerspective] = new PerspectiveVec3TransformPair();
    m_TranPair[k_TmFrustumic] = new Vector3fAffineTransformPair();

}

/*******************************************************************************
* Prototype    : void Camera::DeinitTranPairs ()
* Description  : N/A
* Dependency   : N/A
* Arguments    : 
*  name        type            dir     description
*  ----        ----            ---     -----------
*
* Return Value : N/A
* Annotation   : N/A
******************************************************************************/
void Camera::DeinitTranPairs ()
{
    delete m_TranPair[k_TmOrthographic];
    delete m_TranPair[k_TmPerspective];
    delete m_TranPair[k_TmFrustumic];
}

/*******************************************************************************
 * Prototype    : void Camera::SetToDefaultProj ()
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
void Camera::SetToDefaultProj ()
{
    /* .. Set viewer for orthographic projection */
    m_Viewer.eye.Set((Dim)6, 0, 0);
    m_Viewer.z.Set((Dim)(-1), 0, 0);
    m_Viewer.x.Set(0, (Dim)1, 0);
    m_Viewer.y.Set(0, 0, (Dim)1);

    m_Screen.d  = (Dim)1;
    m_Screen.x0 = (Dim)(-1);
    m_Screen.y0 = (Dim)(-1);
    m_Screen.x1 = (Dim)1;
    m_Screen.y1 = (Dim)1;

    UpdateOrthographic();

    /* .. Set perspective */
    DerivePerspectiveFromScreen();

}


/*------------------------- Static Member Functions --------------------------*/



_QGL_END

/*------------------------------ End of Module -------------------------------*/
