#include "../../include/players/HumanPlayer.h"
#include <iostream>

HumanPlayer::HumanPlayer(std::string name, int balance, std::shared_ptr<IGameView> view)
    : name(name), balance(balance), active(true), currentBet(0), view(view) {
}

HumanPlayer::HumanPlayer(std::string name, int balance)
    : name(name), balance(balance), active(true), currentBet(0), view(nullptr) {
}

std::string HumanPlayer::getName() {
    return this->name;
}

int HumanPlayer::getBalance() {
    return this->balance;
}

void HumanPlayer::addChips(int amount) {
    this->balance += amount;
}

bool HumanPlayer::removeChips(int amount) {
    if (amount > this->balance) {
        return false;
    }
    this->balance -= amount;
    return true;
}

void HumanPlayer::receiveCard(Card card) {
    this->hand.push_back(card);
}

std::vector<Card>& HumanPlayer::getHand() {
    return this->hand;
}

void HumanPlayer::clearHand() {
    this->hand.clear();
}

Action HumanPlayer::makeDecision(GameState& state) {
    int minRaise = state.currentBet + state.smallBlind;
    int totalBetNeeded = minRaise;
    this->view->displayPlayerActionMenu(state, this->balance, totalBetNeeded);
    
    int choice = this->view->getMenuChoice(1, 5);
    Action action;
    
    switch (choice) {
        case 1: {
            int minBet = state.currentBet + state.smallBlind;
            int maxBet = this->balance;
            int betAmount = this->view->getIntInput(
                "Enter total bet amount: ", 
                minBet, 
                maxBet
            );
            action.type = (state.currentBet == 0) ? ACTION_BET : ACTION_RAISE;
            action.amount = betAmount;
            break;
        }
        case 2: {
            int callAmount = state.currentBet - this->currentBet;
            if (callAmount >= this->balance) {
                action.type = ACTION_ALL_IN;
                action.amount = this->balance;
            } else {
                action.type = ACTION_CALL;
                action.amount = callAmount;
            }
            break;
        }
        case 3: {
            if (state.currentBet == 0 || state.currentBet == this->currentBet) {
                action.type = ACTION_CHECK;
            } else {
                action.type = ACTION_FOLD;
            }
            break;
        }
        case 4: {
            action.type = ACTION_FOLD;
            break;
        }
        case 5: {
            action.type = ACTION_ALL_IN;
            action.amount = this->balance;
            break;
        }
        default:
            action.type = ACTION_FOLD;
            break;
    }
    
    return action;
}

bool HumanPlayer::isActive() {
    return this->active;
}

void HumanPlayer::setActive(bool active) {
    this->active = active;
}

int HumanPlayer::getCurrentBet() {
    return this->currentBet;
}

void HumanPlayer::setCurrentBet(int bet) {
    this->currentBet = bet;
}

void HumanPlayer::resetCurrentBet() {
    this->currentBet = 0;
}

