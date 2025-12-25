#include "../../include/core/Deck.h"
#include <algorithm>
#include <random>
#include <chrono>

Deck::Deck() {
    reset();
}

void Deck::reset() {
    cards_.clear();
    for (int suit = SUIT_SPADES; suit <= SUIT_CLUBS; ++suit) {
        for (int rank = RANK_TWO; rank <= RANK_ACE; ++rank) {
            cards_.push_back(Card(rank, suit));
        }
    }
}

void Deck::shuffle() {
    if (cards_.empty()) {
        return;
    }
    
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    
    std::shuffle(cards_.begin(), cards_.end(), rng);
}

Card Deck::draw() {
    if (cards_.empty()) {
        return Card();
    }
    
    Card card = cards_.back();
    cards_.pop_back();
    return card;
}

