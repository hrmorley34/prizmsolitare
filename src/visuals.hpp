#include <fxcg/display.h>

#include "card.hpp" // SUIT_COUNT
#include "game.hpp" // DEAL_COUNT, COLUMN_COUNT

#ifndef VISUALS_H
#define VISUALS_H

#define CARD_WIDTH 36
#define CARD_HEIGHT 48
#define MARGIN 4

#define DISCARD_OFFSET_X 8
#define CARD_UNREVEALED_OFFSET_Y 4
#define CARD_REVEALED_OFFSET_Y 8

#if CARD_UNREVEALED_OFFSET_Y > CARD_REVEALED_OFFSET_Y
#error Unrevealed height must be smaller than revealed height
#endif

#define CENTRE_X (LCD_WIDTH_PX / 2)
#define GAME_WIDTH ((CARD_WIDTH + 4) * COLUMN_COUNT - 4)

#define LEFT_X (CENTRE_X - GAME_WIDTH / 2)
#if LEFT_X < MARGIN
#error Too many columns for screen width!
#endif

#define COLUMN_X(col) (LEFT_X + (CARD_WIDTH + MARGIN) * (col))
#define PILE_X LEFT_X
#define PILEDISCARD_LEFT_X COLUMN_X(1)
#define PILEDISCARD_X(num) (PILEDISCARD_LEFT_X + DISCARD_OFFSET_X * (num))
#define SUIT_START_COLUMN (COLUMN_COUNT - SUIT_COUNT)
#define SUIT_X(suit) COLUMN_X(SUIT_START_COLUMN + suit)

#define TOP_Y MARGIN

#define COLUMNS_TOP_Y (TOP_Y + CARD_HEIGHT + MARGIN)
#define COLUMN_CARD_Y(unrevealed, unoffset, revealed, reoffset) \
    (COLUMNS_TOP_Y + (unoffset) * (unrevealed) + (reoffset) * (revealed))
#define COLUMNS_BOTTOM_Y (LCD_HEIGHT_PX - MARGIN)
#define COLUMNS_HEIGHT (COLUMNS_BOTTOM_Y - COLUMNS_TOP_Y)

#if COLUMNS_HEIGHT < CARD_HEIGHT
#error Screen to small to fit cards in columns
#endif

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

#endif
