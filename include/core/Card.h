#ifndef CARD_H
#define CARD_H

#include <string>

enum {
    RANK_TWO = 2,
    RANK_THREE = 3,
    RANK_FOUR = 4,
    RANK_FIVE = 5,
    RANK_SIX = 6,
    RANK_SEVEN = 7,
    RANK_EIGHT = 8,
    RANK_NINE = 9,
    RANK_TEN = 10,
    RANK_JACK = 11,
    RANK_QUEEN = 12,
    RANK_KING = 13,
    RANK_ACE = 14
};

enum {
    SUIT_SPADES = 0,
    SUIT_HEARTS = 1,
    SUIT_DIAMONDS = 2,
    SUIT_CLUBS = 3
};

class Card {
    int rank_;
    int suit_;

public:
    Card(int r, int s);
    Card();
    std::string toString();
    int getRank() { return rank_; }
    int getSuit() { return suit_; }
    bool operator==(Card other);
    bool operator!=(Card other);
    bool operator<(Card other);
};

#endif



