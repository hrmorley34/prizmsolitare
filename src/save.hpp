#include "game.hpp"
#include "savedata.hpp"

#ifndef SAVE_H
#define SAVE_H

#define COMMENT(x)
#define SAVESTATE_LEN (                                     \
    1 COMMENT(SUIT_COUNT) +                                 \
    1 COMMENT(COLUMN_COUNT) +                               \
    2 COMMENT("pile, pilediscard") +                        \
    SUIT_COUNT +                                            \
    COLUMN_COUNT * 2 COMMENT("each column has two lists") + \
    52 COMMENT("total number of cards - there will be no more pointers than this"))

bool SaveState(const Game *const g);
bool DeleteState();
bool ReadState(Game *const g);

#endif
