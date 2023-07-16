#include <fxcg/display.h>

#ifndef IMAGE_H
#define IMAGE_H

// color_t *const VRAM = (color_t *)GetVRAMAddress();
extern color_t *VRAM;

struct Palette
{
    const color_t *const colours;
    const int width;
    const int pixels_per_byte;
    const int bitmask;
    const int transparent = -1;

    void Draw(int colour, color_t *dest) const;
};

struct Image
{
    const unsigned char *const data;
    const Palette *const palette;
    const int width, height;

    void CopySprite(int dest_x, int dest_y, bool upsidedown = false) const;
};

#endif
