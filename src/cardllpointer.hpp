#include <stdlib.h>
#include "card.hpp"

#ifndef CARDLLPOINTER_H
#define CARDLLPOINTER_H

struct CardLLPointer
{
public:
    Card card = Card();
    CardLLPointer *next = nullptr;

    CardLLPointer(const Card card = Card(), CardLLPointer *next = nullptr)
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

inline CardLLPointer *GetAtIndex(CardLLPointer *p, const int index)
{
    for (int i = 0; i < index && p != nullptr; i++)
    {
        p = p->next;
    }
    return p;
}

#endif
