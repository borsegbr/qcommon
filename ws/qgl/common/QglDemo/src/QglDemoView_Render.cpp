#include "stdafx.h"

#include <math.h>


#include "..\QglDemo.h"

#include "..\QglDemoDoc.h"
#include "..\QglDemoView.h"
#include "..\qgldemoview.h"


static void AngleToDir (QglReal theta, QglReal psy, QglReal &x, QglReal &y, QglReal &z)
{
    x = cos(psy) * cos(theta);
    y = cos(psy) * sin(theta);
    z = sin(psy);
}

void CQglDemoView::SetupCamSystem ()
{
    m_CamWidth = 12.0f;
    m_CamHeight = 12.0f * m_DispHeight / m_DispWidth;
    m_CamD = 5;
    m_TraceDepth = 5;

    Qgl_SetBuffer(m_hQgl, m_DispBuf, m_DispDataWidth, m_DispDataWidth * m_DispHeight);

#if 0	// a
    Qgl_SetCameraPose(m_hQgl, 5, 5, 0, -1, -1, 0, 0);
#endif
#if 0	// b
    Qgl_SetCameraPose(m_hQgl, 6, 0, 0, -1, 0, 0, 0);
#endif
#if 1
    Qgl_SetCameraPose(m_hQgl, 2, -5, 0, 0, 1, 0, 0);
#endif

    Qgl_SetCameraScr(m_hQgl, m_CamWidth, m_CamHeight, m_CamD);
    Qgl_SetScreen(m_hQgl, (QglReal)m_DispWidth, (QglReal)m_DispHeight);
    Qgl_SetProjMode(m_hQgl, k_QglPerspective);
    Qgl_SetTraceDepth(m_hQgl, m_TraceDepth);

}

void CQglDemoView::Move (MoveDir dir)
{
    QglReal v = (QglReal)0.2;
    QglReal x, y, z;
    QglReal ax, ay, az;
    Qgl_GetCameraPos(m_hQgl, &x, &y, &z);
    switch (dir)
    {
    case k_Forward:
        Qgl_GetCameraAxis(m_hQgl, k_CamZ, &ax, &ay, &az);
        x += ax * v;
        y += ay * v;
        z += az * v;
        break;
    case k_Backward:
        Qgl_GetCameraAxis(m_hQgl, k_CamZ, &ax, &ay, &az);
        x -= ax * v;
        y -= ay * v;
        z -= az * v;
        break;
    case k_Left:
        Qgl_GetCameraAxis(m_hQgl, k_CamX, &ax, &ay, &az);
        x -= ax * v;
        y -= ay * v;
        z -= az * v;
        break;
    case k_Right:
        Qgl_GetCameraAxis(m_hQgl, k_CamX, &ax, &ay, &az);
        x += ax * v;
        y += ay * v;
        z += az * v;
        break;
    case k_Up:
        Qgl_GetCameraAxis(m_hQgl, k_CamY, &ax, &ay, &az);
        x -= ax * v;
        y -= ay * v;
        z -= az * v;
        break;
    case k_Down:
        Qgl_GetCameraAxis(m_hQgl, k_CamY, &ax, &ay, &az);
        x += ax * v;
        y += ay * v;
        z += az * v;
        break;
    }
    Qgl_SetCameraPos(m_hQgl, x, y, z);
}

void CQglDemoView::RotateUpDown (QglReal dAngle)  // X
{
    QglReal ax, ay, az;
    Qgl_GetCameraAxis(m_hQgl, k_CamX, &ax, &ay, &az);
    Qgl_RotateCamera(m_hQgl, ax, ay, az, dAngle);
}

void CQglDemoView::RotateLeftRight (QglReal dAngle) // y
{
    QglReal ax, ay, az;
    Qgl_GetCameraAxis(m_hQgl, k_CamY, &ax, &ay, &az);
    Qgl_RotateCamera(m_hQgl, ax, ay, az, dAngle);
}

void CQglDemoView::RotateLensAxis (QglReal dAngle)   // Z
{
    QglReal ax, ay, az;
    Qgl_GetCameraAxis(m_hQgl, k_CamZ, &ax, &ay, &az);
    Qgl_RotateCamera(m_hQgl, ax, ay, az, dAngle);
}

void CQglDemoView::CreateDemoScene ()
{
#if 0
    QglObjHandle hSphere = Qgl_AddSphere(m_hQgl, -3, -3, -1, 3);
    QglObjHandle hSphere2 = Qgl_AddSphere(m_hQgl, 4, 4, 4, 3);
#if 0
    Qgl_SetObjOpticEasy(hSphere, 0.1, 1, 0.1, 0, 0);
    Qgl_SetObjOptic(hSphere2, 1, 1, 1, 0.8, 0, 0.8, 0.0, 1.2);

    QglObjHandle hLight = Qgl_AddPointLight(m_hQgl, 9, -2, 1);
    QglObjHandle hLight2 = Qgl_AddPointLight(m_hQgl, 9, 9, 0);
    QglObjHandle hLight3 = Qgl_AddPointLight(m_hQgl, -10, -10, -12);
    QglObjHandle hLight4 = Qgl_AddPointLight(m_hQgl, 7, 4, 0);
    Qgl_SetPointLightEasy(hLight, 1, 1, 1);
    Qgl_SetPointLightEasy(hLight2, 1, 1, 1);
    Qgl_SetPointLightEasy(hLight3, 1, 1, 1);
#endif
#endif


#if 0	// A
    m_Objects[0] = Qgl_CreateSphere((QglReal)0, (QglReal)0, (QglReal)0, (QglReal)3);
    m_Objects[1] = Qgl_CreateSphere((QglReal)0, (QglReal)9, (QglReal)0, (QglReal)3);
    m_Objects[2] = Qgl_CreateSphere((QglReal)9, (QglReal)0, (QglReal)0, (QglReal)3);
    m_Objects[3] = Qgl_CreateSphere((QglReal)9, (QglReal)9, (QglReal)0, (QglReal)3);

    m_ObjNum = 4;

    for (int i = 0; i < m_ObjNum; i++)
    {
        Qgl_AddShape(m_hQgl, m_Objects[i]);
    }
    QglObjHandle hLight1 = Qgl_AddPointLight(m_hQgl, (QglReal)18, (QglReal)18, (QglReal)18);
    Qgl_SetPointLightEasy(hLight1, 1, 1, 1);
#endif
#if 0	// B
    QglObjHandle    hL = Qgl_CreateSphere((QglReal)0, (QglReal)0, (QglReal)0, (QglReal)3);
    QglObjHandle    hR = Qgl_CreateSphere((QglReal)1, (QglReal)2, (QglReal)0, (QglReal)3);
    m_Objects[0] = Qgl_CreateCsg(hL, hR, k_Subtraction);
    //m_Objects[1] = Qgl_CreateSphere((QglReal)9, (QglReal)0, (QglReal)0, (QglReal)3);
    m_ObjNum = 1;

    for (int i = 0; i < m_ObjNum; i++)
    {
        Qgl_AddShape(m_hQgl, m_Objects[i]);
    }
    QglObjHandle hLight1 = Qgl_AddPointLight(m_hQgl, (QglReal)18, (QglReal)18, (QglReal)18);
    Qgl_SetPointLightEasy(hLight1, (QglReal)1, (QglReal)1, (QglReal)1);
#endif
#if 1	// C
	QglObjHandle    hL = Qgl_CreateSphere((QglReal)0, (QglReal)0, (QglReal)0, (QglReal)3);
    QglObjHandle    hR = Qgl_CreateSphere((QglReal)4, (QglReal)0, (QglReal)0, (QglReal)3);
    m_Objects[0] = Qgl_CreateCsg(hL, hR, k_Subtraction);
    m_ObjNum = 1;
    Qgl_SetObjOpticEasy(hL, (QglReal)0.1, (QglReal)1, (QglReal)0.1, (QglReal)0, (QglReal)0);
    Qgl_SetObjOptic(hR, (QglReal)0.1, (QglReal)1, (QglReal)0.1, (QglReal)0, 
        (QglReal)0, (QglReal)0.9, (QglReal)0.1, (QglReal)0);
    //Qgl_SetObjOptic(hSphere2, 1, 1, 1, 0.8, 0, 0.8, 0.0, 1.2);

    for (int i = 0; i < m_ObjNum; i++)
    {
        Qgl_AddShape(m_hQgl, m_Objects[i]);
    }
    //QglObjHandle hLight1 = Qgl_AddPointLight(m_hQgl, 2, -5, 0);
    //Qgl_SetPointLightEasy(hLight1, 1, 1, 1);
    QglObjHandle hLight1 = Qgl_AddPointLight(m_hQgl, (QglReal)(2-10), (QglReal)0, (QglReal)0);
    Qgl_SetPointLightEasy(hLight1, (QglReal)1, (QglReal)1, (QglReal)1);
    QglObjHandle hLight2 = Qgl_AddPointLight(m_hQgl, (QglReal)(2+10), (QglReal)0, (QglReal)0);
    Qgl_SetPointLightEasy(hLight2, (QglReal)1, (QglReal)1, (QglReal)1);
#endif

}

void CQglDemoView::DrawWireframe ()
{
#define MAX_DISP_WIDTH		1280
#define MAX_DISP_HEIGHT		800
#define BMP_BPP				4
#define MAX_DISP_BUF_SIZE	(MAX_DISP_WIDTH*MAX_DISP_HEIGHT*BMP_BPP)

    memset(m_DispBuf, 0, MAX_DISP_BUF_SIZE);

    for (int i = 0; i < m_ObjNum; i++)
    {
        Qgl_DrawWireframe(m_hQgl, m_Objects[i], 0x007F7F7F);
    }
    
}

void CQglDemoView::RaytraceRender ()
{
    Qgl_RaytraceRender(m_hQgl);
}

/*
 * Tests:
 * aB
 * bC
 */
