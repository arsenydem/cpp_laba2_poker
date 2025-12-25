#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <memory>

class Deck {
    std::vector<Card> cards_;

public:
    Deck();
    void shuffle();
    Card draw();
    void reset();
    bool isEmpty() { return cards_.empty(); }
    std::vector<Card>& getRemainingCards() { return cards_; }
    std::vector<Card> getRemainingCardsCopy() { return cards_; }
};

#endif



