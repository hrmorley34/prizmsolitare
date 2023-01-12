#include "stdint.h"

#ifndef CARD_H
#define CARD_H

enum Suit : uint8_t
{
    Diamonds = 0,
    Hearts = 1,
    Clubs = 2,
    Spades = 3
};
#define SUIT_COUNT 4

enum Colour : uint8_t
{
    Red = 0,
    Black = 1
};

Colour GetSuitColour(const Suit suit)
{
    return (Colour)(suit >> 1);
}

typedef uint8_t Rank;

struct Card
{
private:
    uint8_t ivalue; // 0bssrrrr for suit (0-3), rank (1-13)

    void SetInternal(const Suit suit, const Rank rank)
    {
        ivalue = (uint8_t)suit << 4 | (uint8_t)rank;
    }

public:
    Card() : ivalue(0) {}
    Card(const Suit suit, const Rank rank)
    {
        SetInternal(suit, rank);
    }

    bool operator==(const Card c) const
    {
        return ivalue == c.ivalue;
    }
    bool operator!=(const Card c) const
    {
        return !(*this == c);
    }

    bool NonNull() const
    {
        return ivalue != 0;
    }

    Suit GetSuit() const
    {
        return (Suit)(ivalue >> 4);
    }

    Colour GetColour() const
    {
        return GetSuitColour(GetSuit());
    }

    Rank GetRank() const
    {
        return (Rank)(ivalue & 0b00'1111);
    }

    int GetPIndex() const
    {
        return GetSuit() * 13 + GetRank() - 1;
    }
};

#endif
