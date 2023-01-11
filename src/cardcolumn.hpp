#include "card.hpp"
#include "cardll.hpp"

#ifndef CARDCOLUMN_H
#define CARDCOLUMN_H

class CardColumn
{
private:
    CardLL revealed;
    CardLL unrevealed;

public:
    CardColumn() {}

    void PushLeft(Card card)
    {
        revealed.PushLeft(card);
    }

    void PushUnrevealed(Card card)
    {
        unrevealed.PushLeft(card);
    }

    void Reveal()
    {
        if (!revealed.empty())
            return;
        if (unrevealed.empty())
            return;
        revealed.PushRight(unrevealed.PopLeft());
    }

    void MoveMany(int count, CardColumn *dest)
    {
        revealed.MoveMany(count, &(dest->revealed));
        Reveal();
    }
    // void MoveMany(int count, CardLL *dest)
    // {
    //     revealed.MoveMany(count, dest);
    //     Reveal();
    // }
    // void MoveManyFrom(int count, CardLL *src)
    // {
    //     src->MoveMany(count, &revealed);
    // }

    Card PopLeft()
    {
        if (revealed.empty())
            Reveal();
        Card c = revealed.PopLeft();
        Reveal();
        return c;
    }

    Card Peek()
    {
        if (revealed.empty())
            Reveal();
        return revealed.Peek();
    }

    Card PeekAt(int index)
    {
        if (revealed.empty())
            Reveal();
        return revealed.PeekAt(index);
    }

    int revealedlength()
    {
        return revealed.length();
    }

    int unrevealedlength()
    {
        return unrevealed.length();
    }

    bool empty()
    {
        return revealed.empty() && unrevealed.empty();
    }
};

#endif
