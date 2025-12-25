#ifndef TERMINALVIEW_H
#define TERMINALVIEW_H

#include "../interfaces/IGameView.h"
#include "../core/GameState.h"
#include "../core/Card.h"
#include "../core/GameSettings.h"
#include <string>
#include <vector>
#include <memory>

class TerminalView : public IGameView {
    static std::string RED;
    static std::string BLACK;
    static std::string RESET;
    static std::string BOLD;
    static std::string GREEN;
    static std::string YELLOW;
    static std::string BLUE;
    static std::string CYAN;

    void printCard(Card card);
    void printSuit(int suit);
    std::string rankToString(int rank);
    std::string suitToSymbol(int suit);

public:
    TerminalView();
    
    void displayMenu(std::vector<std::string> options) override;
    void displayGameState(GameState& state) override;
    void displayMessage(std::string message) override;
    void displayMessage(std::string message, bool highlight);
    void displayRules() override;
    
    void clearScreen() override;
    void waitForInput() override;
    
    int getMenuChoice(int min, int max) override;
    int getIntInput(std::string prompt, int min = 0, int max = 1000000) override;
    std::string getStringInput(std::string prompt) override;
    
    void displaySettings(GameSettings& settings) override;
    void displayPreGameScreen(int& startingBalance, int& smallBlind) override;
    void displayPlayerActionMenu(GameState& state, int playerBalance, int minBet) override;
};

#endif

