#include "card.hpp"
#include "cardll.hpp"
#include "cardcolumn.hpp"
#include "cardselection.hpp"

#ifndef GAME_H
#define GAME_H

const int DEAL_COUNT = 3;
#define COLUMN_COUNT 7

class Game
{
public:
    CardLL pile;
    CardLL pilediscard;
    CardLL suits[SUIT_COUNT];
    CardColumn columns[COLUMN_COUNT];

    Game()
    {
        FillCardDeck(&pile);
        pile.ShuffleCards();
        for (int i = 0; i < COLUMN_COUNT; i++)
        {
            for (int j = 0; j < i; j++)
            {
                columns[i].PushUnrevealed(pile.PopLeft());
            }
            columns[i].PushLeft(pile.PopLeft());
        }
    }

    void DealToPile()
    {
        if (pile.length() == 0)
        {
            Card c = pilediscard.PopLeft();
            while (c.NonNull())
            {
                pile.PushLeft(c);
                c = pilediscard.PopLeft();
            }
        }
        for (int i = 0; i < DEAL_COUNT; i++)
        {
            Card c = pile.PopLeft();
            if (!c.NonNull())
                break;
            pilediscard.PushLeft(c);
        }
    }

    bool TryMoveToColumn(
        const int from, const int count,
        const int to, const bool checkStacks)
    {
        if (from < 0 || from >= COLUMN_COUNT ||
            to < 0 || to >= COLUMN_COUNT ||
            from == to || count <= 0)
            return false;
#define srccol columns[from]
#define destcol columns[to]
        Card src = srccol.PeekAt(count - 1);
        if (!src.NonNull())
            return false;
        if (checkStacks)
        {
            if (!CanStackOn(destcol.Peek(), src))
                return false;
        }
        srccol.MoveMany(count, &destcol);
        srccol.Reveal();
        return true;
#undef srccol
#undef destcol
    }

    bool TryMovePileToColumn(const int column, const bool checkStacks)
    {
        if (column < 0 || column >= COLUMN_COUNT)
            return false;
#define destcol columns[column]
        Card src = pilediscard.Peek();
        if (!src.NonNull())
            return false;
        if (checkStacks)
        {
            if (!CanStackOn(destcol.Peek(), src))
                return false;
        }
        destcol.PushLeft(pilediscard.PopLeft());
        return true;
#undef destcol
    }

    static bool CanStackOn(const Card base, const Card top)
    {
        return top.NonNull() &&
               ((!base.NonNull() && top.GetRank() == 13) ||
                (base.GetColour() != top.GetColour() &&
                 base.GetRank() == top.GetRank() + 1));
    }

    bool CanStackOnCol(const Card top, const int column)
    {
        if (column < 0 || column >= COLUMN_COUNT)
            return false;
        return CanStackOn(columns[column].Peek(), top);
    }

    bool CanStackOnSuit(const Card top, const Suit suit)
    {
        if (suit < 0 || suit >= SUIT_COUNT)
            return false;
        if (top.GetSuit() != suit)
            return false;
        return suits[suit].Peek().GetRank() == top.GetRank() + 1;
    }

    CardColumn *GetSelectionColumn(const CardSelection sel)
    {
        if (sel.col >= COLUMN_COUNT)
            return nullptr;
        if (sel.col >= 0)
            return &columns[sel.col];
        return nullptr;
    }

    CardLL *GetSelectionLL(const CardSelection sel)
    {
        if (sel.col >= 0)
            return nullptr;
        if (sel.col >= SUIT_COLUMN(0))
            return &suits[SUIT_FROM_COLUMN(sel.col)];
        if (sel.col == PILEDISCARD_COLUMN)
            return &pilediscard;
        if (sel.col == PILE_COLUMN)
            return &pile;
        return nullptr;
    }

    bool TryStackOnSel(CardSelection dest, CardSelection src)
    {
        if (src.col == PILE_COLUMN && dest.col == PILEDISCARD_COLUMN)
        {
            DealToPile();
            return true;
        }
        if (src.col < PILEDISCARD_COLUMN || src.col >= COLUMN_COUNT)
            return false;
        if (dest.col < SUIT_COLUMN(0) || dest.col >= COLUMN_COUNT)
            return false;
        if (src.col == dest.col)
            return false;
        if (src.col < 0)
        {
            src.depth = 1;
            CardLL *srcl = GetSelectionLL(src);
            if (srcl == nullptr)
                return false;
            Card movecard = srcl->Peek();
            if (!movecard.NonNull())
                return false;

            if (dest.col < 0)
            {
                CardLL *destl = GetSelectionLL(dest);
                if (destl == nullptr)
                    return false;
                if (!CanStackOnSuit(movecard, (Suit)(dest.col + SUIT_COUNT)))
                    return false;
                destl->PushLeft(srcl->PopLeft());
                return true;
            }
            else
            {
                CardColumn *destl = GetSelectionColumn(dest);
                if (destl == nullptr)
                    return false;
                if (!CanStackOnCol(movecard, dest.col))
                    return false;
                destl->PushLeft(srcl->PopLeft());
                return true;
            }
        }
        else
        {
            int count = dest.col < 0 ? 1 : src.depth;
            CardColumn *srcc = GetSelectionColumn(src);
            if (srcc == nullptr)
                return false;
            Card movecard = srcc->PeekAt(count - 1);
            if (!movecard.NonNull())
                return false;

            if (dest.col < 0)
            {
                CardLL *destl = GetSelectionLL(dest);
                if (destl == nullptr)
                    return false;
                if (!CanStackOnSuit(movecard, SUIT_FROM_COLUMN(dest.col)))
                    return false;
                destl->PushLeft(srcc->PopLeft());
                return true;
            }
            else
            {
                CardColumn *destc = GetSelectionColumn(dest);
                if (destc == nullptr)
                    return false;
                if (!CanStackOnCol(movecard, dest.col))
                    return false;
                srcc->MoveMany(count, destc);
                return true;
            }
        }
    }

    bool TryStackAuto(CardSelection src)
    {
        if (src.col == PILE_COLUMN)
        {
            DealToPile();
            return true;
        }
        if (src.col == PILEDISCARD_COLUMN ||
            (src.col >= 0 && src.col < COLUMN_COUNT && src.depth == 1))
        {
            Card c;
            if (src.col == PILEDISCARD_COLUMN)
                c = pilediscard.Peek();
            else
                c = columns[src.col].Peek();
            // Works even for no card, because blank rank is 0, so ace - 1 = 0
            if (suits[c.GetSuit()].Peek().GetRank() == c.GetRank() - 1)
            {
                if (src.col == PILEDISCARD_COLUMN)
                    c = pilediscard.PopLeft();
                else
                    c = columns[src.col].PopLeft();
                suits[c.GetSuit()].PushLeft(c);
                return true;
            }
        }
        // if (src.col < 0 || src.col >= COLUMN_COUNT)
        return false;
    }
};

#endif
