#include "imageclasses.hpp"

// color_t *const VRAM = (color_t *)GetVRAMAddress();
color_t *VRAM;

void Palette::Draw(int colour, color_t *dest) const
{
    // if (colour < 0 || colour >= len(colours))
    //     return 0;
    if (colour == transparent)
        return;
    *dest = colours[colour];
}

void Image::CopySprite(int dest_x, int dest_y, bool upsidedown) const
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
                palette->Draw(pcolour, v);
                v += vinc;
            }
            d++;
        }
        v += vlineinc;
    }
}
