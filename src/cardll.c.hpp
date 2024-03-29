#include "cardll.hpp"

CardLLPointer pointers[52] = {};

CardLL::CardLL() : head(nullptr), tail(nullptr) {}

// CardLL::~CardLL()
// {
//     // DeleteQueuePointers(head);
// }

void CardLL::PushLeft(const Card card)
{
    // head = new CardLLPointer(card, head);
    CardLLPointer *lasthead = head;
    // head = (CardLLPointer *)malloc(sizeof(CardLLPointer));
    head = &pointers[card.GetPIndex()];
    *head = CardLLPointer(card, lasthead);
    if (tail == nullptr)
        tail = head;
}

void CardLL::PushRight(const Card card)
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

Card CardLL::PopLeft()
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

Card CardLL::PopAt(const int index)
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

Card CardLL::Peek() const
{
    if (head == nullptr)
        return Card();
    return head->card;
}

Card CardLL::PeekAt(const int index) const
{
    CardLLPointer *p = GetAtIndex(head, index);
    if (p == nullptr)
        return Card();
    return p->card;
}

void CardLL::MoveMany(int count, CardLL *const dest)
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

int CardLL::length() const
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

bool CardLL::empty() const
{
    return head == nullptr;
}

#define SHUFFLE_ITERATIONS 256
void CardLL::ShuffleCards()
{
    int length = this->length();
    for (int i = 0; i < SHUFFLE_ITERATIONS; i++)
    {
        this->PushRight(this->PopAt(
#ifdef __sh__
            sys_rand()
#else
            rand()
#endif
            % (length - 1)));
    }
}

int CardLL::WriteToArray(SaveData *arr) const
{
    uint8_t *sizeloc = &arr->value;
    *sizeloc = 0;
    arr++;

    CardLLPointer *p = head;
    while (p != nullptr)
    {
        arr->card = p->card;
        arr++;
        (*sizeloc)++;
        p = p->next;
    }

    return (*sizeloc) + 1;
}

int CardLL::GetWriteLength() const
{
    return length() + 1;
}

int CardLL::ReadFromArray(const SaveData *arr)
{
    uint8_t length = arr->value;
    arr++;

    CardLLPointer *p = head = tail = nullptr;
    for (int i = 0; i < length; i++)
    {
        Card c = arr->card;
        tail = &pointers[c.GetPIndex()];
        *tail = CardLLPointer(c);
        if (i == 0)
            head = tail;
        else
            p->next = tail;
        p = tail;
        arr++;
    }

    return length + 1;
}

void FillCardDeck(CardLL *const cs)
{
    for (uint8_t i = 0; i < SUIT_COUNT; i++)
    {
        for (uint8_t j = 1; j <= 13; j++)
        {
            Card c = Card((Suit)i, (Rank)j);
            pointers[c.GetPIndex()] = CardLLPointer(c);
            cs->PushRight(pointers[c.GetPIndex()].card);
        }
    }
}
