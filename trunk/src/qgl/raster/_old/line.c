/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : line.c
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     xxxx-xx-xx  <Persons>   Created.
 *
 ******************************************************************************/

/*--------------------------------- Includes ---------------------------------*/

#include <math.h>

#include "qgl.h"

/*---------------------------------- Macros ----------------------------------*/

/* 
 * .. Line Drawing
 */

/* .... Switches */

#define LINE_MIDPOINT       0
#define LINE_BRESENHAM      1
#define LINE_MODE           LINE_BRESENHAM

#define CIRCLE_MIDPOINT     0
#define CIRCLE_BRESENHAM    1
#define CIRCLE_MODE         CIRCLE_BRESENHAM

/* 
 * .. Circle Drawing
 */

/* 
 * .. Ellipse Drawing
 */

/*---------------------------------- Types -----------------------------------*/

/*-------------------------------- Constants ---------------------------------*/

/*-------------------------------- Variables ---------------------------------*/

/*----------------------------- Static Functions -----------------------------*/

/*---------------------------- External Functions ----------------------------*/

/* 
 * .. Line Drawing
 */

/*******************************************************************************
 * Prototype    : void Qgl_Line (QglInt x0, QglInt y0, QglInt x1, QglInt y1, 
 *                               QglPixie *pixie)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/

void Qgl_Line (QglInt x0, QglInt y0, QglInt x1, QglInt y1, QglPixie *pixie)
{   
#if LINE_MODE == LINE_MIDPOINT
    QglInt  x, y;
    QglSint a, b;
    QglSint inc;
    QglSint d;
    QglSint delta1, delta2;

    b = x1 - x0;
    a = y0 - y1;
    
    if (abs(b) < abs(a))
    {   /* Scan with `y' */

        if (b < 0 || b == 0 && a < 0)
        {
            b = -b; /* y1 - y0 */
            a = -a; /* x0 - x1 */
        }

        if (y0 > y1)
        {
            d = x0;
            x0 = x1;
            x1 = d;

            d = y0;
            y0 = y1;
            y1 = d;
        }
        
        x = x0;
        y = y0;
        inc = (x < x1)? 1 : -1;

        d = 2 * b + inc * a;
        delta1 = 2 * b;
        delta2 = 2 * (inc * a + b);

        while (y <= y1)
        {
            pixie->set(pixie, x, y);
            if (d > 0)
            {
                y++;
                x += inc;
                d += delta2;
            }
            else
            {
                y++;
                d += delta1;
            }
        }
    }
    else
    {   /* Scan with `x' */
        if (a < 0 || a == 0 && b < 0)
        {
            b = -b;
            a = -a;
        }

        if (x0 > x1)
        {
            d = x0;
            x0 = x1;
            x1 = d;
            
            d = y0;
            y0 = y1;
            y1 = d;
        }
        
        x = x0;
        y = y0;
        inc = (y < y1)? 1 : -1;

        d = 2 * a + inc * b;
        delta1 = 2 * a;
        delta2 = 2 * (a + inc * b);
        
        while (x <= x1)
        {
            pixie->set(pixie, x, y);
            if (d > 0)
            {
                x++;
                y += inc;
                d += delta2;
            }
            else
            {
                x++;
                d += delta1;
            }
        }
    }

#elif LINE_MODE == LINE_BRESENHAM   
    /* Bresenham algorithm scanning from top to bottom */

    QglInt  x, y;
    QglInt  acc;
    QglSint dx, dy;
    QglSint xDir;

    dx = x1 - x0;
    dy = y1 - y0;

    if (dy < 0)
    {
        acc = x0;
        x0 = x1;
        x1 = acc;

        acc = y0;
        y0 = y1;
        y1 = acc;

        dx = -dx;
        dy = -dy;
    }

    acc = 0;
    xDir = 1;

    if (dx < 0)
    {
        dx = -dx;
        xDir = -1;
    }

    if (dx > dy)
    {   /* Scan with `x' */
        QglInt thr = dx / 2;
        QglInt i;
        
        y = y0;
        x = x0;

        for (i = 0; i <= dx; i++)
        {
            pixie->set(pixie, x, y);

            acc += dy;
            if (acc > thr)
            {
                y++;
                acc -= dx;
            }

            x += xDir;
        }
    }
    else
    {   /* Scan with 'y' */
        QglInt thr = dy / 2;

        x = x0;
        for (y = y0; y <= y1; y++)
        {
            pixie->set(pixie, x, y);

            acc += dx;
            if (acc > thr)
            {
                x += xDir;
                acc -= dy;
            }
        }
    }

#else

#   error "Bad mode!"
    
#endif

}

/* 
 * .. Circle Drawing
 */

/*******************************************************************************
 * Prototype    : void Qgl_Circle (QglInt x0, QglInt y0, QglInt radius, 
 *                                 QglPixie *pixie)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/

void Qgl_Circle (QglInt x0, QglInt y0, QglInt radius, QglPixie *pixie)
{
#if CIRCLE_MODE == CIRCLE_BRESENHAM
    enum Direction
    {
        k_H,
        k_D,
        k_V
    } direction;

    QglInt  rx, ry;
    QglSint delta;

    rx = 0;
    ry = radius;
    delta = 2 * (1 - radius);

    pixie->set(pixie, x0 + rx, y0 + ry);
    pixie->set(pixie, x0 + rx, y0 - ry);
    
    while (ry != 0)
    {
        
        if (delta < 0)
        {
            QglSint deltaHD = 2 * (delta + ry) - 1;
            direction = (deltaHD <= 0)? k_H : k_D;
        }
        else if (delta > 0)
        {
            QglSint deltaDV = 2 * (delta - rx) - 1;
            direction = (deltaDV <= 0)? k_D : k_V;
        }
        else
        {
            direction = k_D;
        }

        switch(direction) 
        {
        case k_H:
            rx++;
            delta += 2 * rx + 1;
            break;
        case k_D:
            rx++;
            ry--;
            delta += 2 * (rx - ry + 1);
            break;
        case k_V:
            ry--;
            delta += -2 * ry + 1;
            break;
        }

        pixie->set(pixie, x0 + rx, y0 + ry);
        pixie->set(pixie, x0 - rx, y0 + ry);
        pixie->set(pixie, x0 - rx, y0 - ry);
        pixie->set(pixie, x0 + rx, y0 - ry);
    }

    pixie->set(pixie, x0 + rx, y0 + ry);
    pixie->set(pixie, x0 - rx, y0 + ry);
    
    
#elif CIRCLE_MODE == CIRCLE_MIDPOINT
    QglInt  rx, ry; 
    QglSint deltaX, deltaY;
    QglSint d;

    rx = 0;
    ry = radius;
    deltaX = 3;
    deltaY = 2 - radius - radius;
    d = 1 - radius;

    pixie->set(pixie, x0 + rx, y0 + ry);
    pixie->set(pixie, x0 + ry, y0 + rx);
    pixie->set(pixie, x0 + rx, y0 - ry);
    pixie->set(pixie, x0 - ry, y0 + rx);

    while (rx < ry)
    {
        if (d < 0)
        {
            d += deltaX;
            deltaX += 2;
            rx++;
        }
        else
        {
            d += deltaX + deltaY;
            deltaX += 2;
            deltaY += 2;
            rx++;
            ry--;
        }
        pixie->set(pixie, x0 + rx, y0 + ry);
        pixie->set(pixie, x0 + ry, y0 + rx);
        pixie->set(pixie, x0 - rx, y0 + ry);
        pixie->set(pixie, x0 + ry, y0 - rx);
        pixie->set(pixie, x0 - rx, y0 - ry);
        pixie->set(pixie, x0 - ry, y0 - rx);
        pixie->set(pixie, x0 + rx, y0 - ry);
        pixie->set(pixie, x0 - ry, y0 + rx);
    }

    if (rx == ry)
    {
        pixie->set(pixie, x0 + rx, y0 + ry);
        pixie->set(pixie, x0 - rx, y0 + ry);
        pixie->set(pixie, x0 - rx, y0 - ry);
        pixie->set(pixie, x0 + rx, y0 - ry);
    }

#else

#   error "Bad mode!"

#endif
}



/* 
 * .. Ellipse Drawing
 */

/*******************************************************************************
 * Prototype    : void Qgl_Ellipse (QglInt x0, QglInt y0, QglInt a, QglInt b, 
 *                                  QglPixie *pixie)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
void Qgl_Ellipse (QglInt x0, QglInt y0, QglInt a, QglInt b, QglPixie *pixie)
{
    QglInt  rx, ry;
    QglSint d;

    rx = 0;
    ry = b;

    d = 4 * b * b + a * a * (-4 * b + 1);

    pixie->set(pixie, x0, y0 + ry);
    pixie->set(pixie, x0, y0 - ry);
    
    while (2 * b * b * (rx + 1) < a * a * (2 * ry - 1))
    {
        if (d < 0)
        {
            d += 4 * b * b * (2 * rx + 3);
            rx++;
        }
        else
        {
            d += 4 * (b * b * (2 * rx + 3) + a * a * (-2 * ry + 2));
            rx++;
            ry--;
        }

        pixie->set(pixie, x0 + rx, y0 + ry);
        pixie->set(pixie, x0 - rx, y0 + ry);
        pixie->set(pixie, x0 - rx, y0 - ry);
        pixie->set(pixie, x0 + rx, y0 - ry);
        
    }

    d = (b * (2 * rx + 1)) * (b * (2 * rx + 1));
    d += 4 * a * a * ((ry - 1) * (ry - 1) - b * b);

    while (ry > 0)
    {

        if (d < 0)
        {
            d += 4 * (b * b * (2 * rx + 2) + a * a * (-2 * ry + 3));
            rx++;
            ry--;
        }
        else
        {
            d += 4 * a * a * (-2 * ry + 3);
            ry--;
        }
        pixie->set(pixie, x0 + rx, y0 + ry);
        pixie->set(pixie, x0 - rx, y0 + ry);
        pixie->set(pixie, x0 - rx, y0 - ry);
        pixie->set(pixie, x0 + rx, y0 - ry);
    }

    pixie->set(pixie, x0 + rx, y0);
    pixie->set(pixie, x0 - rx, y0);
}

/*------------------------------ End of Module -------------------------------*/
