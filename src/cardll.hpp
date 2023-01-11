#include <stdlib.h>
#include "stdint.h"

#include "card.hpp"
#include "cardcollectionutils.hpp"

#ifndef CARDLL_H
#define CARDLL_H

CardLLPointer pointers[52] = {};

class CardLL
{
private:
    CardLLPointer *head;
    CardLLPointer *tail;

public:
    CardLL() : head(nullptr), tail(nullptr) {}

    // ~CardLL()
    // {
    //     // DeleteQueuePointers(head);
    // }

    void PushLeft(Card card)
    {
        // head = new CardLLPointer(card, head);
        CardLLPointer *lasthead = head;
        // head = (CardLLPointer *)malloc(sizeof(CardLLPointer));
        head = &pointers[card.GetPIndex()];
        *head = CardLLPointer(card, lasthead);
        if (tail == nullptr)
            tail = head;
    }

    void PushRight(Card card)
    {
        if (head == nullptr)
        {
            // head = tail = new CardLLPointer(card);
            // head = tail = (CardLLPointer *)malloc(sizeof(CardLLPointer));
            head = tail = &pointers[card.GetPIndex()];
            *head = CardLLPointer(card);
        }
        else
        {
            // tail->next = new CardLLPointer(card);
            // tail->next = (CardLLPointer *)malloc(sizeof(CardLLPointer));
            tail->next = &pointers[card.GetPIndex()];
            *(tail->next) = CardLLPointer(card);
            tail = tail->next;
        }
    }

    Card PopLeft()
    {
        if (head == nullptr)
            return Card();
        CardLLPointer *p = head;
        Card c = p->card;
        head = p->next;
        // delete p;
        // free(p);
        if (head == nullptr)
            tail = nullptr;
        return c;
    }

    Card PopAt(int index)
    {
        if (index < 0 || head == nullptr)
            return Card();
        if (index == 0)
            return PopLeft();
        CardLLPointer *prev = nullptr;
        CardLLPointer *p = head;
        for (int i = 0; i < index; i++)
        {
            prev = p;
            p = p->next;
            if (p == nullptr)
                return Card();
        }
        Card c = p->card;
        prev->next = p->next;
        // delete p;
        // free(p);
        return c;
    }

    Card Peek()
    {
        if (head == nullptr)
            return Card();
        return head->card;
    }

    Card PeekAt(int index)
    {
        CardLLPointer *p = GetAtIndex(head, index);
        if (p == nullptr)
            return Card();
        return p->card;
    }

    void MoveMany(int count, CardLL *dest)
    {
        if (count <= 0)
            return;
        if (count > this->length())
            count = this->length();
        CardLLPointer *move_head = head;
        CardLLPointer *move_tail = GetAtIndex(head, count - 1);
        // move_tail is not null, because of length check
        CardLLPointer *new_head = move_tail->next;
        move_tail->next = dest->head;
        dest->head = move_head;
        this->head = new_head;
    }

    int length()
    {
        int i = 0;
        CardLLPointer *p = head;
        while (p != nullptr)
        {
            i += 1;
            p = p->next;
        }
        return i;
    }

    bool empty()
    {
        return head == nullptr;
    }

#define SHUFFLE_ITERATIONS 256
    void ShuffleCards()
    {
        int length = this->length();
        for (int i = 0; i < SHUFFLE_ITERATIONS; i++)
        {
            this->PushRight(this->PopAt(sys_rand() % (length - 1)));
        }
    }
};

void DefineCardDeck()
{
    for (uint8_t i = 0; i < SUIT_COUNT; i++)
    {
        for (uint8_t j = 1; j <= 13; j++)
        {
            pointers[i * 13 + j - 1] = Card((Suit)i, (Rank)j);
        }
    }
}

void FillCardDeck(CardLL *cs)
{
    for (uint8_t i = 0; i < 52; i++)
    {
        cs->PushRight(pointers[i].card);
    }
}

#endif
