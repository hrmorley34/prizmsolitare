#include "card.hpp"
#include "cardll.hpp"
#include "cardcolumn.hpp"
#include "cardselection.hpp"
#include "savedata.hpp"

#ifndef GAME_H
#define GAME_H

extern const int DEAL_COUNT;
#define COLUMN_COUNT 7

class Game
{
public:
    CardLL pile;
    CardLL pilediscard;
    CardLL suits[SUIT_COUNT];
    CardColumn columns[COLUMN_COUNT];

    Game();

    void DealToPile();

    bool TryMoveToColumn(
        const int from, const int count,
        const int to, const bool checkStacks);
    bool TryMovePileToColumn(const int column, const bool checkStacks);

    static bool CanStackOn(const Card base, const Card top);
    bool CanStackOnCol(const Card top, const int column);
    bool CanStackOnSuit(const Card top, const Suit suit);

    CardColumn *GetSelectionColumn(const CardSelection sel);
    CardLL *GetSelectionLL(const CardSelection sel);

    bool TryStackOnSel(CardSelection dest, CardSelection src);
    bool TryStackAuto(CardSelection src);

    int WriteToArray(SaveData *const arr) const;
    int GetWriteLength() const;
    int ReadFromArray(const SaveData *const arr);
};

#endif
