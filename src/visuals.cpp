#include "visuals.hpp"

void calculate_squash(
    const int unrevealed, const int revealed,
    int *const unoffset, int *const reoffset)
{
    *unoffset = CARD_UNREVEALED_OFFSET_Y;
    *reoffset = CARD_REVEALED_OFFSET_Y;
#define CALCSQ_MAX_Y (COLUMN_CARD_Y(unrevealed, *unoffset, revealed - 1, *reoffset) + CARD_HEIGHT)
    while (*unoffset > 0)
    {
        if (CALCSQ_MAX_Y <= COLUMNS_BOTTOM_Y)
            return;
        *unoffset -= 1;
        if (CALCSQ_MAX_Y <= COLUMNS_BOTTOM_Y)
            return;
        *reoffset -= 1;
    }
    while (*reoffset > 0)
    {
        if (CALCSQ_MAX_Y <= COLUMNS_BOTTOM_Y)
            return;
        *reoffset -= 1;
    }
    return; // unoffset = 0; reoffset = 0
#undef CALCSQ_MAX_Y
}
