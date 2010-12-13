#include "./ddrawbase.h"

#pragma comment(lib, "ddraw.lib")

_QUL_BEGIN

DirectDrawUtils::DirectDrawUtils ()
	{
	}

DirectDrawUtils::~DirectDrawUtils ()
	{
	Finalize();
	}

void DirectDrawUtils::InitFullScreen (HWND hwnd)
	{
	m_hwnd = hwnd;

	HRESULT hr;
	hr = DirectDrawCreate(NULL, &m_pDirectDraw, NULL);
	if (FAILED(hr))
		::MessageBox(hwnd, "DirectDrawCreate failed", "DirectDraw error", MB_OK);
	hr = m_pDirectDraw->SetCooperativeLevel(hwnd, DDSCL_FULLSCREEN|DDSCL_EXCLUSIVE);
	if (FAILED(hr))
		DisplayError("SetCooperativeLevel failed");

	SetupPrimarySurface ();

	}

void DirectDrawUtils::SetDisplayMode (DWORD width, DWORD height, DWORD bitsPerPel)
	{
	m_pDirectDraw->SetDisplayMode(width, height, bitsPerPel);
	}

void DirectDrawUtils::SetupPrimarySurface ()
	{
	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;
	HRESULT result = m_pDirectDraw->CreateSurface(&ddsd, &m_pPrimarySurface, NULL);
	if (FAILED(result))
		{
		DisplayError("CreateSurface failed");
		}
	DDSCAPS ddscaps;
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	result = m_pPrimarySurface->GetAttachedSurface(&ddscaps, &m_pAttachedSurface);
	if (FAILED(result))
		{
		DisplayError("GetAttachedSurface failed");
		}
	if (!m_pPrimarySurface)
		{
		DisplayError("Primary NULL");
		}
	if (!m_pAttachedSurface)
		{
		DisplayError("Attached NULL");
		}	
	}

void DirectDrawUtils::DrawBits (int left, int top, int width, int height, int bitDepth, BYTE *pBits)
	{
	HDC hdc;
	m_pPrimarySurface->GetDC(&hdc);
	HBITMAP hbmp = ::CreateBitmap(width, height, 1, bitDepth, pBits);
	HDC hdcComp = ::CreateCompatibleDC(hdc);
	::SelectObject(hdcComp, hbmp);
	::BitBlt(hdc, left, top, width, height, hdcComp, 0, 0, SRCCOPY);
	m_pPrimarySurface->ReleaseDC(hdc);

	// Destroying is very significant when applying to huge image sequences.
	DeleteObject(hbmp);
	DeleteObject(hdcComp);
	}

void DirectDrawUtils::BeginDrawing (HDC *hdc)
	{
	m_pPrimarySurface->GetDC(hdc);
	}

void DirectDrawUtils::EndDrawing (HDC hdc)
	{
	m_pPrimarySurface->ReleaseDC(hdc);
	}

void DirectDrawUtils::DisplayError (const char *error, bool canExit) const
	{
	m_pDirectDraw->FlipToGDISurface();
	::MessageBox(m_hwnd, error, "DirectDraw error", MB_OK);
	if (canExit)
		exit(1);	
	}

void DirectDrawUtils::Finalize ()
	{
	if (m_pPrimarySurface)
		m_pPrimarySurface->Release();
	}

_QUL_END
