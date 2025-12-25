#include "../../include/core/Card.h"

Card::Card(int r, int s) : rank_(r), suit_(s) {}

Card::Card() : rank_(RANK_TWO), suit_(SUIT_SPADES) {}

std::string Card::toString() {
    std::string result;
    
    if (rank_ >= RANK_TWO && rank_ <= RANK_TEN) {
        result = std::to_string(rank_);
    } else {
        switch (rank_) {
            case RANK_JACK:
                result = "J";
                break;
            case RANK_QUEEN:
                result = "Q";
                break;
            case RANK_KING:
                result = "K";
                break;
            case RANK_ACE:
                result = "A";
                break;
            default:
                result = "?";
                break;
        }
    }
    
    switch (suit_) {
        case SUIT_SPADES:
            result += "♠";
            break;
        case SUIT_HEARTS:
            result += "♥";
            break;
        case SUIT_DIAMONDS:
            result += "♦";
            break;
        case SUIT_CLUBS:
            result += "♣";
            break;
        default:
            result += "?";
            break;
    }
    
    return result;
}

bool Card::operator==(Card other) {
    return rank_ == other.rank_ && suit_ == other.suit_;
}

bool Card::operator!=(Card other) {
    return !(*this == other);
}

bool Card::operator<(Card other) {
    if (rank_ != other.rank_) {
        return rank_ < other.rank_;
    }
    return suit_ < other.suit_;
}



