#include "catch2/catch_amalgamated.hpp"

#include "../src/cardll.hpp"

TEST_CASE("Card queue stores Cards", "[CardLL]")
{
    DefineCardDeck();

    CardLL queue = CardLL();
    queue.PushRight(Card((Suit)0, (Rank)1));
    queue.PushRight(Card((Suit)1, (Rank)13));
    queue.PushRight(Card((Suit)2, (Rank)10));
    queue.PushRight(Card((Suit)3, (Rank)10));

    SECTION("items can be peeked in order")
    {
        REQUIRE((queue.Peek() == Card((Suit)0, (Rank)1)));
        REQUIRE((queue.PeekAt(0) == Card((Suit)0, (Rank)1)));
        REQUIRE((queue.PeekAt(1) == Card((Suit)1, (Rank)13)));
        REQUIRE((queue.PeekAt(2) == Card((Suit)2, (Rank)10)));
        REQUIRE((queue.PeekAt(3) == Card((Suit)3, (Rank)10)));
        REQUIRE((queue.PeekAt(4) == Card()));
    }

    SECTION("items can be popped in order")
    {
        REQUIRE((queue.PopLeft() == Card((Suit)0, (Rank)1)));
        REQUIRE((queue.PopLeft() == Card((Suit)1, (Rank)13)));
        REQUIRE((queue.PopLeft() == Card((Suit)2, (Rank)10)));
        REQUIRE((queue.PopLeft() == Card((Suit)3, (Rank)10)));
        REQUIRE((queue.PopLeft() == Card()));
    }

    SECTION("items can be popped out of order")
    {
        REQUIRE((queue.PopAt(4) == Card()));
        REQUIRE((queue.PopAt(2) == Card((Suit)2, (Rank)10)));
        REQUIRE((queue.PopAt(0) == Card((Suit)0, (Rank)1)));
        REQUIRE((queue.PopAt(2) == Card()));
        REQUIRE((queue.PopAt(1) == Card((Suit)3, (Rank)10)));
        REQUIRE((queue.PopAt(0) == Card((Suit)1, (Rank)13)));
        REQUIRE((queue.PopAt(0) == Card()));
    }

    SECTION("items can be pushed to the end of the queue")
    {
        REQUIRE((queue.PopLeft() == Card((Suit)0, (Rank)1)));
        queue.PushRight(Card((Suit)0, (Rank)4));
        REQUIRE((queue.PopLeft() == Card((Suit)1, (Rank)13)));
        REQUIRE((queue.PopLeft() == Card((Suit)2, (Rank)10)));
        queue.PushRight(Card((Suit)1, (Rank)9));
        queue.PushRight(Card((Suit)1, (Rank)10));
        REQUIRE((queue.PopLeft() == Card((Suit)3, (Rank)10)));
        REQUIRE((queue.PopLeft() == Card((Suit)0, (Rank)4)));
        REQUIRE((queue.PopLeft() == Card((Suit)1, (Rank)9)));
        REQUIRE((queue.PopLeft() == Card((Suit)1, (Rank)10)));
        REQUIRE((queue.PopLeft() == Card()));
    }

    SECTION("length and empty are updated correctly")
    {
        REQUIRE(queue.length() == 4);
        REQUIRE(!queue.empty());
        queue.PopLeft();
        REQUIRE(queue.length() == 3);
        REQUIRE(!queue.empty());
        queue.PushRight(Card((Suit)0, (Rank)4));
        REQUIRE(queue.length() == 4);
        REQUIRE(!queue.empty());
        queue.PopLeft();
        REQUIRE(queue.length() == 3);
        REQUIRE(!queue.empty());
        queue.PopLeft();
        REQUIRE(queue.length() == 2);
        REQUIRE(!queue.empty());
        queue.PushRight(Card((Suit)1, (Rank)9));
        queue.PushRight(Card((Suit)1, (Rank)10));
        REQUIRE(queue.length() == 4);
        REQUIRE(!queue.empty());
        queue.PopLeft();
        queue.PopLeft();
        queue.PopLeft();
        queue.PopLeft();
        REQUIRE(queue.length() == 0);
        REQUIRE(queue.empty());
        queue.PopLeft();
        REQUIRE(queue.length() == 0);
        REQUIRE(queue.empty());
    }

    SECTION("shuffle works")
    {
        REQUIRE(queue.length() == 4);
        queue.ShuffleCards();
        REQUIRE(queue.length() == 4);
    }

    SECTION("fillcarddeck works")
    {
        while (!queue.empty())
            queue.PopLeft();
        REQUIRE(queue.length() == 0);
        FillCardDeck(&queue);
        REQUIRE(queue.length() == 52);
    }
}

TEST_CASE("Card stack stores Cards", "[Card]")
{
    DefineCardDeck();

    CardLL stack = CardLL();
    stack.PushLeft(Card((Suit)0, (Rank)1));
    stack.PushLeft(Card((Suit)1, (Rank)13));
    stack.PushLeft(Card((Suit)2, (Rank)10));
    stack.PushLeft(Card((Suit)3, (Rank)10));

    SECTION("items can be peeked and popped in order")
    {
        REQUIRE((stack.Peek() == Card((Suit)3, (Rank)10)));
        REQUIRE((stack.PopLeft() == Card((Suit)3, (Rank)10)));
        REQUIRE((stack.Peek() == Card((Suit)2, (Rank)10)));
        REQUIRE((stack.PopLeft() == Card((Suit)2, (Rank)10)));
        REQUIRE((stack.Peek() == Card((Suit)1, (Rank)13)));
        REQUIRE((stack.PopLeft() == Card((Suit)1, (Rank)13)));
        REQUIRE((stack.Peek() == Card((Suit)0, (Rank)1)));
        REQUIRE((stack.PopLeft() == Card((Suit)0, (Rank)1)));
        REQUIRE((stack.Peek() == Card()));
    }

    SECTION("items can be pushed to the top of the stack")
    {
        REQUIRE((stack.PopLeft() == Card((Suit)3, (Rank)10)));
        stack.PushLeft(Card((Suit)0, (Rank)4));
        REQUIRE((stack.PopLeft() == Card((Suit)0, (Rank)4)));
        REQUIRE((stack.PopLeft() == Card((Suit)2, (Rank)10)));
        stack.PushLeft(Card((Suit)1, (Rank)9));
        stack.PushLeft(Card((Suit)1, (Rank)10));
        REQUIRE((stack.PopLeft() == Card((Suit)1, (Rank)10)));
        REQUIRE((stack.PopLeft() == Card((Suit)1, (Rank)9)));
        REQUIRE((stack.PopLeft() == Card((Suit)1, (Rank)13)));
        REQUIRE((stack.PopLeft() == Card((Suit)0, (Rank)1)));
        REQUIRE((stack.PopLeft() == Card()));
    }

    SECTION("length and empty are updated correctly")
    {
        REQUIRE(stack.length() == 4);
        REQUIRE(!stack.empty());
        stack.PopLeft();
        REQUIRE(stack.length() == 3);
        REQUIRE(!stack.empty());
        stack.PushLeft(Card((Suit)0, (Rank)4));
        REQUIRE(stack.length() == 4);
        REQUIRE(!stack.empty());
        stack.PopLeft();
        REQUIRE(stack.length() == 3);
        REQUIRE(!stack.empty());
        stack.PopLeft();
        REQUIRE(stack.length() == 2);
        REQUIRE(!stack.empty());
        stack.PushLeft(Card((Suit)1, (Rank)9));
        stack.PushLeft(Card((Suit)1, (Rank)10));
        REQUIRE(stack.length() == 4);
        REQUIRE(!stack.empty());
        stack.PopLeft();
        stack.PopLeft();
        stack.PopLeft();
        stack.PopLeft();
        REQUIRE(stack.length() == 0);
        REQUIRE(stack.empty());
        stack.PopLeft();
        REQUIRE(stack.length() == 0);
        REQUIRE(stack.empty());
    }

    SECTION("MoveMany moves every items")
    {
        CardLL s2 = CardLL();
        s2.PushLeft(Card((Suit)1, (Rank)2));
        stack.MoveMany(4, &s2);
        REQUIRE(stack.empty());
        REQUIRE((s2.PopLeft() == Card((Suit)3, (Rank)10)));
        REQUIRE((s2.PopLeft() == Card((Suit)2, (Rank)10)));
        REQUIRE((s2.PopLeft() == Card((Suit)1, (Rank)13)));
        REQUIRE((s2.PopLeft() == Card((Suit)0, (Rank)1)));
        REQUIRE((s2.PopLeft() == Card((Suit)1, (Rank)2)));
        REQUIRE(s2.empty());
    }

    SECTION("MoveMany moves to empty stack")
    {
        CardLL s2 = CardLL();
        stack.MoveMany(2, &s2);
        REQUIRE((stack.PopLeft() == Card((Suit)1, (Rank)13)));
        REQUIRE((stack.PopLeft() == Card((Suit)0, (Rank)1)));
        REQUIRE(stack.empty());
        REQUIRE((s2.PopLeft() == Card((Suit)3, (Rank)10)));
        REQUIRE((s2.PopLeft() == Card((Suit)2, (Rank)10)));
        REQUIRE(s2.empty());
    }

    SECTION("MoveMany ignores extra items")
    {
        CardLL s2 = CardLL();
        s2.PushLeft(Card((Suit)1, (Rank)2));
        stack.MoveMany(10, &s2);
        REQUIRE(stack.empty());
        REQUIRE((s2.PopLeft() == Card((Suit)3, (Rank)10)));
        REQUIRE((s2.PopLeft() == Card((Suit)2, (Rank)10)));
        REQUIRE((s2.PopLeft() == Card((Suit)1, (Rank)13)));
        REQUIRE((s2.PopLeft() == Card((Suit)0, (Rank)1)));
        REQUIRE((s2.PopLeft() == Card((Suit)1, (Rank)2)));
        REQUIRE(s2.empty());
    }
}
