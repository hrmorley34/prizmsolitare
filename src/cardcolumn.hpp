#include "card.hpp"
#include "cardll.hpp"
#include "savedata.hpp"

#ifndef CARDCOLUMN_H
#define CARDCOLUMN_H

class CardColumn
{
private:
    CardLL revealed;
    CardLL unrevealed;

public:
    CardColumn() {}

    void PushLeft(const Card card)
    {
        revealed.PushLeft(card);
    }

    void PushUnrevealed(const Card card)
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

    Card Peek() const
    {
        // if (revealed.empty())
        //     Reveal();
        return revealed.Peek();
    }

    Card PeekAt(const int index) const
    {
        // if (revealed.empty())
        //     Reveal();
        return revealed.PeekAt(index);
    }

    int revealedlength() const
    {
        return revealed.length();
    }

    int unrevealedlength() const
    {
        return unrevealed.length();
    }

    bool empty() const
    {
        return revealed.empty() && unrevealed.empty();
    }

    int WriteToArray(SaveData *const arr) const
    {
        int length = unrevealed.WriteToArray(arr);
        length += revealed.WriteToArray(arr + length);
        return length;
    }

    int GetWriteLength() const
    {
        return unrevealed.GetWriteLength() +
               revealed.GetWriteLength();
    }

    int ReadFromArray(const SaveData *const arr)
    {
        int length = unrevealed.ReadFromArray(arr);
        length += revealed.ReadFromArray(arr + length);
        return length;
    }
};

#endif
