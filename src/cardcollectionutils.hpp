#include <stdlib.h>
#include "card.hpp"

#ifndef CARDCOLLECTIONUTILS_H
#define CARDCOLLECTIONUTILS_H

struct CardLLPointer
{
public:
    Card card;
    CardLLPointer *next = nullptr;

    CardLLPointer() {}
    CardLLPointer(const Card card, CardLLPointer *next = nullptr)
        : card(card), next(next) {}
};

// void DeleteQueuePointers(CardLLPointer *p)
// {
//     CardLLPointer *next;
//     while (p != nullptr)
//     {
//         next = p->next;
//         // delete p;
//         free(p);
//         p = next;
//     }
// }

CardLLPointer *GetAtIndex(CardLLPointer *p, const int index)
{
    for (int i = 0; i < index && p != nullptr; i++)
    {
        p = p->next;
    }
    return p;
}

#endif
