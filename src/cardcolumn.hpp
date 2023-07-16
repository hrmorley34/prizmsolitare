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
    CardColumn();

    void PushLeft(const Card card);
    void PushUnrevealed(const Card card);
    void Reveal();

    void MoveMany(int count, CardColumn *dest);
    // void MoveMany(int count, CardLL *dest);
    // void MoveManyFrom(int count, CardLL *src);

    Card PopLeft();
    Card Peek() const;
    Card PeekAt(const int index) const;

    int revealedlength() const;
    int unrevealedlength() const;
    bool empty() const;

    int WriteToArray(SaveData *const arr) const;
    int GetWriteLength() const;
    int ReadFromArray(const SaveData *const arr);
};

#endif
