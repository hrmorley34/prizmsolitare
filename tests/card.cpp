#include "catch2/catch_amalgamated.hpp"

#include "../src/card.hpp"

TEST_CASE("Cards store properties", "[Card]")
{
    SECTION("empty card is marked as empty")
    {
        Card empty = Card();
        REQUIRE(empty.NonNull() == false);
    }

    SECTION("card preserves properties")
    {
        for (int suit = 0; suit < 4; suit++)
        {
            for (int rank = 1; rank < 13; rank++)
            {
                Card c = Card((Suit)suit, (Rank)rank);
                REQUIRE(c.NonNull() == true);
                REQUIRE(c.GetSuit() == suit);
                REQUIRE(c.GetRank() == rank);
            }
        }
    }
}

TEST_CASE("Cards have equality", "[Card]")
{
    SECTION("same card is equal")
    {
        REQUIRE((Card() == Card()) == true);
        REQUIRE((Card() != Card()) == false);
        REQUIRE((Card((Suit)1, (Rank)5) == Card((Suit)1, (Rank)5)) == true);
        REQUIRE((Card((Suit)1, (Rank)5) != Card((Suit)1, (Rank)5)) == false);
        REQUIRE((Card((Suit)3, (Rank)13) == Card((Suit)3, (Rank)13)) == true);
        REQUIRE((Card((Suit)3, (Rank)13) != Card((Suit)3, (Rank)13)) == false);
    }

    SECTION("different cards are not equal")
    {
        REQUIRE((Card() == Card((Suit)1, (Rank)5)) == false);
        REQUIRE((Card() != Card((Suit)1, (Rank)5)) == true);
        REQUIRE((Card((Suit)1, (Rank)5) == Card((Suit)1, (Rank)4)) == false);
        REQUIRE((Card((Suit)1, (Rank)5) != Card((Suit)1, (Rank)4)) == true);
        REQUIRE((Card((Suit)3, (Rank)13) == Card((Suit)2, (Rank)13)) == false);
        REQUIRE((Card((Suit)3, (Rank)13) != Card((Suit)2, (Rank)13)) == true);
    }
}
