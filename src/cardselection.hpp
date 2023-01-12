#include "card.hpp"

#ifndef CARDSELECTION_H
#define CARDSELECTION_H

/*
 * col:
 * 0..6: columns
 * -4..-1: suits
 * -5: discard pile
 * -6: normal pile
 * depth:
 * 0: no selection
 * 1: a single card, or an empty column/suit/pile
 * 2+: multiple cards; only applicable to columns
 */
struct CardSelection
{
public:
    int col;
    int depth;

    CardSelection()
    {
        col = 0;
        depth = 0;
    }

    CardSelection(const int col, const int depth)
    {
        this->col = col;
        this->depth = depth;
    }
};

#define SUIT_COLUMN(suit) (-SUIT_COUNT + suit)
#define SUIT_FROM_COLUMN(col) (Suit)(col + SUIT_COUNT)
#define PILE_COLUMN (-SUIT_COUNT - 2)
#define PILEDISCARD_COLUMN (-SUIT_COUNT - 1)

#endif
