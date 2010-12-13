#include "raytrace_renderer.h"
#include "screen.h"


_QGL_BEGIN

RaytraceRenderer::RaytraceRenderer ()
{
    m_TotalDepth = 5;
}

RaytraceRenderer::~RaytraceRenderer ()
{

}


void RaytraceRenderer::Render ()
{
    Dim width, height;
    m_pScreen->GetDims(width, height);

    RectScreen::Style *pStyle = RectScreen::CreateStyle();

    int iWidth = (int)width;
    int iHeight = (int)height;

    for (int y = 0; y < iHeight; y++)
    {
        Dim fy = (Dim)y;
        for (int x = 0; x < iWidth; x++)
        {
            Dim fx = (Dim)x;
            Ray ray;

            /* 
             * Ray-trace algorithm, using [:fx:], [:fy:]
             * ...
             */

            m_pScreen->CreateRay(fx, fy, ray);

            Light                   light;
            RectScreen::PixelColor   color;

            /* Ray-tracing */
            m_pScene->Trace(this, ray, 0, light);

            /* TODO: Filtering of screen power receiving scheme */
            /* ... */

            light.ToPixelColor(color);
            pStyle->SetPixelColor(color);
            m_pScreen->PutPixel(pStyle, x, y);
        }
    }

    RectScreen::DestroyStyle(pStyle);
}

_QGL_END

