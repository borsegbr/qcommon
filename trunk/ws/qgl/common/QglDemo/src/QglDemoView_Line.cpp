#include "stdafx.h"
#include "..\QglDemo.h"

#include "..\QglDemoDoc.h"
#include "..\QglDemoView.h"
#include "..\qgldemoview.h"



void CQglDemoView::LineFill (void *data, int x, int y)
{
	CQglDemoView *pView = (CQglDemoView *)data;
	int pos = y * pView->m_DispDataWidth + x * 4;

	pView->m_DispBuf[pos++] = 0x00;
	pView->m_DispBuf[pos++] = 0xFF;
	pView->m_DispBuf[pos++] = 0x00;
	pView->m_DispBuf[pos] = 0xFF;
}

void CQglDemoView::PixelFill (void *data, int x, int y)
{
	CQglDemoView *pView = (CQglDemoView *)data;
	int pos = y * pView->m_DispDataWidth + x * 4;

	pView->m_DispBuf[pos++] = 0xFF;
	pView->m_DispBuf[pos++] = 0xFF;
	pView->m_DispBuf[pos++] = 0xFF;
	pView->m_DispBuf[pos] = 0xFF;
}

void CQglDemoView::SeedFillSet (void *data, int x, int y, int chnl, int val)
{
	CQglDemoView *pView = (CQglDemoView *)data;

	/* Draw */
	int pos = y * pView->m_DispDataWidth + x * 4;

	pView->m_DispBuf[pos++] = 0xFF;
	pView->m_DispBuf[pos++] = 0xFF;
	pView->m_DispBuf[pos++] = 0xFF;
	pView->m_DispBuf[pos] = 0xFF;


	int width = pView->m_SfillBmp.right - pView->m_SfillBmp.left;
	x -= pView->m_SfillBmp.left;
	y -= pView->m_SfillBmp.top;

	/* Always [:k_QglSeedSetFill:] */
	pView->m_SfillBmp.data[y * width + x] = 2;


}

int CQglDemoView::SeedFillGet (void *data, int x, int y, int chnl)
{
	CQglDemoView *pView = (CQglDemoView *)data;
	int width = pView->m_SfillBmp.right - pView->m_SfillBmp.left;
	x -= pView->m_SfillBmp.left;
	y -= pView->m_SfillBmp.top;

	switch (chnl)
	{
	case k_QglSeedGetEither:
		return (pView->m_SfillBmp.data[y * width + x] != 0);
	case k_QglSeedGetBorder:
		return (pView->m_SfillBmp.data[y * width + x] == 1);
	}
	return 0;
}

const BYTE X = 1;

BYTE k_Buf[] = 
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,X,X,X,X,0,0,X,X,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,X,0,0,0,0,X,0,X,0,X,X,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,X,0,0,X,0,0,0,0,0,X,X,0,0,0,X,0,0,0,0,0,X,X,X,X,X,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,X,0,X,X,X,0,0,X,0,0,0,0,0,0,0,X,0,0,0,0,X,0,0,0,0,0,X,0,0,0,0,0,
	0,0,0,0,0,0,0,X,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,X,0,0,0,X,0,0,0,0,0,0,0,X,0,0,0,0,
	0,0,0,0,0,0,0,X,0,0,0,0,0,0,X,X,X,0,0,0,0,0,0,0,X,0,X,0,0,0,0,0,0,0,X,0,0,0,0,0,
	0,0,0,0,0,0,X,0,0,0,0,0,0,X,0,0,X,0,0,0,0,X,0,0,X,X,0,0,0,0,0,0,0,0,X,0,0,0,0,0,
	0,0,0,0,0,X,0,0,X,X,X,0,0,X,0,0,0,X,0,0,X,0,X,0,X,0,X,0,0,0,0,0,0,0,0,X,X,0,0,0,
	0,0,0,0,X,0,0,0,X,0,X,0,X,0,0,0,0,X,0,0,X,0,X,0,X,0,X,0,0,0,0,X,0,0,0,0,X,0,0,0,
	0,0,0,0,0,X,0,0,X,X,X,0,X,0,0,0,0,X,0,0,X,0,X,0,X,0,0,X,0,0,0,0,0,0,0,0,X,0,0,0,
	0,0,0,0,0,0,X,0,0,0,0,0,0,X,0,0,0,X,0,0,0,X,X,0,X,0,0,0,X,0,0,0,0,0,0,0,X,0,0,0,
	0,0,0,0,0,0,0,X,0,0,0,0,0,X,0,0,0,X,0,0,0,0,0,0,X,0,0,0,X,0,0,0,X,0,0,0,X,0,0,0,
	0,0,0,0,0,0,0,X,0,0,0,0,0,0,X,X,X,0,0,0,0,0,0,0,X,0,0,0,X,0,0,0,X,X,0,0,X,0,0,0,
	0,0,0,0,0,0,X,0,0,0,0,0,0,0,0,0,0,0,X,X,X,0,0,0,X,0,0,0,X,0,0,0,X,0,X,X,0,0,0,0,
	0,0,0,0,0,X,0,0,0,0,0,0,0,0,0,0,0,X,0,0,X,0,0,0,X,0,0,0,X,0,0,0,X,0,0,0,0,0,0,0,
	0,0,0,0,0,X,0,0,0,X,X,0,0,0,0,0,X,0,0,0,X,0,0,0,0,X,X,X,0,0,0,0,X,0,0,0,0,0,0,0,
	0,0,0,0,0,X,0,0,0,X,X,0,0,0,0,0,X,0,0,0,0,X,X,0,0,0,0,0,0,0,0,0,X,0,0,0,0,0,0,0,
	0,0,0,0,X,0,0,0,0,0,0,0,0,0,0,X,0,0,0,0,0,0,X,0,0,0,0,0,0,0,0,X,0,0,0,0,0,0,0,0,
	0,0,0,X,0,0,0,X,0,0,0,0,0,0,0,X,0,0,0,0,0,0,0,X,0,0,0,0,0,0,X,0,0,0,0,0,0,0,0,0,
	0,0,0,0,X,0,X,0,X,0,0,0,0,0,X,0,0,0,0,0,0,0,0,0,X,X,X,X,X,X,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,X,X,0,0,X,X,X,X,X,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

void CQglDemoView::DrawRect (QglRect *rect, QglPixie *pixie)
{
	Qgl_Line(rect->left, rect->top, rect->right, rect->top, pixie);
	Qgl_Line(rect->left, rect->top, rect->left, rect->bottom, pixie);
	Qgl_Line(rect->left, rect->bottom, rect->right, rect->bottom, pixie);
	Qgl_Line(rect->right, rect->top, rect->right, rect->bottom, pixie);
	
}
void CQglDemoView::SetBorder ()
{
	m_SfillBmp.left = 180;
	m_SfillBmp.right = 220;
	m_SfillBmp.top = 60;
	m_SfillBmp.bottom = 90;
	m_SfillBmp.data = k_Buf;

}

void DrawLine (QglVertex2i *v0, QglVertex2i *v1, QglPixie *pixie)
{
	Qgl_Line(v0->x, v0->y, v1->x, v1->y, pixie);
}


void CQglDemoView::TestPolygon ()
{
	/* Seed filling */
	QglSmartPixie smartPxe;

	SetBorder();
	smartPxe.data = this;
	smartPxe.get = SeedFillGet;
	smartPxe.set = SeedFillSet;

	Qgl_SeedFill(&smartPxe, 190, 74);


	/* Edge filling */
	//QglVertex2i vertices[] = {
	//	{23, 12}, {11, 20}, {15, 43}, {35, 43}, {42, 17}, {0, 50}
	//};
    QglVertex2i vertices[] = {
    	{369, 336}, {333, 360}, {345, 429}, {405, 429}, {426, 351}, {300, 450}
    };
	QglPolygoni polygon;
	polygon.vertices = vertices;
	polygon.count = 5;


    QglPixie pixie;

	pixie.set = LineFill;
	pixie.data = this;
	DrawLine(vertices+0, vertices+1, &pixie);
	DrawLine(vertices+1, vertices+2, &pixie);
	DrawLine(vertices+2, vertices+3, &pixie);
	DrawLine(vertices+3, vertices+4, &pixie);
	DrawLine(vertices+4, vertices+0, &pixie);

    pixie.set = PixelFill;
    pixie.data = this;
    Qgl_PolygonFill(&polygon, &pixie);

	/* Round and Ellipse */
	QglVertex2i center = {70, 71};
	Qgl_Circle(center.x, center.y, 23, &pixie);

	center.x += 130;
	center.y += 120;
	Qgl_Ellipse(center.x, center.y, 30, 40, &pixie);
		
}


void CQglDemoView::TestClip ()
{

	QglFloat left = 50;
	QglFloat top = 100;
	QglFloat right = 150;
	QglFloat bottom = 150;

	QglFloat x0 = 40;
	QglFloat y0 = 70;
	QglFloat x1 = 200;
	QglFloat y1 = 190;

	QglBool inside = Qgl_LineClip(&x0, &y0, &x1, &y1, left, right, top, bottom);

	QglVertex2i v0, v1;
	v0.x = (QglInt)x0;
	v0.y = (QglInt)y0;
	v1.x = (QglInt)x1;
	v1.y = (QglInt)y1;

	QglPixie pixie;
	pixie.set = LineFill;
	pixie.data = this;

	QglRect rect;
	rect.left = (QglInt)left;
	rect.right = (QglInt)right;
	rect.top = (QglInt)top;
	rect.bottom = (QglInt)bottom;
	DrawRect(&rect, &pixie);

	if (inside)
	{
		DrawLine(&v0, &v1, &pixie);
	}
	
	/* Polygon Clipping */
	QglVertex2f vertices[] = {
		{left - 10, top - 20},
		{left + 20, top + 40},
		{left - 12, bottom + 20},
		{left + 40, bottom + 30},
		{right + 10, bottom - 2},
		{right - 2, bottom -34},
	};
	QglPolygonf polygon;
	polygon.vertices = vertices;
	polygon.count = 6;

	QglPolygonf polygonOut;
	QglVertex2f polyVtx[12];
	polygonOut.vertices = polyVtx;
	polygonOut.count = 0;

	Qgl_PolygonClip(&polygon, &polygonOut, left, right, top, bottom);

	QglPolygoni polyView;
	QglVertex2i polyViewVtx[12];
	polyView.vertices = polyViewVtx;

	polyView.count = polygonOut.count;
	for (int i = 0; i < polygonOut.count; i++)
	{
		polyView.vertices[i].x = (QglInt)polygonOut.vertices[i].x;
		polyView.vertices[i].y = (QglInt)polygonOut.vertices[i].y;
	}

	pixie.set = PixelFill;
	pixie.data = this;

	Qgl_PolygonFill(&polyView, &pixie);
}



