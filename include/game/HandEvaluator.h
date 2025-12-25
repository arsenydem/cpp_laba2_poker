#ifndef HANDEVALUATOR_H
#define HANDEVALUATOR_H

#include "../interfaces/IHandEvaluator.h"
#include "../core/Card.h"
#include <vector>
#include <unordered_map>

enum {
    HAND_ROYAL_FLUSH = 9000000,
    HAND_STRAIGHT_FLUSH = 8000000,
    HAND_FOUR_OF_A_KIND = 7000000,
    HAND_FULL_HOUSE = 6000000,
    HAND_FLUSH = 5000000,
    HAND_STRAIGHT = 4000000,
    HAND_THREE_OF_A_KIND = 3000000,
    HAND_TWO_PAIR = 2000000,
    HAND_ONE_PAIR = 1000000,
    HAND_HIGH_CARD = 0
};

class HandEvaluator : public IHandEvaluator {
private:
    std::vector<Card> getBestFiveCards(std::vector<Card>& cards);
    bool isRoyalFlush(std::vector<Card>& cards);
    bool isStraightFlush(std::vector<Card>& cards);
    bool isFourOfAKind(std::vector<Card>& cards);
    bool isFullHouse(std::vector<Card>& cards);
    bool isFlush(std::vector<Card>& cards);
    bool isStraight(std::vector<Card>& cards);
    bool isThreeOfAKind(std::vector<Card>& cards);
    bool isTwoPair(std::vector<Card>& cards);
    bool isOnePair(std::vector<Card>& cards);
    int getHandValue(std::vector<Card>& cards);
    float calculateEquityPreFlop(std::vector<Card>& playerHand, int numOpponents);
    float calculateEquityPostFlop(std::vector<Card>& playerHand, 
                                  std::vector<Card>& communityCards, 
                                  int numOpponents);
    float calculateEquityPostTurn(std::vector<Card>& playerHand,
                                  std::vector<Card>& communityCards,
                                  int numOpponents);
    void generateCombinations(std::vector<Card>& availableCards, 
                             int k, 
                             std::vector<std::vector<Card>>& result,
                             std::vector<Card>& current,
                             int start);
    bool improvesToHandType(std::vector<Card>& hand,
                           std::vector<Card>& community,
                           Card testCard,
                           int targetHandType);
    std::vector<Card> getAvailableCards(std::vector<Card>& hand,
                                       std::vector<Card>& community,
                                       std::vector<std::vector<Card>> opponentHands);

public:
    int evaluateHand(std::vector<Card>& hand) override;
    float calculateWinProbability(
        std::vector<Card>& playerHand,
        std::vector<Card>& communityCards,
        int numOpponents
    ) override;
    std::vector<Card> findOuts(
        std::vector<Card>& hand,
        std::vector<Card>& community,
        std::vector<std::vector<Card>>& opponentHands
    ) override;
    int evaluateBestHand(
        std::vector<Card>& playerHand,
        std::vector<Card>& communityCards
    ) override;
};

#endif
