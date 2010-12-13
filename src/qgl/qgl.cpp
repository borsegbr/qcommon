#include "qgl.h"

#include "screen.h"
#include "camera.h"
#include "vector3d.h"
#include "sphere.h"
#include "simple_scene.h"
#include "simple_ether.h"
#include "renderer.h"
#include "point_light.h"

using namespace qgl;

struct OpaqItr
{
    QglUint8    occupied[sizeof(Itr)];
};

struct QglStruct
{
    QglUint8    *buffer;
    QglInt      bufByteWidth, bufSize;

    RectScreen           *pScreen;
    Camera              *pCamera;
    SimpleScene         *pScene;
    RaytraceRenderer    *pRenderer;
    SimpleEther         *pEther;

    QglStruct () : buffer(NULL)
    {
        pScreen = new RectScreen;
        pCamera = new Camera;
        pScene = new SimpleScene;
        pEther = new SimpleEther(pScene);
        pRenderer = new RaytraceRenderer;
        
        pScene->SetEther(pEther);

        pScreen->AttachCamera(pCamera);

        pRenderer->SetScreen(pScreen);
        pRenderer->SetScene(pScene);

    }

    ~QglStruct ()
    {
        DisposeScene();

        if (pRenderer)
        {
            delete pRenderer;
        }

        if (pScreen)
        {
            delete pScreen;
        }

        if (pCamera)
        {
            delete pCamera;
        }

        if (pScene)
        {
            delete pScene;
        }

    }

    void DisposeScene ()
    {
        Itr itr;

        ListEnumerator &pointLights = pScene->GetPointLights();
        for (pointLights.GetFirst(itr); itr->IsValid(); pointLights.GetNext(itr))
        {
            PointLight *p = itr->GetDynPtr<PointLight *>();
            delete p;
        }

        ListEnumerator &opticals = pScene->GetOpticals();
        for (opticals.GetFirst(itr); itr->IsValid(); opticals.GetNext(itr))
        {
            Optical *p = itr->GetDynPtr<Optical *>();
            delete p;
        }
    }

};


/* Main body */

QglHandle Qgl_CreateHandle (void)
{
    QglStruct *p = new QglStruct;

    QglHandle h = (QglHandle)p;
    
    return h;
}

void Qgl_DestroyHandle (QglHandle h)
{
    QglStruct *p = (QglStruct *)h;

    delete p;
}

void Qgl_SetBuffer (QglHandle h, QglUint8 *buffer, QglInt bufByteWidth, QglInt bufSize)
{
    QglStruct *p = (QglStruct *)h;

    p->buffer = buffer;
    p->bufByteWidth = bufByteWidth;
    p->bufSize = bufSize;

}

/* Screen */

void Qgl_SetProjMode (QglHandle h, ProjMode mode)
{
    QglStruct *p = (QglStruct *)h;
    RectScreen *pScreen = p->pScreen;

    pScreen->SetProjMode((RectScreen::ProjMode)mode);
}

void Qgl_SetCameraPose (QglHandle h, QglReal x, QglReal y, QglReal z, QglReal ax, QglReal ay, QglReal az, QglReal tilt)
    /* rotate by [:tilt:] counter-clockwise viewed from photographer  */
{
    QglStruct *p = (QglStruct *)h;
    Camera *pCamera = p->pCamera;
    RectScreen *pScreen = p->pScreen;

    Vector3f    eye(x, y, z);
    Vector3f    vz(ax, ay, az);
    Camera::Viewer viewer(eye, vz, tilt);

    pCamera->SetViewer(viewer);
    pScreen->ConformWithCamera();
}

void Qgl_GetCameraPos (QglHandle h, QglReal *pX, QglReal *pY, QglReal *pZ)
{
    QglStruct *p = (QglStruct *)h;
    Camera *pCamera = p->pCamera;
    Camera::Viewer  viewer = pCamera->GetViewer();

    *pX = viewer.eye[Vector3f::k_x];
    *pY = viewer.eye[Vector3f::k_y];
    *pZ = viewer.eye[Vector3f::k_z];

}

void Qgl_SetCameraPos (QglHandle h, QglReal x, QglReal y, QglReal z)
{
    QglStruct *p = (QglStruct *)h;
    Camera *pCamera = p->pCamera;
    RectScreen *pScreen = p->pScreen;
    Camera::Viewer  viewer = pCamera->GetViewer();

    viewer.eye[Vector3f::k_x] = x;
    viewer.eye[Vector3f::k_y] = y;
    viewer.eye[Vector3f::k_z] = z;

    pCamera->SetViewer(viewer);
    pScreen->ConformWithCamera();
}

void Qgl_GetCameraAxis (QglHandle h, CamAxis axis, QglReal *pX, QglReal *pY, QglReal *pZ)
{
    QglStruct *p = (QglStruct *)h;
    Camera *pCamera = p->pCamera;
    RectScreen *pScreen = p->pScreen;
    Camera::Viewer  viewer = pCamera->GetViewer();

    switch (axis)
    {
    case k_CamX:
        *pX = viewer.x[Vector3f::k_x];
        *pY = viewer.x[Vector3f::k_y];
        *pZ = viewer.x[Vector3f::k_z];
        break;
    case k_CamY:
        *pX = viewer.y[Vector3f::k_x];
        *pY = viewer.y[Vector3f::k_y];
        *pZ = viewer.y[Vector3f::k_z];
        break;
    case k_CamZ:
        *pX = viewer.z[Vector3f::k_x];
        *pY = viewer.z[Vector3f::k_y];
        *pZ = viewer.z[Vector3f::k_z];
        break;
    }
}

void Qgl_SetCameraAxis (QglHandle h, CamAxis axis, QglReal x, QglReal y, QglReal z)
{
    QglStruct *p = (QglStruct *)h;
    Camera *pCamera = p->pCamera;
    RectScreen *pScreen = p->pScreen;
    Camera::Viewer  viewer = pCamera->GetViewer();

    switch (axis)
    {
    case k_CamX:
        viewer.x[Vector3f::k_x] = x;
        viewer.x[Vector3f::k_y] = y;
        viewer.x[Vector3f::k_z] = z;
        break;
    case k_CamY:
        viewer.y[Vector3f::k_x] = x;
        viewer.y[Vector3f::k_y] = y;
        viewer.y[Vector3f::k_z] = z;
        break;
    case k_CamZ:
        viewer.z[Vector3f::k_x] = x;
        viewer.z[Vector3f::k_y] = y;
        viewer.z[Vector3f::k_z] = z;
        break;
    }

    pCamera->SetViewer(viewer);
    pScreen->ConformWithCamera();
}

void Qgl_SetCameraScr (QglHandle h, QglReal width, QglReal height, QglReal d)
{
    QglReal halfWidth = width / 2;
    QglReal halfHeight = height / 2;

    QglStruct *p = (QglStruct *)h;
    Camera *pCamera = p->pCamera;

    Camera::Screen pScreen(-halfWidth, -halfHeight, halfWidth, halfHeight, d);

    pCamera->SetScreen(pScreen);

}

void Qgl_RotateCamera (QglHandle h, QglReal ax, QglReal ay, QglReal az, QglReal alpha)
{
    QglStruct *p = (QglStruct *)h;
    Camera *pCamera = p->pCamera;
    RectScreen *pScreen = p->pScreen;

    Camera::Viewer  viewer = pCamera->GetViewer();

    Vector3f    axis(ax, ay, az);
    viewer.Rotate (axis, alpha);

    pCamera->SetViewer(viewer);
    pScreen->ConformWithCamera();
}

void Qgl_TurnUprightCamera (QglHandle h)
{
    QglStruct *p = (QglStruct *)h;
    Camera *pCamera = p->pCamera;
    RectScreen *pScreen = p->pScreen;

    Camera::Viewer  viewer = pCamera->GetViewer();

    viewer.TurnUpright();

    pCamera->SetViewer(viewer);
    pScreen->ConformWithCamera();
}

void Qgl_SetScreen (QglHandle h, QglReal width, QglReal height)
{
    QglStruct *p = (QglStruct *)h;
    RectScreen *pScreen = p->pScreen;

    pScreen->SetDims(width, height);
    
    QglUint8 colorPattern[] = {2, 1, 0, 3};
    pScreen->SetImageBuf(p->buffer, p->bufByteWidth, colorPattern);
}

void Qgl_Clear (QglHandle h)
{

}

/* Wireframe */

void Qgl_DrawWireframe (QglHandle h, QglObjHandle hObj, QglColorRef color)
{
    QglStruct           *p = (QglStruct *)h;
    RectScreen           *pScreen = p->pScreen;

    Object              *pObj = reinterpret_cast<Object *>(hObj);
    WireframedShape		*pShape = dynamic_cast<WireframedShape *>(pObj);

    if (pShape)
    {
        QglUint8 r = (QglUint8)((color >> 16) & 0xff);
        QglUint8 g = (QglUint8)((color >> 8) & 0xff);
        QglUint8 b = (QglUint8)(color & 0xff);
        SimpleWireframeStyle    style(RectScreen::PixelColor(r, g, b));
        pShape->DrawWireframe(pScreen, &style);
    }
}

/* Ray-trace Rendering */

void Qgl_SetTraceDepth (QglHandle h, int depth)
{
    QglStruct *p = (QglStruct *)h;

    RaytraceRenderer *pRenderer = p->pRenderer;

    pRenderer->SetTotalDepth(depth);
}

void Qgl_RaytraceRender (QglHandle h)
{
    QglStruct *p = (QglStruct *)h;

    RaytraceRenderer *pRenderer = p->pRenderer;

    pRenderer->Render();
    
}


/* .. Objects */

QglObjHandle Qgl_CreateSphere (QglReal cx, QglReal cy, QglReal cz, QglReal r)
{
    CsgSphere       *pSphere = new CsgSphere(cx, cy, cz, r); 
	Object *		pObject = dynamic_cast<Object *>(pSphere);
	CsgShape		*pShape = dynamic_cast<CsgShape *>(pSphere);
	Shape			*ps = dynamic_cast<Shape *>(pSphere);
    return (QglObjHandle)pObject;
}

QglObjHandle Qgl_CreateCsg (QglObjHandle hObjL, QglObjHandle hObjR, QglBindType type)
{
    Object *pObj = reinterpret_cast<Object *>(hObjL);
    CsgShape *pCsgL = dynamic_cast<CsgShape *>(pObj);
    pObj = reinterpret_cast<Object *>(hObjR);
    CsgShape *pCsgR = dynamic_cast<CsgShape *>(pObj);

    CsgUsefulNode *pNode = new CsgUsefulNode(pCsgL, pCsgR, (CsgNode::Operation)type);

    return (QglObjHandle)dynamic_cast<Object *>(pNode);
}

void Qgl_AddShape (QglHandle h, QglObjHandle hObj)
{
    QglStruct       *p = (QglStruct *)h;
    SimpleScene     *pScene = dynamic_cast<SimpleScene *>(p->pScene);
    Object          *pObj = reinterpret_cast<Object *>(hObj);

    Itr itr;
    ListEnumerator &opticals = pScene->GetOpticals();
    opticals.GetFirst(itr);
    opticals.Insert(itr, pObj);
}

QglObjHandle Qgl_AddPointLight (QglHandle h, QglReal cx, QglReal cy, QglReal cz)
{
    QglStruct   *p = (QglStruct *)h;
    SimpleScene *pScene = dynamic_cast<SimpleScene *>(p->pScene);

    Light   light_d, light_s;
    light_d.Set(0.5, 0.5, 0.5);
    light_s.Set(0.5, 0.5, 0.5);

    PointLight  *pPointLight = new PointLight(cx, cy, cz, light_d, light_s);

    Itr itr;
    ListEnumerator &pointLights = pScene->GetPointLights();
    pointLights.GetFirst(itr);
    pointLights.Insert(itr, pPointLight);

    return (QglObjHandle)dynamic_cast<Object *>(pPointLight);

}

void Qgl_RemoveObject (QglHandle h, QglObjHandle hObj)
{
    QglStruct   *p = (QglStruct *)h;
    SimpleScene *pScene = dynamic_cast<SimpleScene *>(p->pScene);

    Itr itr;
    Object *pObj = reinterpret_cast<Object *>(hObj);
    PointLight *pPointLight = dynamic_cast<PointLight *>(pObj);
    if (pPointLight)
    {
        ListEnumerator &pointLights = pScene->GetPointLights();
        pointLights.Find(pPointLight, itr);
        pointLights.Remove(itr);
        delete pPointLight;
        return;
    }
    Optical *pOptical = dynamic_cast<Optical *>(pObj);
    if (pOptical)
    {
        ListEnumerator &opticals = pScene->GetOpticals();
        opticals.Find(pOptical, itr);
        opticals.Remove(itr);
        delete pOptical;
        return;
    }
}

void Qgl_SetObjOpticEasy (QglObjHandle hObj, QglReal r, QglReal g, QglReal b, 
                                     QglReal transparency, QglReal eta)
{
    Qgl_SetObjOptic(hObj, r, g, b, transparency, 0, (Phq)0.5, (Phq)0.5, eta);
}

void Qgl_SetObjOptic (QglObjHandle hObj, QglReal r, QglReal g, QglReal b,
                                 QglReal transparency, QglReal ambientRate, 
                                 QglReal specularRate, QglReal diffuseRate, QglReal eta)
{
    Object          *pObj = reinterpret_cast<Object *>(hObj);
    SimpleOptical   *pOptical = dynamic_cast<SimpleOptical *>(pObj);

    if (!pOptical)
    {
        return;
    }

    ColorComponents    color(r, g, b);

	const Phq		k_AttRate = 0.5;

    // generate the rate of attenuation from diffuse rate

    Phq attenuant = k_AttRate * pow(specularRate / (specularRate + diffuseRate), 2);

    QglReal tg = (transparency) * attenuant;
    QglReal rg = (1 - transparency) * attenuant;
    QglReal ri = 1 * attenuant;
    
    QglReal st = transparency * specularRate;
    QglReal s = (1 - transparency) * specularRate;

    QglReal dt = transparency * diffuseRate;
    QglReal d = (1 - transparency) * diffuseRate ;

    QglReal si = specularRate;
    QglReal di = diffuseRate;

    pOptical->Set(color, s, d, st, dt, si, di, ambientRate, rg, tg, ri, eta);

}

void Qgl_SetPointLightEasy (QglObjHandle hObj, QglReal r, QglReal g, QglReal b)
{
    const QglReal k_SpecularRate = 1.0;
    const QglReal k_DiffuseRate = 1.0;

    Qgl_SetPointLight(hObj, r * k_SpecularRate, g * k_SpecularRate, b * k_SpecularRate, 
        r * k_DiffuseRate, g * k_DiffuseRate, b * k_DiffuseRate);
}

void Qgl_SetPointLight (QglObjHandle hObj, QglReal rs, QglReal gs, QglReal bs,
                                   QglReal rd, QglReal gd, QglReal bd)
{
    Object      *pObj = reinterpret_cast<Object *>(hObj);
    PointLight  *pPointLight = dynamic_cast<PointLight *>(pObj);

    if (!pPointLight)
    {
        return;
    }

    pPointLight->SetLight_s(Light(rs, gs, bs));
    pPointLight->SetLight_d(Light(rd, gd, bd));

}