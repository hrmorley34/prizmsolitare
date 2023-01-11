#include "../imageclasses.hpp"
#include "../card.hpp"

#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#pragma region IMAGEDATA
const Image CardImage;
const Image SuitImages[4];
const Image NumImages[2][14];
void DrawCard(Card card, int x, int y);
void DrawCardBack(int x, int y);
void DrawCardShine(int x, int y);
const color_t BackgroundColour;
#pragma endregion IMAGEDATA

void DrawBackground(int miny, int yrange, int minx, int xrange)
{
    color_t *v = VRAM + miny * LCD_WIDTH_PX + minx;
    for (int y = 0; y < yrange; y++)
    {
        for (int x = 0; x < xrange; x++)
        {
            *v = BackgroundColour;
            v++;
        }
        v += LCD_WIDTH_PX - xrange;
    }
}

#endif
