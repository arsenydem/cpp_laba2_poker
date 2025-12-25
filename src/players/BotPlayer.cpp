#include "../../include/players/BotPlayer.h"
#include "../../include/core/Card.h"
#include "../../include/game/HandEvaluator.h"
#include <algorithm>
#include <random>
#include <chrono>

BotPlayer::BotPlayer(std::string name, int balance, int difficulty)
    : name(name), balance(balance), active(true), currentBet(0), 
      difficulty(difficulty), rng(std::chrono::steady_clock::now().time_since_epoch().count()) {
}

std::string BotPlayer::getName() {
    return this->name;
}

int BotPlayer::getBalance() {
    return this->balance;
}

void BotPlayer::addChips(int amount) {
    this->balance += amount;
}

bool BotPlayer::removeChips(int amount) {
    if (amount > this->balance) {
        return false;
    }
    this->balance -= amount;
    return true;
}

void BotPlayer::receiveCard(Card card) {
    this->hand.push_back(card);
}

std::vector<Card>& BotPlayer::getHand() {
    return this->hand;
}

void BotPlayer::clearHand() {
    this->hand.clear();
}

float BotPlayer::evaluateHandStrength(GameState& state) {
    if (this->hand.size() != 2) {
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(this->rng);
    }
    
    if (state.evaluator) {
        int handValue = state.evaluator->evaluateBestHand(this->hand, state.communityCards);
        
        float normalized = 0.0f;
        
        if (handValue >= HAND_ROYAL_FLUSH) {
            normalized = 0.95f + (handValue - HAND_ROYAL_FLUSH) / 1000000.0f * 0.05f;
        } else if (handValue >= HAND_STRAIGHT_FLUSH) {
            normalized = 0.85f + (handValue - HAND_STRAIGHT_FLUSH) / 1000000.0f * 0.10f;
        } else if (handValue >= HAND_FOUR_OF_A_KIND) {
            normalized = 0.75f + (handValue - HAND_FOUR_OF_A_KIND) / 1000000.0f * 0.10f;
        } else if (handValue >= HAND_FULL_HOUSE) {
            normalized = 0.65f + (handValue - HAND_FULL_HOUSE) / 1000000.0f * 0.10f;
        } else if (handValue >= HAND_FLUSH) {
            normalized = 0.55f + (handValue - HAND_FLUSH) / 1000000.0f * 0.10f;
        } else if (handValue >= HAND_STRAIGHT) {
            normalized = 0.45f + (handValue - HAND_STRAIGHT) / 1000000.0f * 0.10f;
        } else if (handValue >= HAND_THREE_OF_A_KIND) {
            normalized = 0.35f + (handValue - HAND_THREE_OF_A_KIND) / 1000000.0f * 0.10f;
        } else if (handValue >= HAND_TWO_PAIR) {
            normalized = 0.25f + (handValue - HAND_TWO_PAIR) / 1000000.0f * 0.10f;
        } else if (handValue >= HAND_ONE_PAIR) {
            normalized = 0.15f + (handValue - HAND_ONE_PAIR) / 1000000.0f * 0.10f;
        } else {
            normalized = 0.05f + (handValue - HAND_HIGH_CARD) / 1000000.0f * 0.10f;
        }
        
        float randomness = (10.0f - this->difficulty) / 10.0f * 0.1f;
        std::uniform_real_distribution<float> dist(-randomness, randomness);
        normalized += dist(this->rng);
        
        return std::max(0.0f, std::min(1.0f, normalized));
    }
    
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(this->rng);
}

float BotPlayer::calculatePotOdds(GameState& state) {
    if (state.pot == 0) return 0.0f;
    int callAmount = state.currentBet - this->currentBet;
    if (callAmount <= 0) return 1.0f;
    return static_cast<float>(state.pot) / (state.pot + callAmount);
}

float BotPlayer::getBluffProbability() {
    if (this->difficulty <= 3) return 0.1f;
    if (this->difficulty <= 6) return 0.2f;
    if (this->difficulty <= 8) return 0.3f;
    return 0.4f;
}

Action BotPlayer::makeSimpleDecision(GameState& state, float handStrength) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float random = dist(this->rng);
    
    Action action;
    if (random < 0.4f || handStrength < 0.3f) {
        action.type = ACTION_FOLD;
    } else if (random < 0.7f) {
        action.type = ACTION_CHECK;
    } else {
        action.type = ACTION_CALL;
    }
    return action;
}

Action BotPlayer::makeMediumDecision(GameState& state, float handStrength, float potOdds) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float random = dist(this->rng);
    
    Action action;
    if (handStrength < 0.4f && potOdds < 0.3f) {
        action.type = ACTION_FOLD;
    } else if (handStrength > 0.7f) {
        if (random < 0.3f) {
            action.type = ACTION_RAISE;
            int minRaise = state.currentBet + state.smallBlind;
            int raiseAmount = std::max(state.smallBlind, (int)(this->balance * 0.2f));
            action.amount = std::min(this->balance + this->currentBet, minRaise + raiseAmount);
        } else {
            action.type = ACTION_CALL;
        }
    } else {
        action.type = (potOdds > 0.5f) ? ACTION_CALL : ACTION_CHECK;
    }
    return action;
}

Action BotPlayer::makeAdvancedDecision(GameState& state, float handStrength, float potOdds, float bluffProb) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float random = dist(this->rng);
    
    Action action;
    
    if (handStrength > 0.8f) {
        if (random < 0.6f) {
            action.type = ACTION_RAISE;
            int minRaise = state.currentBet + state.smallBlind;
            int raiseAmount = std::max(state.smallBlind * 2, (int)(this->balance * 0.3f));
            action.amount = std::min(this->balance + this->currentBet, minRaise + raiseAmount);
        } else {
            action.type = ACTION_CALL;
        }
    }
    else if (random < bluffProb && handStrength > 0.4f) {
        action.type = ACTION_RAISE;
        int minRaise = state.currentBet + state.smallBlind;
        int raiseAmount = std::max(state.smallBlind, (int)(this->balance * 0.15f));
        action.amount = std::min(this->balance + this->currentBet, minRaise + raiseAmount);
    }
    else if (handStrength < 0.3f && potOdds < 0.25f) {
        action.type = ACTION_FOLD;
    }
    else if (potOdds > 0.4f) {
        action.type = ACTION_CALL;
    }
    else {
        action.type = ACTION_CHECK;
    }
    
    return action;
}

Action BotPlayer::makeDecision(GameState& state) {
    if (!this->active || this->balance == 0) {
        Action action;
        action.type = ACTION_FOLD;
        return action;
    }
    
    float handStrength = this->evaluateHandStrength(state);
    float potOdds = this->calculatePotOdds(state);
    float bluffProb = this->getBluffProbability();
    
    Action action;
    
    if (this->difficulty <= 3) {
        action = this->makeSimpleDecision(state, handStrength);
    } else if (this->difficulty <= 6) {
        action = this->makeMediumDecision(state, handStrength, potOdds);
    } else {
        action = this->makeAdvancedDecision(state, handStrength, potOdds, bluffProb);
    }
    
    int callAmount = state.currentBet - this->currentBet;
    
    if (action.type == ACTION_CALL) {
        if (callAmount >= this->balance) {
            action.type = ACTION_ALL_IN;
            action.amount = this->balance + this->currentBet;
        }
    } else if (action.type == ACTION_RAISE) {
        if (action.amount >= this->balance + this->currentBet) {
            action.type = ACTION_ALL_IN;
            action.amount = this->balance + this->currentBet;
        } else {
            int minRaise = state.currentBet + state.smallBlind;
            if (action.amount < minRaise) {
                action.amount = std::min(minRaise, this->balance + this->currentBet);
            }
        }
    }
    
    return action;
}

bool BotPlayer::isActive() {
    return this->active;
}

void BotPlayer::setActive(bool active) {
    this->active = active;
}

int BotPlayer::getCurrentBet() {
    return this->currentBet;
}

void BotPlayer::setCurrentBet(int bet) {
    this->currentBet = bet;
}

void BotPlayer::resetCurrentBet() {
    this->currentBet = 0;
}

