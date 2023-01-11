#include <fxcg/display.h>

#ifndef IMAGE_H
#define IMAGE_H

// color_t *const VRAM = (color_t *)GetVRAMAddress();
color_t *VRAM;

struct Palette
{
    const color_t *const colours;
    const int width;
    const int pixels_per_byte;
    const int bitmask;
    const int transparent = -1;

    void write(int colour, color_t *dest) const
    {
        // if (colour < 0 || colour >= len(colours))
        //     return 0;
        if (colour == transparent)
            return;
        *dest = colours[colour];
    }
};

struct Image
{
    const unsigned char *const data;
    const Palette *const palette;
    const int width, height;

    void CopySprite(int dest_x, int dest_y, bool upsidedown = false) const
    {
        const unsigned char *d = data;
        color_t *v = VRAM + dest_y * LCD_WIDTH_PX + dest_x;
        int vinc = upsidedown ? -1 : 1;
        int vlineinc = vinc * (LCD_WIDTH_PX - width);
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x += palette->pixels_per_byte)
            {
                for (int dx = 0; dx < palette->pixels_per_byte && x + dx < width; dx++)
                {
                    unsigned char pcolour = (*d) >> (8 - (dx + 1) * palette->width);
                    pcolour &= palette->bitmask;
                    palette->write(pcolour, v);
                    v += vinc;
                }
                d++;
            }
            v += vlineinc;
        }
    }
};

#endif
