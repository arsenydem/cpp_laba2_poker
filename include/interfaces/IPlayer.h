#ifndef IPLAYER_H
#define IPLAYER_H

#include "../core/Card.h"
#include "../core/GameState.h"
#include <string>
#include <vector>
#include <memory>

class GameState;

class IPlayer {
public:
    virtual ~IPlayer() = default;
    
    virtual std::string getName() = 0;
    virtual int getBalance() = 0;
    virtual void addChips(int amount) = 0;
    virtual bool removeChips(int amount) = 0;
    
    virtual void receiveCard(Card card) = 0;
    virtual std::vector<Card>& getHand() = 0;
    virtual void clearHand() = 0;
    
    virtual Action makeDecision(GameState& state) = 0;
    virtual bool isActive() = 0;
    virtual void setActive(bool active) = 0;
    
    virtual int getCurrentBet() = 0;
    virtual void setCurrentBet(int bet) = 0;
    virtual void resetCurrentBet() = 0;
};

#endif



