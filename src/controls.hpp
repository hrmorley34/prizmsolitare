#include <fxcg/display.h>
#include <fxcg/keyboard.h>

#include "game.hpp"
#include "cardselection.hpp"
#include "visuals.hpp"
#include "gen/imagedata.hpp"

#ifndef CONTROLS_H
#define CONTROLS_H

class Controls
{
private:
    Game *game;

    /*
     * src: the source of a move
     * sel: the current selection, which may be the dest of a move
     */
    CardSelection src, sel;

    void SelectSrc()
    {
        src = sel;
        // sel is left unchanged
    }

public:
    Controls(Game *game)
    {
        this->game = game;

        src = CardSelection(0, 0);
        sel = CardSelection(PILE_COLUMN, 1);
    }

    void ClearScreen()
    {
        DrawBackground(0, LCD_HEIGHT_PX, 0, LCD_WIDTH_PX);
    }

    void DrawScreenTop()
    {
        DrawBackground(TOP_Y - MARGIN / 2, CARD_HEIGHT + MARGIN + 1, PILE_X - MARGIN / 2, GAME_WIDTH + MARGIN);

        if (!game->pile.empty())
            DrawCardBack(PILE_X, TOP_Y);
        int discardcount = game->pilediscard.length();
        if (discardcount > DEAL_COUNT)
            discardcount = DEAL_COUNT;
        for (int c = 0; c < discardcount; c++)
            DrawCard(game->pilediscard.PeekAt(discardcount - c - 1), PILEDISCARD_X(c), TOP_Y);
        for (int suit = 0; suit < SUIT_COUNT; suit++)
        {
            Card c = game->suits[suit].Peek();
            if (c.NonNull())
                DrawCard(c, SUIT_X(suit), TOP_Y);
        }

        if (src.depth > 0)
            DrawScreenTopShine(src);
        DrawScreenTopShine(sel);
    }

    void DrawScreenTopShine(CardSelection sel)
    {
        if (sel.col >= 0)
            return;
        if (sel.col >= SUIT_COLUMN(0))
            DrawCardShine(SUIT_X(SUIT_FROM_COLUMN(sel.col)), TOP_Y);
        if (sel.col == PILEDISCARD_COLUMN)
        {
            int discardcount = game->pilediscard.length();
            if (discardcount > DEAL_COUNT)
                discardcount = DEAL_COUNT;
            if (discardcount < 1)
                discardcount = 1;
            DrawCardShine(PILEDISCARD_X(discardcount - 1), TOP_Y);
        }
        if (sel.col == PILE_COLUMN)
            DrawCardShine(PILE_X, TOP_Y);
    }

    void DrawScreenCol(int col)
    {
#define cold game->columns[col]
        const int colx = COLUMN_X(col);

        DrawBackground(COLUMNS_TOP_Y - MARGIN / 2, COLUMNS_HEIGHT + MARGIN, colx - MARGIN / 2, CARD_WIDTH + MARGIN);

        const int unlen = cold.unrevealedlength();
        const int relen = cold.revealedlength();
        int unoff, reoff;
        calculate_squash(unlen, relen, &unoff, &reoff);

        int shinec = 0;
        if (src.col == col && src.depth > shinec)
            shinec = src.depth;
        if (sel.col == col && sel.depth > shinec)
            shinec = sel.depth;
        int shinefirst = relen - shinec;
        if (shinefirst < 0)
            shinefirst = 0;

        for (int c = 0; c < unlen; c++)
            DrawCardBack(colx, COLUMN_CARD_Y(c, unoff, 0, reoff));
        for (int c = 0; c < shinefirst; c++)
            DrawCard(cold.PeekAt(relen - c - 1), colx, COLUMN_CARD_Y(unlen, unoff, c, reoff));
        for (int c = shinefirst; c < relen; c++)
        {
            const int y = COLUMN_CARD_Y(unlen, unoff, c, reoff);
            DrawCard(cold.PeekAt(relen - c - 1), colx, y);
            DrawCardShine(colx, y);
        }

        if (unlen == 0 && relen == 0 && shinec > 0)
            DrawCardShine(colx, COLUMNS_TOP_Y);
#undef cold
    }

    void DrawScreen()
    {
        DrawScreenTop();
        for (int col = 0; col < COLUMN_COUNT; col++)
            DrawScreenCol(col);
    }

    void MoveSelection(int dx, int dy)
    {
        // const int prev_col = sel.col;

        sel.col += dx;
        if (sel.col < PILE_COLUMN)
            sel.col = COLUMN_COUNT - 1;
        if (sel.col >= COLUMN_COUNT)
            sel.col = PILE_COLUMN;

        if (sel.col < 0)
        {
            sel.depth = 1;
            if (dy != 0)
            {
                switch (sel.col)
                {
                case PILE_COLUMN:
                    sel.col = 0;
                    break;
                case PILEDISCARD_COLUMN:
                    sel.col = 1;
                    break;
                default:
                    sel.col += COLUMN_COUNT;
                    break;
                }
            }
        }
        else
        {
            CardColumn *col = game->GetSelectionColumn(sel);
            int length = col->revealedlength();
            if (length < 1)
                length = 1;
            if (sel.depth < 1)
                sel.depth = 1;
            if (sel.depth > length)
                sel.depth = length;
            sel.depth -= dy;
            if (sel.depth < 1 || sel.depth > length)
            {
                sel.depth = 1;
                // move up/down (same thing)
                if (sel.col >= COLUMN_COUNT - SUIT_COUNT)
                    sel.col -= COLUMN_COUNT;
                else if (sel.col >= 1)
                    sel.col = PILEDISCARD_COLUMN;
                else
                    sel.col = PILE_COLUMN;
            }
        }

        // if (prev_col < 0 || sel.col < 0)
        //     DrawScreenTop();
        // if (prev_col > 0)
        //     DrawScreenCol(prev_col);
        // if (sel.col > 0 && sel.col != prev_col)
        //     DrawScreenCol(sel.col);
    }

    void DoSelect()
    {
        if (src.depth == 0)
        {
            if (game->TryStackAuto(sel))
            {
                // preserve current selection for next move
            }
            else
            {
                src = sel;
            }
        }
        else
        {
            if (game->TryStackOnSel(sel, src))
            {
                // do nothing - may react with colour here
            }
            src = CardSelection(0, 0);
        }

        // DrawScreen();
    }

    void HandleKey(const int key)
    {
        switch (key)
        {
        case KEY_CTRL_UP:
            MoveSelection(0, -1);
            break;
        case KEY_CTRL_DOWN:
            MoveSelection(0, 1);
            break;
        case KEY_CTRL_LEFT:
            MoveSelection(-1, 0);
            break;
        case KEY_CTRL_RIGHT:
            MoveSelection(1, 0);
            break;
        case KEY_CTRL_EXE:
            DoSelect();
            break;
        }
        // ClearScreen();
        DrawScreen();
    }
};

#endif
