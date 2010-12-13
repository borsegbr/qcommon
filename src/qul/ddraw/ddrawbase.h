/*
 * === DirctDraw Utility ===
 *  Unit Name:  ddrawbase
 *  Author:     quanben
 *  Purpose:    Encapsulation of elementary DirectDraw utilities.
 * == History ==
 *  2005-01-16: Created.
 *  2006-10-18: Memory leak which caused by UNRELEASE of GDI objects in [:DirectDrawUtils::DrawBits:] may lead to display block, corrected.
 *
 */

#if !defined (_ddrawbase_H)
#define _ddrawbase_H    200610180105


#include <qul/all/predef.h>

#include <ddraw.h>

_QUL_BEGIN

class DirectDrawUtils
    {

private:
    void SetupPrimarySurface ();

public:
    DirectDrawUtils ();
    ~DirectDrawUtils ();

    void InitFullScreen (HWND hwnd);
    void SetDisplayMode (DWORD width, DWORD height, DWORD bitsPerPel);
    void GetDC (HDC &hdc);
    void DrawBits (int left, int top, int width, int height, int bitDepth, BYTE *pBits);
    void BeginDrawing (HDC *hdc);
    void EndDrawing (HDC hdc);
    void Finalize ();

    void DisplayError (const char *error, bool canExit = true) const;

private:
    HWND                m_hwnd;
    IDirectDraw         *m_pDirectDraw;
    IDirectDrawSurface  *m_pPrimarySurface;
    IDirectDrawSurface  *m_pAttachedSurface;

    };  // class DirectDrawUtils

_QUL_END

#endif  // ifndef _ddrawbase_H
