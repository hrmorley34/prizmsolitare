#include <stdlib.h>
#include "stdint.h"

#include "card.hpp"
#include "cardllpointer.hpp"
#include "savedata.hpp"

#ifndef CARDLL_H
#define CARDLL_H

class CardLL
{
private:
    CardLLPointer *head;
    CardLLPointer *tail;

public:
    CardLL();
    // ~CardLL();

    void PushLeft(const Card card);
    void PushRight(const Card card);

    Card PopLeft();
    Card PopAt(const int index);
    Card Peek() const;
    Card PeekAt(const int index) const;

    void MoveMany(int count, CardLL *const dest);

    int length() const;
    bool empty() const;

    void ShuffleCards();

    int WriteToArray(SaveData *arr) const;
    int GetWriteLength() const;
    int ReadFromArray(const SaveData *arr);
};

void FillCardDeck(CardLL *const cs);

#endif
