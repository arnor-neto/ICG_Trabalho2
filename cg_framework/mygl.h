#ifndef _MYGL_H_
#define _MYGL_H_
#include <math.h>

#include "definitions.h"

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

struct pixel
{
    int X, Y;
    int R, G, B, A;
};

void putPixel(struct pixel P)
{
    FBptr[4 * P.X + 4 * P.Y * IMAGE_WIDTH + 0] = P.R;
    FBptr[4 * P.X + 4 * P.Y * IMAGE_WIDTH + 1] = P.G;
    FBptr[4 * P.X + 4 * P.Y * IMAGE_WIDTH + 2] = P.B;
    FBptr[4 * P.X + 4 * P.Y * IMAGE_WIDTH + 3] = P.A;
}

void drawLine(struct pixel P1, struct pixel P2)
{

    int dx = P2.X - P1.X;
    int dy = P2.Y - P1.Y;
    int incr_E, incr_NE;
    int adcX, adcY;
    int d;

    float red, green, blue, alpha;
    float changeR, changeG, changeB, changeA;

    red = P1.R;
    green = P1.G;
    blue = P1.B;
    alpha = P1.A;

    if (dx > 0)
    {
        adcX = 1;
    }
    else
    {
        adcX = -1;
    }

    if (dy > 0)
    {
        adcY = 1;
    }
    else
    {
        adcY = -1;
    }

    if (dx < 0)
    {
        drawLine(P2, P1);
        return;
    }

    putPixel(P1);

    // Cálculo de interpolação

    if (abs(dx) >= abs(dy))
    {
        changeR = (float)(P2.R - P1.R) / abs(dx);
        changeG = (float)(P2.G - P1.G) / abs(dx);
        changeB = (float)(P2.B - P1.B) / abs(dx);
        changeA = (float)(P2.A - P1.A) / abs(dx);
    }
    else
    {
        changeR = (float)(P2.R - P1.R) / abs(dy);
        changeG = (float)(P2.G - P1.G) / abs(dy);
        changeB = (float)(P2.B - P1.B) / abs(dy);
        changeA = (float)(P2.A - P1.A) / abs(dy);
    }

    // esse é do 5 -> 1
    if (abs(dx) >= abs(dy) && dy >= 0)
    {
        d = 2 * dy - dx;
        incr_E = 2 * dy;
        incr_NE = 2 * (dy - dx);

        while (P1.X != P2.X || P1.Y != P2.Y)
        {
            if (d <= 0)
            {
                d += incr_E;
                P1.X += adcX;
            }
            else
            {
                d += incr_NE;
                P1.X += adcX;
                P1.Y += adcY;
            }
            //Interpolação de cores
            red += changeR;
            green += changeG;
            blue += changeB;
            alpha += changeA;
            P1.R = red;
            P1.G = green;
            P1.B = blue;
            P1.A = alpha;

            putPixel(P1);
        }
    }

    // Esse é o do 4 -> 8
    if (abs(dx) >= abs(dy) && dy <= 0)
    {
        d = -2 * dy - dx;
        incr_E = -2 * dy;
        incr_NE = 2 * (-dy - dx);

        while (P1.X != P2.X || P1.Y != P2.Y)
        {
            if (d <= 0)
            {
                d += incr_E;
                P1.X += adcX;
            }
            else
            {
                d += incr_NE;
                P1.X += adcX;
                P1.Y += adcY;
            }
            //Interpolação de cores
            red += changeR;
            green += changeG;
            blue += changeB;
            alpha += changeA;
            P1.R = red;
            P1.G = green;
            P1.B = blue;
            P1.A = alpha;

            putPixel(P1);
        }
    }

    // esse é do 6 -> 2
    if (abs(dy) > abs(dx) && dy >= 0)
    {
        d = 2 * dx - dy;
        incr_E = 2 * dx;
        incr_NE = 2 * (dx - dy);

        while (P1.X != P2.X || P1.Y != P2.Y)
        {
            if (d <= 0)
            {
                d += incr_E;
                P1.Y += adcY;
            }
            else
            {
                d += incr_NE;
                P1.X += adcX;
                P1.Y += adcY;
            }
            //Interpolação de cores
            red += changeR;
            green += changeG;
            blue += changeB;
            alpha += changeA;
            P1.R = red;
            P1.G = green;
            P1.B = blue;
            P1.A = alpha;

            putPixel(P1);
        }
    }

    // esse é do 3 -> 7
    if (abs(dy) > abs(dx) && dy <= 0)
    {
        d = 2 * dx + dy;
        incr_E = 2 * dx;
        incr_NE = 2 * (dx + dy);

        while (P1.X != P2.X || P1.Y != P2.Y)
        {
            if (d < 0)
            {
                d += incr_E;
                P1.Y += adcY;
            }
            else
            {
                d += incr_NE;
                P1.X += adcX;
                P1.Y += adcY;
            }
            //Interpolação de cores
            red += changeR;
            green += changeG;
            blue += changeB;
            alpha += changeA;
            P1.R = red;
            P1.G = green;
            P1.B = blue;
            P1.A = alpha;

            putPixel(P1);
        }
    }
}

void drawTriangle(struct pixel P1, struct pixel P2, struct pixel P3)
{
    drawLine(P1, P2);
    drawLine(P2, P3);
    drawLine(P3, P1);
}

#endif // _MYGL_H_
