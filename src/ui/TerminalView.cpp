#include "../../include/ui/TerminalView.h"
#include "../../include/core/GameSettings.h"
#include "../../include/interfaces/IPlayer.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

std::string TerminalView::RED = "\033[31m";
std::string TerminalView::BLACK = "\033[30m";
std::string TerminalView::RESET = "\033[0m";
std::string TerminalView::BOLD = "\033[1m";
std::string TerminalView::GREEN = "\033[32m";
std::string TerminalView::YELLOW = "\033[33m";
std::string TerminalView::BLUE = "\033[34m";
std::string TerminalView::CYAN = "\033[36m";

TerminalView::TerminalView() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
}

void TerminalView::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void TerminalView::waitForInput() {
    std::cout << "\nPress Enter to continue...";
    std::cin.clear();
    std::string dummy;
    std::getline(std::cin, dummy);
}

std::string TerminalView::rankToString(int rank)  {
    switch (rank) {
        case RANK_TWO: return "2";
        case RANK_THREE: return "3";
        case RANK_FOUR: return "4";
        case RANK_FIVE: return "5";
        case RANK_SIX: return "6";
        case RANK_SEVEN: return "7";
        case RANK_EIGHT: return "8";
        case RANK_NINE: return "9";
        case RANK_TEN: return "10";
        case RANK_JACK: return "J";
        case RANK_QUEEN: return "Q";
        case RANK_KING: return "K";
        case RANK_ACE: return "A";
        default: return "?";
    }
}

std::string TerminalView::suitToSymbol(int suit)  {
    switch (suit) {
        case SUIT_SPADES: return "♠";
        case SUIT_HEARTS: return "♥";
        case SUIT_DIAMONDS: return "♦";
        case SUIT_CLUBS: return "♣";
        default: return "?";
    }
}

void TerminalView::printSuit(int suit)  {
    if (suit == SUIT_HEARTS || suit == SUIT_DIAMONDS) {
        std::cout << RED;
    } else {
        std::cout << BLACK;
    }
    std::cout << suitToSymbol(suit) << RESET;
}

void TerminalView::printCard(Card card) {
    std::cout << "[";
    std::cout << rankToString(card.getRank());
    printSuit(card.getSuit());
    std::cout << "]";
}

void TerminalView::displayMenu(std::vector<std::string> options) {
    clearScreen();
    std::cout << BOLD << CYAN << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "                    TEXAS HOLD'EM POKER\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n" << RESET;
    std::cout << "\n";
    
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << options[i] << "\n";
    }
    std::cout << "\n";
}

void TerminalView::displayGameState(GameState& state) {
    clearScreen();
    
    std::cout << BOLD << CYAN << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "                    TEXAS HOLD'EM POKER\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n" << RESET;
    
    std::cout << "\n" << BOLD << std::left 
              << std::setw(20) << ("POT: $" + std::to_string(state.pot))
              << std::setw(25) << ("CURRENT BET: $" + std::to_string(state.currentBet))
              << std::setw(20) << ("SMALL BLIND: $" + std::to_string(state.smallBlind))
              << RESET << "\n\n";
    
    std::cout << "                    COMMUNITY CARDS\n";
    std::cout << "              ┌─────┬─────┬─────┬─────┬─────┐\n";
    std::cout << "              │ ";
    for (size_t i = 0; i < 5; ++i) {
        if (i < state.communityCards.size()) {
            printCard(state.communityCards[i]);
        } else {
            std::cout << " ? ";
        }
        if (i < 4) std::cout << " │ ";
    }
    std::cout << " │\n";
    std::cout << "              └─────┴─────┴─────┴─────┴─────┘\n\n";
    
    std::cout << "───────────────────────────────────────────────────────────────\n";
    std::cout << "PLAYERS:\n";
    for (size_t i = 0; i < state.players.size(); ++i) {
        auto player = state.players[i];
        std::string prefix = "";
        if (i == state.dealerPosition) prefix = "(D) ";
        else if (i == (state.dealerPosition + 1) % state.players.size()) prefix = "(SB) ";
        else if (i == (state.dealerPosition + 2) % state.players.size()) prefix = "(BB) ";
        
        std::string marker = (i == state.currentPlayerIndex) ? " >>> " : "     ";
        std::string activeMark = player->isActive() ? "[Active]" : "[Fold]";
        
        std::string normalizedPrefix = prefix;
        if (normalizedPrefix.length() < 4) {
            normalizedPrefix += std::string(4 - normalizedPrefix.length(), ' ');
        }
        
        std::string balanceStr = "$" + std::to_string(player->getBalance());
        std::string currentBetStr = "Current: $" + std::to_string(player->getCurrentBet());
        
        std::cout << marker 
                  << normalizedPrefix
                  << std::left << std::setw(16) << player->getName()
                  << std::right << std::setw(10) << balanceStr
                  << "  " << std::left << std::setw(10) << activeMark
                  << std::right << std::setw(15) << currentBetStr;
        
        if (i == state.currentPlayerIndex) std::cout << " <";
        std::cout << "\n";
    }
    std::cout << "───────────────────────────────────────────────────────────────\n";
    
    if (state.players.size() > 0 && state.players[0]->getHand().size() == 2) {
        std::cout << "\n                      YOUR HAND\n";
        auto hand = state.players[0]->getHand();
        std::cout << "              ┌─────┐ ┌─────┐\n";
        std::cout << "              │ ";
        printCard(hand[0]);
        std::cout << " │ │ ";
        printCard(hand[1]);
        std::cout << " │\n";
        std::cout << "              └─────┘ └─────┘\n";
    }
    
    if (state.winChance.has_value()) {
        std::cout << "\nWin Chance: " << std::fixed << std::setprecision(1) 
                  << state.winChance.value() * 100 << "%\n";
    }
    
    if (state.outs.has_value() && !state.outs->empty()) {
        std::cout << "Outs: ";
        for ( auto& card : state.outs.value()) {
            printCard(card);
            std::cout << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\n";
}

void TerminalView::displayMessage(std::string message) {
    std::cout << YELLOW << message << RESET << "\n";
}

void TerminalView::displayMessage(std::string message, bool highlight) {
    if (highlight) {
        std::cout << BOLD << GREEN << message << RESET << "\n";
    } else {
        displayMessage(message);
    }
}

void TerminalView::displayRules() {
    clearScreen();
    std::cout << BOLD << CYAN << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "                         RULES\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n" << RESET;
    
    std::cout << "\n" << BOLD << "POKER COMBINATIONS (from highest to lowest):\n" << RESET;
    std::cout << "1. Royal Flush - A, K, Q, J, 10 of the same suit\n";
    std::cout << "2. Straight Flush - Five consecutive cards of the same suit\n";
    std::cout << "3. Four of a Kind - Four cards of the same rank\n";
    std::cout << "4. Full House - Three of a kind + pair\n";
    std::cout << "5. Flush - Five cards of the same suit\n";
    std::cout << "6. Straight - Five consecutive cards\n";
    std::cout << "7. Three of a Kind - Three cards of the same rank\n";
    std::cout << "8. Two Pair - Two pairs of cards\n";
    std::cout << "9. One Pair - Two cards of the same rank\n";
    std::cout << "10. High Card - Highest card\n";
    
    std::cout << "\n" << BOLD << "GAME FLOW:\n" << RESET;
    std::cout << "1. Pre-flop - After receiving pocket cards\n";
    std::cout << "2. Flop - After revealing 3 community cards\n";
    std::cout << "3. Turn - After revealing 4th community card\n";
    std::cout << "4. River - After revealing 5th community card\n";
    std::cout << "5. Showdown - Revealing all cards\n";
    
    std::cout << "\n" << BOLD << "TERMINOLOGY:\n" << RESET;
    std::cout << "• Blind - Forced bet before cards are dealt\n";
    std::cout << "• Fold - Discard cards and forfeit the hand\n";
    std::cout << "• Check - Pass when no bet is required\n";
    std::cout << "• Bet - Make the first bet in a round\n";
    std::cout << "• Call - Match the current bet\n";
    std::cout << "• Raise - Increase the current bet\n";
    std::cout << "• All-in - Bet all remaining chips\n";
    
    waitForInput();
}

void TerminalView::displaySettings(GameSettings& settings) {
    clearScreen();
    std::cout << BOLD << CYAN << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "                        SETTINGS\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n" << RESET;
    std::cout << "\n";
}

void TerminalView::displayPreGameScreen(int& startingBalance, int& smallBlind) {
    clearScreen();
    std::cout << BOLD << CYAN << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "                    GAME SETUP\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n" << RESET;
    
    startingBalance = getIntInput("Enter starting balance (minimum 100): ", 100, 100000);
    int maxSmallBlind = startingBalance / 10;
    smallBlind = getIntInput("Enter small blind (1-" + std::to_string(maxSmallBlind) + "): ", 1, maxSmallBlind);
}

void TerminalView::displayPlayerActionMenu(GameState& state, int playerBalance, int minBet) {
    std::cout << "\n───────────────────────────────────────────────────────────────\n";
    std::cout << BOLD << "YOUR ACTIONS:\n" << RESET;
    
    int playerCurrentBet = 0;
    if (!state.players.empty()) {
        playerCurrentBet = state.players[0]->getCurrentBet();
    }
    
    int callAmount = state.currentBet - playerCurrentBet;
    bool canCheck = (state.currentBet == 0 || state.currentBet == playerCurrentBet);
    
    std::cout << "  1. Bet/Raise    (minimum: " << minBet << ")\n";
    if (callAmount > 0) {
        std::cout << "  2. Call         (" << callAmount << ")\n";
    } else {
        std::cout << "  2. Check\n";
    }
    std::cout << "  3. Fold\n";
    std::cout << "  4. All-in       (" << playerBalance << ")\n";
    std::cout << "\nEnter your choice: ";
}

int TerminalView::getMenuChoice(int min, int max) {
    int choice;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail() || choice < min || choice > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please enter a number between " << min << " and " << max << ": ";
        } else {
            std::cin.ignore();
            return choice;
        }
    }
}

int TerminalView::getIntInput(std::string prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < min || value > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
        } else {
            std::cin.ignore();
            return value;
        }
    }
}

std::string TerminalView::getStringInput(std::string prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

