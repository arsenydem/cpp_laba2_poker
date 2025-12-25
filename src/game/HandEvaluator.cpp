#include "../../include/game/HandEvaluator.h"
#include <algorithm>
#include <unordered_set>
#include <cmath>
#include <map>

int HandEvaluator::evaluateHand( std::vector<Card>& hand) {
    if (hand.size() < 5) {
        return 0;
    }
    return getHandValue(hand);
}

int HandEvaluator::evaluateBestHand( std::vector<Card>& playerHand, 
 std::vector<Card>& communityCards)  {
    std::vector<Card> allCards;
    allCards.insert(allCards.end(), playerHand.begin(), playerHand.end());
    allCards.insert(allCards.end(), communityCards.begin(), communityCards.end());
    
    if (allCards.size() < 5) {
        return 0;
    }
    
    std::vector<Card> bestFive = getBestFiveCards(allCards);
    return getHandValue(bestFive);
}

std::vector<Card> HandEvaluator::getBestFiveCards( std::vector<Card>& cards)  {
    if (cards.size() <= 5) {
        std::vector<Card> result = cards;
        std::sort(result.begin(), result.end(), [](Card a, Card b) {
            return a.getRank() > b.getRank();
        });
        return result;
    }
    
    std::vector<std::vector<Card>> combinations;
    std::vector<Card> current;
    generateCombinations(cards, 5, combinations, current, 0);
    
    int maxValue = 0;
    std::vector<Card> bestCombination;
    
    for ( auto& combo : combinations) {
        int value = getHandValue(combo);
        if (value > maxValue) {
            maxValue = value;
            bestCombination = combo;
        }
    }
    
    return bestCombination;
}

void HandEvaluator::generateCombinations( std::vector<Card>& availableCards,
                                         int k,
                                         std::vector<std::vector<Card>>& result,
                                         std::vector<Card>& current,
                                         int start)  {
    if (current.size() == k) {
        result.push_back(current);
        return;
    }
    
    for (int i = start; i < availableCards.size(); ++i) {
        current.push_back(availableCards[i]);
        generateCombinations(availableCards, k, result, current, i + 1);
        current.pop_back();
    }
}

bool HandEvaluator::isRoyalFlush( std::vector<Card>& cards)  {
    if (cards.size() != 5) return false;
    
    std::vector<int> ranks;
    int suit = cards[0].getSuit();
    
    for ( auto& card : cards) {
        if (card.getSuit() != suit) return false;
        ranks.push_back(card.getRank());
    }
    
    std::sort(ranks.begin(), ranks.end());
    return ranks[0] == RANK_TEN && ranks[1] == RANK_JACK && 
           ranks[2] == RANK_QUEEN && ranks[3] == RANK_KING && ranks[4] == RANK_ACE;
}

bool HandEvaluator::isStraightFlush( std::vector<Card>& cards)  {
    if (cards.size() != 5) return false;
    
    int suit = cards[0].getSuit();
    std::vector<int> ranks;
    
    for ( auto& card : cards) {
        if (card.getSuit() != suit) return false;
        ranks.push_back(card.getRank());
    }
    
    std::sort(ranks.begin(), ranks.end());
    
    bool isStraight = true;
    for (size_t i = 1; i < ranks.size(); ++i) {
        if (ranks[i] != ranks[i-1] + 1) {
            isStraight = false;
            break;
        }
    }
    
    if (!isStraight && ranks[0] == RANK_TWO && ranks[1] == RANK_THREE &&
        ranks[2] == RANK_FOUR && ranks[3] == RANK_FIVE && ranks[4] == RANK_ACE) {
        isStraight = true;
    }
    
    return isStraight;
}

bool HandEvaluator::isFourOfAKind( std::vector<Card>& cards)  {
    if (cards.size() != 5) return false;
    
    std::map<int, int> rankCount;
    for ( auto& card : cards) {
        rankCount[card.getRank()]++;
    }
    
    for ( auto& pair : rankCount) {
        if (pair.second == 4) return true;
    }
    return false;
}

bool HandEvaluator::isFullHouse( std::vector<Card>& cards)  {
    if (cards.size() != 5) return false;
    
    std::map<int, int> rankCount;
    for ( auto& card : cards) {
        rankCount[card.getRank()]++;
    }
    
    bool hasThree = false, hasPair = false;
    for ( auto& pair : rankCount) {
        if (pair.second == 3) hasThree = true;
        if (pair.second == 2) hasPair = true;
    }
    
    return hasThree && hasPair;
}

bool HandEvaluator::isFlush( std::vector<Card>& cards)  {
    if (cards.size() != 5) return false;
    
    int suit = cards[0].getSuit();
    for ( auto& card : cards) {
        if (card.getSuit() != suit) return false;
    }
    return true;
}

bool HandEvaluator::isStraight( std::vector<Card>& cards)  {
    if (cards.size() != 5) return false;
    
    std::vector<int> ranks;
    for ( auto& card : cards) {
        ranks.push_back(card.getRank());
    }
    std::sort(ranks.begin(), ranks.end());
    
    bool isStraight = true;
    for (size_t i = 1; i < ranks.size(); ++i) {
        if (ranks[i] != ranks[i-1] + 1) {
            isStraight = false;
            break;
        }
    }
    
    if (!isStraight && ranks[0] == RANK_TWO && ranks[1] == RANK_THREE &&
        ranks[2] == RANK_FOUR && ranks[3] == RANK_FIVE && ranks[4] == RANK_ACE) {
        isStraight = true;
    }
    
    return isStraight;
}

bool HandEvaluator::isThreeOfAKind( std::vector<Card>& cards)  {
    if (cards.size() != 5) return false;
    
    std::map<int, int> rankCount;
    for ( auto& card : cards) {
        rankCount[card.getRank()]++;
    }
    
    for ( auto& pair : rankCount) {
        if (pair.second == 3) return true;
    }
    return false;
}

bool HandEvaluator::isTwoPair( std::vector<Card>& cards)  {
    if (cards.size() != 5) return false;
    
    std::map<int, int> rankCount;
    for ( auto& card : cards) {
        rankCount[card.getRank()]++;
    }
    
    int pairCount = 0;
    for ( auto& pair : rankCount) {
        if (pair.second == 2) pairCount++;
    }
    
    return pairCount == 2;
}

bool HandEvaluator::isOnePair( std::vector<Card>& cards)  {
    if (cards.size() != 5) return false;
    
    std::map<int, int> rankCount;
    for ( auto& card : cards) {
        rankCount[card.getRank()]++;
    }
    
    int pairCount = 0;
    for ( auto& pair : rankCount) {
        if (pair.second == 2) pairCount++;
    }
    
    return pairCount == 1;
}

int HandEvaluator::getHandValue( std::vector<Card>& cards)  {
    if (cards.size() != 5) return 0;
    
    std::vector<Card> sortedCards = cards;
    std::sort(sortedCards.begin(), sortedCards.end(), []( Card& a,  Card& b) {
        return a.getRank() > b.getRank();
    });
    
    if (isRoyalFlush(sortedCards)) {
        return HAND_ROYAL_FLUSH;
    }
    
    if (isStraightFlush(sortedCards)) {
        return HAND_STRAIGHT_FLUSH + sortedCards[0].getRank();
    }
    
    if (isFourOfAKind(sortedCards)) {
        std::map<int, int> rankCount;
        for ( auto& card : sortedCards) {
            rankCount[card.getRank()]++;
        }
        int fourRank = 0, kicker = 0;
        for ( auto& pair : rankCount) {
            if (pair.second == 4) fourRank = pair.first;
            else kicker = pair.first;
        }
        return HAND_FOUR_OF_A_KIND + fourRank * 100 + kicker;
    }
    
    if (isFullHouse(sortedCards)) {
        std::map<int, int> rankCount;
        for ( auto& card : sortedCards) {
            rankCount[card.getRank()]++;
        }
        int threeRank = 0, pairRank = 0;
        for ( auto& pair : rankCount) {
            if (pair.second == 3) threeRank = pair.first;
            if (pair.second == 2) pairRank = pair.first;
        }
        return HAND_FULL_HOUSE + threeRank * 100 + pairRank;
    }
    
    if (isFlush(sortedCards)) {
        int value = HAND_FLUSH;
        for ( auto& card : sortedCards) {
            value = value * 100 + card.getRank();
        }
        return value;
    }
    
    if (isStraight(sortedCards)) {
        int highCard = sortedCards[0].getRank();
        if (sortedCards[0].getRank() == RANK_ACE && sortedCards[1].getRank() == RANK_FIVE) {
            highCard = RANK_FIVE;
        }
        return HAND_STRAIGHT + highCard;
    }
    
    if (isThreeOfAKind(sortedCards)) {
        std::map<int, int> rankCount;
        for ( auto& card : sortedCards) {
            rankCount[card.getRank()]++;
        }
        int threeRank = 0;
        std::vector<int> kickers;
        for ( auto& pair : rankCount) {
            if (pair.second == 3) threeRank = pair.first;
            else kickers.push_back(pair.first);
        }
        std::sort(kickers.begin(), kickers.end(), std::greater<int>());
        return HAND_THREE_OF_A_KIND + threeRank * 10000 + 
               (kickers.size() > 0 ? kickers[0] * 100 : 0) + 
               (kickers.size() > 1 ? kickers[1] : 0);
    }
    
    if (isTwoPair(sortedCards)) {
        std::map<int, int> rankCount;
        for ( auto& card : sortedCards) {
            rankCount[card.getRank()]++;
        }
        std::vector<int> pairs;
        int kicker = 0;
        for ( auto& pair : rankCount) {
            if (pair.second == 2) pairs.push_back(pair.first);
            else kicker = pair.first;
        }
        std::sort(pairs.begin(), pairs.end(), std::greater<int>());
        return HAND_TWO_PAIR + pairs[0] * 10000 + pairs[1] * 100 + kicker;
    }
    
    if (isOnePair(sortedCards)) {
        std::map<int, int> rankCount;
        for ( auto& card : sortedCards) {
            rankCount[card.getRank()]++;
        }
        int pairRank = 0;
        std::vector<int> kickers;
        for ( auto& pair : rankCount) {
            if (pair.second == 2) pairRank = pair.first;
            else kickers.push_back(pair.first);
        }
        std::sort(kickers.begin(), kickers.end(), std::greater<int>());
        return HAND_ONE_PAIR + pairRank * 1000000 + 
               (kickers.size() > 0 ? kickers[0] * 10000 : 0) +
               (kickers.size() > 1 ? kickers[1] * 100 : 0) +
               (kickers.size() > 2 ? kickers[2] : 0);
    }
    
    int value = HAND_HIGH_CARD;
    for ( auto& card : sortedCards) {
        value = value * 100 + card.getRank();
    }
    return value;
}

std::vector<Card> HandEvaluator::getAvailableCards(std::vector<Card>& hand,
                                                    std::vector<Card>& community,
                                                    std::vector<std::vector<Card>> opponentHands) {
    std::vector<Card> allUsed;
    allUsed.insert(allUsed.end(), hand.begin(), hand.end());
    allUsed.insert(allUsed.end(), community.begin(), community.end());
    
    for ( auto& oppHand : opponentHands) {
        allUsed.insert(allUsed.end(), oppHand.begin(), oppHand.end());
    }
    
    std::vector<Card> available;
    for (int suit = SUIT_SPADES; suit <= SUIT_CLUBS; ++suit) {
        for (int rank = RANK_TWO; rank <= RANK_ACE; ++rank) {
            Card card(rank, suit);
            bool isUsed = false;
            for ( auto& used : allUsed) {
                if (used == card) {
                    isUsed = true;
                    break;
                }
            }
            if (!isUsed) {
                available.push_back(card);
            }
        }
    }
    
    return available;
}

float HandEvaluator::calculateWinProbability( std::vector<Card>& playerHand,
 std::vector<Card>& communityCards,
                                            int numOpponents) {
    if (playerHand.size() != 2) {
        return 0.0f;
    }
    
    if (communityCards.empty()) {
        return calculateEquityPreFlop(playerHand, numOpponents);
    } else if (communityCards.size() == 3) {
        return calculateEquityPostFlop(playerHand, communityCards, numOpponents);
    } else if (communityCards.size() == 4) {
        return calculateEquityPostTurn(playerHand, communityCards, numOpponents);
    } else if (communityCards.size() == 5) {
        int playerHandValue = evaluateBestHand(playerHand, communityCards);
        if (playerHandValue >= HAND_FULL_HOUSE) return 0.9f;
        if (playerHandValue >= HAND_FLUSH) return 0.7f;
        if (playerHandValue >= HAND_STRAIGHT) return 0.6f;
        if (playerHandValue >= HAND_THREE_OF_A_KIND) return 0.5f;
        return 0.3f;
    }
    
    return 0.0f;
}

float HandEvaluator::calculateEquityPreFlop( std::vector<Card>& playerHand, int numOpponents)  {
    int rank1 = playerHand[0].getRank();
    int rank2 = playerHand[1].getRank();
    bool suited = playerHand[0].getSuit() == playerHand[1].getSuit();
    bool paired = rank1 == rank2;
    
    float baseEquity = 0.0f;
    
    if (paired) {
        if (rank1 >= RANK_TEN) baseEquity = 0.85f;
        else if (rank1 >= RANK_SEVEN) baseEquity = 0.75f;
        else baseEquity = 0.65f;
    } else if (rank1 == RANK_ACE || rank2 == RANK_ACE) {
        int otherRank = (rank1 == RANK_ACE) ? rank2 : rank1;
        if (otherRank >= RANK_TEN) baseEquity = suited ? 0.75f : 0.70f;
        else if (otherRank >= RANK_SEVEN) baseEquity = suited ? 0.65f : 0.60f;
        else baseEquity = suited ? 0.55f : 0.50f;
    } else {
        int highRank = std::max(rank1, rank2);
        int lowRank = std::min(rank1, rank2);
        bool connected = std::abs(highRank - lowRank) <= 4;
        
        if (highRank >= RANK_KING) {
            baseEquity = (suited && connected) ? 0.60f : (suited ? 0.55f : 0.50f);
        } else if (highRank >= RANK_TEN) {
            baseEquity = (suited && connected) ? 0.50f : (suited ? 0.45f : 0.40f);
        } else {
            baseEquity = (suited && connected) ? 0.40f : (suited ? 0.35f : 0.30f);
        }
    }
    
    float equity = baseEquity;
    for (int i = 0; i < numOpponents; ++i) {
        equity *= 0.85f;
    }
    
    return std::max(0.0f, std::min(1.0f, equity));
}

float HandEvaluator::calculateEquityPostFlop( std::vector<Card>& playerHand,
 std::vector<Card>& communityCards,
                                             int numOpponents)  {
    std::vector<Card> availableCards = getAvailableCards(playerHand, communityCards, {});
    
    if (availableCards.size() < 2) return 0.0f;
    
    std::vector<std::vector<Card>> turnRiverCombos;
    std::vector<Card> current;
    generateCombinations(availableCards, 2, turnRiverCombos, current, 0);
    
 int MAX_COMBOS = 1000;
    if (turnRiverCombos.size() > MAX_COMBOS) {
        turnRiverCombos.resize(MAX_COMBOS);
    }
    
    int wins = 0;
    int ties = 0;
    int total = 0;
    
    int playerBestHand = evaluateBestHand(playerHand, communityCards);
    
    for ( auto& combo : turnRiverCombos) {
        std::vector<Card> fullBoard = communityCards;
        fullBoard.insert(fullBoard.end(), combo.begin(), combo.end());
        
        int playerHandValue = evaluateBestHand(playerHand, fullBoard);
        
        bool playerWins = true;
        bool hasTie = false;
        
        for (int opp = 0; opp < numOpponents; ++opp) {
            std::vector<Card> remainingForOpp = availableCards;
            for ( auto& card : combo) {
                remainingForOpp.erase(
                    std::remove_if(remainingForOpp.begin(), remainingForOpp.end(),
                        [&card]( Card& c) { return c == card; }),
                    remainingForOpp.end()
                );
            }
            
            if (remainingForOpp.size() < 2) continue;
            
            std::vector<Card> oppHand = {remainingForOpp[0], remainingForOpp[1]};
            int oppHandValue = evaluateBestHand(oppHand, fullBoard);
            
            if (oppHandValue > playerHandValue) {
                playerWins = false;
                break;
            } else if (oppHandValue == playerHandValue) {
                hasTie = true;
            }
        }
        
        if (playerWins && !hasTie) {
            wins++;
        } else if (hasTie) {
            ties++;
        }
        total++;
    }
    
    if (total == 0) return 0.0f;
    
    return static_cast<float>(wins + ties * 0.5f) / total;
}

float HandEvaluator::calculateEquityPostTurn( std::vector<Card>& playerHand,
 std::vector<Card>& communityCards,
                                             int numOpponents)  {
    std::vector<Card> availableCards = getAvailableCards(playerHand, communityCards, {});
    
    if (availableCards.empty()) return 0.0f;
    
    int wins = 0;
    int ties = 0;
    int total = 0;
    
    int playerCurrentHand = evaluateBestHand(playerHand, communityCards);
    
    for ( auto& riverCard : availableCards) {
        std::vector<Card> fullBoard = communityCards;
        fullBoard.push_back(riverCard);
        
        int playerHandValue = evaluateBestHand(playerHand, fullBoard);
        
        bool playerWins = true;
        bool hasTie = false;
        
        std::vector<Card> remainingForOpp = availableCards;
        remainingForOpp.erase(
            std::remove_if(remainingForOpp.begin(), remainingForOpp.end(),
                [&riverCard]( Card& c) { return c == riverCard; }),
            remainingForOpp.end()
        );
        
        for (int opp = 0; opp < numOpponents && remainingForOpp.size() >= 2; ++opp) {
            std::vector<Card> oppHand = {remainingForOpp[0], remainingForOpp[1]};
            int oppHandValue = evaluateBestHand(oppHand, fullBoard);
            
            if (oppHandValue > playerHandValue) {
                playerWins = false;
                break;
            } else if (oppHandValue == playerHandValue) {
                hasTie = true;
            }
            
            remainingForOpp.erase(remainingForOpp.begin(), remainingForOpp.begin() + 2);
        }
        
        if (playerWins && !hasTie) {
            wins++;
        } else if (hasTie) {
            ties++;
        }
        total++;
    }
    
    if (total == 0) return 0.0f;
    
    return static_cast<float>(wins + ties * 0.5f) / total;
}

bool HandEvaluator::improvesToHandType(std::vector<Card>& hand,
                                       std::vector<Card>& community,
                                       Card testCard,
                                      int targetHandType)  {
    std::vector<Card> testBoard = community;
    testBoard.push_back(testCard);
    
    int currentValue = evaluateBestHand(hand, community);
    int testValue = evaluateBestHand(hand, testBoard);
    
    int currentType = (currentValue / 1000000) * 1000000;
    int testType = (testValue / 1000000) * 1000000;
    
    return testType >= targetHandType && testType > currentType;
}

std::vector<Card> HandEvaluator::findOuts( std::vector<Card>& hand,
 std::vector<Card>& community,
 std::vector<std::vector<Card>>& opponentHands) {
    if (hand.size() != 2) {
        return {};
    }
    
    std::vector<Card> availableCards = getAvailableCards(hand, community, opponentHands);
    
    if (availableCards.empty()) {
        return {};
    }
    
    int currentHandValue = evaluateBestHand(hand, community);
    int currentHandType = (currentHandValue / 1000000) * 1000000;
    
    std::unordered_map<int, std::vector<Card>> outsByType;
    
    int handTypes[] = {
        HAND_ROYAL_FLUSH,
        HAND_STRAIGHT_FLUSH,
        HAND_FOUR_OF_A_KIND,
        HAND_FULL_HOUSE,
        HAND_FLUSH,
        HAND_STRAIGHT,
        HAND_THREE_OF_A_KIND,
        HAND_TWO_PAIR,
        HAND_ONE_PAIR
    };
    
    for (int targetType : handTypes) {
        if (targetType <= currentHandType) {
            continue;
        }
        
        for (auto& testCard : availableCards) {
            if (improvesToHandType(hand, community, testCard, targetType)) {
                bool isPureOut = true;
                for (int strongerType : handTypes) {
                    if (strongerType > targetType && 
                        improvesToHandType(hand, community, testCard, strongerType)) {
                        isPureOut = false;
                        break;
                    }
                }
                
                if (isPureOut) {
                    bool alreadyExists = false;
                    for (auto& existingCard : outsByType[targetType]) {
                        if (existingCard == testCard) {
                            alreadyExists = true;
                            break;
                        }
                    }
                    if (!alreadyExists) {
                        outsByType[targetType].push_back(testCard);
                    }
                }
            }
        }
    }
    
    std::vector<Card> result;
    for (int i = 0; i < 9; ++i) {
        int targetType = handTypes[i];
        if (outsByType.find(targetType) != outsByType.end()) {
            for (auto& card : outsByType[targetType]) {
                result.push_back(card);
            }
        }
    }
    
    return result;
}

