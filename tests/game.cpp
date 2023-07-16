#include "catch2/catch_amalgamated.hpp"

#include "../src/game.hpp"
#include "../src/save.hpp"

TEST_CASE("Game functions", "[Game]")
{
    Game game = Game();

    for (int i = 0; i < COLUMN_COUNT; i++)
    {
        REQUIRE(game.columns[i].revealedlength() == 1);
        REQUIRE(game.columns[i].unrevealedlength() == i);
    }

    for (int i = 0; i < 4; i++)
    {
        REQUIRE(game.suits[i].length() == 0);
    }

    REQUIRE(game.pile.length() > 0);
    REQUIRE(game.pilediscard.length() == 0);

    SECTION("TryMoveToColumn (no checks) works")
    {
        REQUIRE(game.columns[0].revealedlength() == 1);
        REQUIRE(game.columns[0].unrevealedlength() == 0);
        REQUIRE(game.columns[1].revealedlength() == 1);
        REQUIRE(game.columns[1].unrevealedlength() == 1);
        REQUIRE(game.TryMoveToColumn(0, 1, 1, false));
        REQUIRE(game.columns[0].revealedlength() == 0);
        REQUIRE(game.columns[0].unrevealedlength() == 0);
        REQUIRE(game.columns[1].revealedlength() == 2);
        REQUIRE(game.columns[1].unrevealedlength() == 1);

        for (int i = 0; i < 4; i++)
        {
            game.columns[4].PushLeft(game.pile.PopLeft());
        }

        REQUIRE(game.columns[4].revealedlength() == 5);
        REQUIRE(game.columns[4].unrevealedlength() == 4);
        REQUIRE(game.columns[3].revealedlength() == 1);
        REQUIRE(game.columns[3].unrevealedlength() == 3);
        REQUIRE(game.TryMoveToColumn(4, 3, 3, false));
        REQUIRE(game.columns[4].revealedlength() == 2);
        REQUIRE(game.columns[4].unrevealedlength() == 4);
        REQUIRE(game.columns[3].revealedlength() == 4);
        REQUIRE(game.columns[3].unrevealedlength() == 3);

        for (int i = 0; i < 3; i++)
        {
            game.columns[6].PushLeft(game.pile.PopLeft());
        }

        REQUIRE(game.columns[6].revealedlength() == 4);
        REQUIRE(game.columns[6].unrevealedlength() == 6);
        REQUIRE(game.columns[5].revealedlength() == 1);
        REQUIRE(game.columns[5].unrevealedlength() == 5);
        REQUIRE(game.TryMoveToColumn(6, 4, 5, false));
        REQUIRE(game.columns[6].revealedlength() == 1); // Reveal() has happened
        REQUIRE(game.columns[6].unrevealedlength() == 5);
        REQUIRE(game.columns[5].revealedlength() == 5);
        REQUIRE(game.columns[5].unrevealedlength() == 5);
    }

    SECTION("TryMoveToColumn (no checks) fails for invalid input")
    {
        REQUIRE(!game.TryMoveToColumn(-1, 1, 0, false));
        REQUIRE(!game.TryMoveToColumn(0, 1, -1, false));
        REQUIRE(!game.TryMoveToColumn(COLUMN_COUNT, 1, 0, false));
        REQUIRE(!game.TryMoveToColumn(0, 1, COLUMN_COUNT, false));
        REQUIRE(!game.TryMoveToColumn(0, 0, 1, false));
        REQUIRE(!game.TryMoveToColumn(0, -1, 1, false));
        REQUIRE(!game.TryMoveToColumn(0, 1, 0, false));

        REQUIRE(game.columns[0].revealedlength() == 1);
        REQUIRE(game.columns[0].unrevealedlength() == 0);
        REQUIRE(game.columns[1].revealedlength() == 1);
        REQUIRE(game.columns[1].unrevealedlength() == 1);
        REQUIRE(!game.TryMoveToColumn(0, 2, 1, false));

        for (int i = 0; i < 4; i++)
        {
            game.columns[4].PushLeft(game.pile.PopLeft());
        }

        REQUIRE(game.columns[4].revealedlength() == 5);
        REQUIRE(game.columns[4].unrevealedlength() == 4);
        REQUIRE(game.columns[3].revealedlength() == 1);
        REQUIRE(game.columns[3].unrevealedlength() == 3);
        REQUIRE(!game.TryMoveToColumn(4, 6, 3, false));
    }

    SECTION("TryMoveToColumn checks function correctly")
    {
        for (int i = 0; i < COLUMN_COUNT; i++)
        {
            while (!game.columns[i].empty())
            {
                game.columns[i].PopLeft();
            }
        }

        game.columns[0].PushLeft(Card(Hearts, 5));
        game.columns[1].PushLeft(Card(Spades, 6));
        REQUIRE(game.columns[0].revealedlength() == 1);
        REQUIRE(game.columns[1].revealedlength() == 1);
        REQUIRE(game.TryMoveToColumn(0, 1, 1, true));
        REQUIRE(game.columns[0].revealedlength() == 0);
        REQUIRE(game.columns[1].revealedlength() == 2);
        REQUIRE((game.columns[1].PeekAt(0) == Card(Hearts, 5)));
        REQUIRE((game.columns[1].PeekAt(1) == Card(Spades, 6)));

        game.columns[4].PushLeft(Card(Hearts, 12));
        game.columns[4].PushLeft(Card(Spades, 11));
        game.columns[4].PushLeft(Card(Hearts, 10));
        game.columns[4].PushLeft(Card(Clubs, 9));
        game.columns[3].PushLeft(Card(Diamonds, 12));

        REQUIRE(game.columns[4].revealedlength() == 4);
        REQUIRE(game.columns[3].revealedlength() == 1);
        REQUIRE(game.TryMoveToColumn(4, 3, 3, true));
        REQUIRE(game.columns[4].revealedlength() == 1);
        REQUIRE(game.columns[3].revealedlength() == 4);

        game.columns[6].PushLeft(Card(Clubs, 4));
        game.columns[6].PushLeft(Card(Diamonds, 3));
        game.columns[6].PushLeft(Card(Spades, 2));
        game.columns[6].PushLeft(Card(Hearts, 1));
        game.columns[5].PushLeft(Card(Diamonds, 5));

        REQUIRE(game.columns[6].revealedlength() == 4);
        REQUIRE(game.columns[5].revealedlength() == 1);
        REQUIRE(game.TryMoveToColumn(6, 4, 5, true));
        REQUIRE(game.columns[6].revealedlength() == 0);
        REQUIRE(game.columns[5].revealedlength() == 5);

        REQUIRE((game.columns[5].PeekAt(3) == Card(Clubs, 4)));
        game.columns[2].PushLeft(Card(Spades, 5));

        REQUIRE(game.columns[5].revealedlength() == 5);
        REQUIRE(game.columns[2].revealedlength() == 1);
        REQUIRE(!game.TryMoveToColumn(5, 4, 2, true));
        REQUIRE(game.columns[5].revealedlength() == 5);
        REQUIRE(game.columns[2].revealedlength() == 1);

        REQUIRE((game.columns[5].PeekAt(3) == Card(Clubs, 4)));
        game.columns[2].PopLeft();
        game.columns[2].PushLeft(Card(Diamonds, 4));

        REQUIRE(game.columns[5].revealedlength() == 5);
        REQUIRE(game.columns[2].revealedlength() == 1);
        REQUIRE(!game.TryMoveToColumn(5, 4, 2, true));
        REQUIRE(game.columns[5].revealedlength() == 5);
        REQUIRE(game.columns[2].revealedlength() == 1);
    }
}

TEST_CASE("Game can be stored and read", "[Game]")
{
    Game game = Game();
    SaveData savebuffer[SAVESTATE_LEN];

    SECTION("GetWriteLength is correct")
    {
        REQUIRE(game.GetWriteLength() == SAVESTATE_LEN);
    }

    SECTION("WriteToArray has correct length")
    {
        for (int i = 0; i < SUIT_COUNT; i++)
        {
            // no cards
            REQUIRE(game.suits[i].WriteToArray(savebuffer) == 1);
            REQUIRE(savebuffer[0].value == 0);
        }

        for (int i = 0; i < COLUMN_COUNT; i++)
        {
            // header length 2 + `i` unrevealed cards + 1 revealed card
            REQUIRE(game.columns[i].WriteToArray(savebuffer) == 2 + i + 1);
            REQUIRE(savebuffer[0].value == i);
            REQUIRE(savebuffer[i + 1].value == 1);
        }

        REQUIRE(game.WriteToArray(savebuffer) == SAVESTATE_LEN);
    }

    SECTION("ReadToArray works on written input")
    {
        for (int i = 0; i < SUIT_COUNT; i++)
        {
            // no cards
            REQUIRE(game.suits[i].WriteToArray(savebuffer) == 1);
            REQUIRE(game.suits[i].ReadFromArray(savebuffer) == 1);
        }

        for (int i = 0; i < COLUMN_COUNT; i++)
        {
            // header length 2 + `i` unrevealed cards + 1 revealed card
            const int count = 2 + i + 1;
            REQUIRE(game.columns[i].WriteToArray(savebuffer) == count);
            REQUIRE(game.columns[i].ReadFromArray(savebuffer) == count);
        }

        REQUIRE(game.WriteToArray(savebuffer) == SAVESTATE_LEN);
        REQUIRE(game.ReadFromArray(savebuffer) == SAVESTATE_LEN);
    }
}
