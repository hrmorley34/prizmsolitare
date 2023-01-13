#ifdef __sh__
#include <fxcg/file.h>
#endif

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

#ifdef __sh__

unsigned char StateFileDir[] = "solitare";
unsigned char StateFileItem[] = "state";
#define StateFile StateFileDir, StateFileItem

SaveData savebuffer[SAVESTATE_LEN];

bool SaveState(const Game *const g)
{
    MCS_CreateDirectory(StateFileDir); // can fail silently
    if (MCS_WriteItem(StateFile, 0, (SAVESTATE_LEN / 4 + 1) * 4, 0) != 0)
        return false;

    if (g->WriteToArray(savebuffer) < 0)
        return false;
    if (MCSOvwDat2(StateFile, SAVESTATE_LEN, savebuffer, 0) != 0)
        return false;
    return true;
}

bool DeleteState()
{
    return MCSDelVar2(StateFile) == 0;
}

bool ReadState(Game *const g)
{
    int len;
    MCSGetDlen2(StateFile, &len);
    if (len < SAVESTATE_LEN)
        return false;
    if (MCSGetData1(0, SAVESTATE_LEN, savebuffer) != 0)
        return false;
    if (g->ReadFromArray(savebuffer) < 0)
        return false;
    return true;
}

#endif // __sh__

#endif
