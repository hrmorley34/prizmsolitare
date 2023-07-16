#include <fxcg/display.h>
#include <fxcg/keyboard.h>

#include "game.hpp"
#include "cardselection.hpp"
#include "visuals.hpp"
#include "imagedata.hpp"

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

    void SelectSrc();

public:
    Controls(Game *const game);

    void ClearScreen() const;

    void DrawScreenTop() const;

    void DrawScreenTopShine(CardSelection sel) const;

    void DrawScreenCol(int col) const;

    void DrawScreen() const;

    void MoveSelection(const int dx, const int dy);

    void DoSelect();

    void HandleKey(const int key);
};

#endif
