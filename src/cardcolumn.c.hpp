#include "cardcolumn.hpp"

CardColumn::CardColumn() {}

void CardColumn::PushLeft(const Card card)
{
    revealed.PushLeft(card);
}

void CardColumn::PushUnrevealed(const Card card)
{
    unrevealed.PushLeft(card);
}

void CardColumn::Reveal()
{
    if (!revealed.empty())
        return;
    if (unrevealed.empty())
        return;
    revealed.PushRight(unrevealed.PopLeft());
}

void CardColumn::MoveMany(int count, CardColumn *dest)
{
    revealed.MoveMany(count, &(dest->revealed));
    Reveal();
}
// void CardColumn::MoveMany(int count, CardLL *dest)
// {
//     revealed.MoveMany(count, dest);
//     Reveal();
// }
// void CardColumn::MoveManyFrom(int count, CardLL *src)
// {
//     src->MoveMany(count, &revealed);
// }

Card CardColumn::PopLeft()
{
    if (revealed.empty())
        Reveal();
    Card c = revealed.PopLeft();
    Reveal();
    return c;
}

Card CardColumn::Peek() const
{
    // if (revealed.empty())
    //     Reveal();
    return revealed.Peek();
}

Card CardColumn::PeekAt(const int index) const
{
    // if (revealed.empty())
    //     Reveal();
    return revealed.PeekAt(index);
}

int CardColumn::revealedlength() const
{
    return revealed.length();
}

int CardColumn::unrevealedlength() const
{
    return unrevealed.length();
}

bool CardColumn::empty() const
{
    return revealed.empty() && unrevealed.empty();
}

int CardColumn::WriteToArray(SaveData *const arr) const
{
    int length = unrevealed.WriteToArray(arr);
    length += revealed.WriteToArray(arr + length);
    return length;
}

int CardColumn::GetWriteLength() const
{
    return unrevealed.GetWriteLength() +
           revealed.GetWriteLength();
}

int CardColumn::ReadFromArray(const SaveData *const arr)
{
    int length = unrevealed.ReadFromArray(arr);
    length += revealed.ReadFromArray(arr + length);
    return length;
}
