#ifndef IHANDEVALUATOR_H
#define IHANDEVALUATOR_H

#include "../core/Card.h"
#include <vector>

class IHandEvaluator {
public:
    virtual ~IHandEvaluator() = default;
    virtual int evaluateHand(std::vector<Card>& hand) = 0;
    virtual float calculateWinProbability(
        std::vector<Card>& playerHand,
        std::vector<Card>& communityCards,
        int numOpponents
    ) = 0;
    virtual std::vector<Card> findOuts(
        std::vector<Card>& hand,
        std::vector<Card>& community,
        std::vector<std::vector<Card>>& opponentHands
    ) = 0;
    virtual int evaluateBestHand(
        std::vector<Card>& playerHand,
        std::vector<Card>& communityCards
    ) = 0;
};

#endif

