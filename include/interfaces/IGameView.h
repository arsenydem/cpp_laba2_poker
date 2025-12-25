#ifndef IGAMEVIEW_H
#define IGAMEVIEW_H

#include "../core/GameState.h"
#include <string>
#include <vector>
#include <memory>

class GameState;
struct GameSettings;

class IGameView {
public:
    virtual ~IGameView() = default;
    
    virtual void displayMenu(std::vector<std::string> options) = 0;
    virtual void displayGameState(GameState& state) = 0;
    virtual void displayMessage(std::string message) = 0;
    virtual void displayRules() = 0;
    
    virtual void clearScreen() = 0;
    virtual void waitForInput() = 0;
    
    virtual int getMenuChoice(int min, int max) = 0;
    virtual int getIntInput(std::string prompt, int min = 0, int max = 1000000) = 0;
    virtual std::string getStringInput(std::string prompt) = 0;
    
    virtual void displayPlayerActionMenu(GameState& state, int playerBalance, int minBet) = 0;
    virtual void displaySettings(GameSettings& settings) = 0;
    virtual void displayPreGameScreen(int& startingBalance, int& smallBlind) = 0;
};

#endif



