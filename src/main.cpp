#include "../include/ui/TerminalView.h"
#include "../include/game/PokerGame.h"
#include "../include/core/GameSettings.h"
#include <memory>
#include <iostream>
#include <iomanip>

int main() {
    try {
        auto view = std::make_shared<TerminalView>();
        GameSettings gameSettings;
        
        bool running = true;
        while (running) {
            std::vector<std::string> mainMenu = {
                "Play",
                "Settings",
                "Rules",
                "Exit"
            };
            
            view->displayMenu(mainMenu);
            int choice = view->getMenuChoice(1, 4);
            
            switch (choice) {
                case 1: {
                    auto game = std::make_unique<PokerGame>(view, gameSettings);
                    game->run();
                    break;
                }
                case 2: {
                    bool settingsDone = false;
                    while (!settingsDone) {
                        view->displaySettings(gameSettings);
                        std::cout << std::left << std::setw(45) << "  1. Change difficulty (1-10)" 
                                  << "[Current: " << gameSettings.difficulty << "]\n";
                        std::cout << std::left << std::setw(45) << "  2. Change number of bots (1-5)" 
                                  << "[Current: " << gameSettings.numBots << "]\n";
                        std::cout << std::left << std::setw(45) << "  3. Toggle show opponent cards" 
                                  << "[Current: " << (gameSettings.showOpponentCards ? "Yes" : "No") << "]\n";
                        std::cout << std::left << std::setw(45) << "  4. Toggle calculate win chance" 
                                  << "[Current: " << (gameSettings.calculateWinChance ? "Yes" : "No") << "]\n";
                        std::cout << std::left << std::setw(45) << "  5. Toggle show outs" 
                                  << "[Current: " << (gameSettings.showOuts ? "Yes" : "No") << "]\n";
                        std::cout << std::left << std::setw(45) << "  6. Toggle show all hands at end" 
                                  << "[Current: " << (gameSettings.showAllHandsAtEnd ? "Yes" : "No") << "]\n";
                        std::cout << "  7. Back to main menu\n";
                        std::cout << "\n";
                        int choice = view->getMenuChoice(1, 7);
                        
                        switch (choice) {
                            case 1: {
                                int oldValue = gameSettings.difficulty;
                                int newValue = view->getIntInput("Enter difficulty (1-10): ", 1, 10);
                                if (newValue != oldValue) {
                                    gameSettings.difficulty = newValue;
                                    view->displayMessage("Difficulty changed from " + std::to_string(oldValue) + " to " + std::to_string(newValue) + ".", true);
                                    view->waitForInput();
                                }
                                break;
                            }
                            case 2: {
                                int oldValue = gameSettings.numBots;
                                int newValue = view->getIntInput("Enter number of bots (1-5): ", 1, 5);
                                if (newValue != oldValue) {
                                    gameSettings.numBots = newValue;
                                    view->displayMessage("Number of bots changed from " + std::to_string(oldValue) + " to " + std::to_string(newValue) + ".", true);
                                    view->waitForInput();
                                }
                                break;
                            }
                            case 3: {
                                bool oldValue = gameSettings.showOpponentCards;
                                gameSettings.showOpponentCards = !gameSettings.showOpponentCards;
                                view->displayMessage("Show opponent cards changed from " + std::string(oldValue ? "Yes" : "No") + " to " + std::string(gameSettings.showOpponentCards ? "Yes" : "No") + ".", true);
                                view->waitForInput();
                                break;
                            }
                            case 4: {
                                bool oldValue = gameSettings.calculateWinChance;
                                gameSettings.calculateWinChance = !gameSettings.calculateWinChance;
                                view->displayMessage("Calculate win chance changed from " + std::string(oldValue ? "Yes" : "No") + " to " + std::string(gameSettings.calculateWinChance ? "Yes" : "No") + ".", true);
                                view->waitForInput();
                                break;
                            }
                            case 5: {
                                bool oldValue = gameSettings.showOuts;
                                gameSettings.showOuts = !gameSettings.showOuts;
                                view->displayMessage("Show outs changed from " + std::string(oldValue ? "Yes" : "No") + " to " + std::string(gameSettings.showOuts ? "Yes" : "No") + ".", true);
                                view->waitForInput();
                                break;
                            }
                            case 6: {
                                bool oldValue = gameSettings.showAllHandsAtEnd;
                                gameSettings.showAllHandsAtEnd = !gameSettings.showAllHandsAtEnd;
                                view->displayMessage("Show all hands at end changed from " + std::string(oldValue ? "Yes" : "No") + " to " + std::string(gameSettings.showAllHandsAtEnd ? "Yes" : "No") + ".", true);
                                view->waitForInput();
                                break;
                            }
                            case 7:
                                settingsDone = true;
                                break;
                        }
                    }
                    break;
                }
                case 3: {
                    view->displayRules();
                    break;
                }
                case 4: {
                    running = false;
                    view->displayMessage("Thanks for playing!");
                    break;
                }
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}



