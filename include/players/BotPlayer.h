#ifndef BOTPLAYER_H
#define BOTPLAYER_H

#include "../interfaces/IPlayer.h"
#include "../core/Card.h"
#include "../core/GameState.h"
#include <string>
#include <vector>
#include <memory>
#include <random>

class BotPlayer : public IPlayer {
    std::string name;
    int balance;
    std::vector<Card> hand;
    bool active;
    int currentBet;
    int difficulty;
    std::mt19937 rng;

    float evaluateHandStrength(GameState& state);
    float calculatePotOdds(GameState& state);
    float getBluffProbability();
    Action makeSimpleDecision(GameState& state, float handStrength);
    Action makeMediumDecision(GameState& state, float handStrength, float potOdds);
    Action makeAdvancedDecision(GameState& state, float handStrength, float potOdds, float bluffProb);

public:
    BotPlayer(std::string name, int balance, int difficulty);
    
    std::string getName() override;
    int getBalance() override;
    void addChips(int amount) override;
    bool removeChips(int amount) override;
    
    void receiveCard(Card card) override;
    std::vector<Card>& getHand() override;
    void clearHand() override;
    
    Action makeDecision(GameState& state) override;
    bool isActive() override;
    void setActive(bool active) override;
    
    int getCurrentBet() override;
    void setCurrentBet(int bet) override;
    void resetCurrentBet() override;
};

#endif

