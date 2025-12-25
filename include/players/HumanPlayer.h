#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "../interfaces/IPlayer.h"
#include "../interfaces/IGameView.h"
#include "../core/Card.h"
#include "../core/GameState.h"
#include <string>
#include <vector>
#include <memory>
 
class HumanPlayer : public IPlayer {
    std::string name;
    int balance;
    std::vector<Card> hand;
    bool active;
    int currentBet;
    std::shared_ptr<IGameView> view;

public:
    HumanPlayer(std::string name, int balance, std::shared_ptr<IGameView> view);
    HumanPlayer(std::string name, int balance);
    
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



