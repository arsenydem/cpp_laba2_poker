#include "../../include/game/PokerGame.h"
#include "../../include/players/HumanPlayer.h"
#include "../../include/players/BotPlayer.h"
#include <algorithm>
#include <random>
#include <chrono>

PokerGame::PokerGame(std::shared_ptr<IGameView> view)
    : view(view), evaluator(std::make_shared<HandEvaluator>()), 
      deck(std::make_unique<Deck>()), gameRunning(false) {
}

PokerGame::PokerGame(std::shared_ptr<IGameView> view,  GameSettings& settings)
    : view(view), evaluator(std::make_shared<HandEvaluator>()), 
      deck(std::make_unique<Deck>()), gameRunning(false), settings(settings) {
}

void PokerGame::initialize() {
    int startingBalance, smallBlind;
    this->view->displayPreGameScreen(startingBalance, smallBlind);
    this->settings.startingBalance = startingBalance;
    this->settings.smallBlind = smallBlind;
}

void PokerGame::setupPlayers() {
    this->players.clear();
    
    auto humanPlayer = std::make_shared<HumanPlayer>("Player", this->settings.startingBalance, this->view);
    this->players.push_back(humanPlayer);
    
    for (int i = 1; i <= this->settings.numBots; ++i) {
        std::string botName = "Bot" + std::to_string(i);
        auto bot = std::make_shared<BotPlayer>(botName, this->settings.startingBalance, this->settings.difficulty);
        this->players.push_back(bot);
    }
}

void PokerGame::dealCards() {
    this->deck->reset();
    this->deck->shuffle();
    
    for (auto& player : this->players) {
        player->clearHand();
    }
    
    for (int i = 0; i < 2; ++i) {
        for (auto& player : this->players) {
            if (player->getBalance() > 0) {
                player->receiveCard(this->deck->draw());
            }
        }
    }
}

void PokerGame::postBlinds() {
    for (auto& player : this->players) {
        player->resetCurrentBet();
        player->setActive(true);
    }
    
    this->state.currentBet = 0;
    this->state.pot = 0;
    
    int sbIndex = (this->state.dealerPosition + 1) % this->players.size();
    int sbAmount = std::min(this->settings.smallBlind, this->players[sbIndex]->getBalance());
    this->players[sbIndex]->removeChips(sbAmount);
    this->players[sbIndex]->setCurrentBet(sbAmount);
    this->state.pot += sbAmount;
    
    int bbIndex = (this->state.dealerPosition + 2) % this->players.size();
    int bbAmount = std::min(this->settings.smallBlind * 2, this->players[bbIndex]->getBalance());
    this->players[bbIndex]->removeChips(bbAmount);
    this->players[bbIndex]->setCurrentBet(bbAmount);
    this->state.currentBet = bbAmount;
    this->state.pot += bbAmount;
}

void PokerGame::bettingRound() {
    int startIndex = (this->state.dealerPosition + 3) % this->players.size();
    int currentIndex = startIndex;
    int lastRaiseIndex = -1;
    int raisesInRound = 0;
 int maxRaises = 3;
    
    while (true) {
        currentIndex = this->getNextActivePlayer(currentIndex);
        if (currentIndex == -1) break;
        
        auto player = this->players[currentIndex];
        
        if (player->getBalance() == 0 && player->getCurrentBet() == this->state.currentBet) {
            currentIndex = (currentIndex + 1) % this->players.size();
            if (currentIndex == startIndex) break;
            continue;
        }
        
        if (this->allPlayersActed() && currentIndex == startIndex && lastRaiseIndex == -1) {
            break;
        }
        
        if (player->getCurrentBet() == this->state.currentBet && 
            (lastRaiseIndex == -1 || currentIndex == lastRaiseIndex)) {
            if (this->allPlayersActed()) break;
        }
        
        this->state.currentPlayerIndex = currentIndex;
        this->updateGameState();
        this->view->displayGameState(this->state);
        
        Action action = player->makeDecision(this->state);
        this->processAction(player, action);
        
        if (action.type == ACTION_RAISE || action.type == ACTION_BET) {
            if (raisesInRound < maxRaises) {
                lastRaiseIndex = currentIndex;
                raisesInRound++;
                startIndex = currentIndex;
            }
        }
        
        currentIndex = (currentIndex + 1) % this->players.size();
        if (currentIndex == startIndex && lastRaiseIndex == -1) {
            if (this->allPlayersActed()) break;
        }
    }
    
    for (auto& player : this->players) {
        this->state.pot += player->getCurrentBet();
        player->resetCurrentBet();
    }
}

void PokerGame::revealCommunityCards(int count) {
    for (int i = 0; i < count; ++i) {
        if (!this->deck->isEmpty()) {
            this->state.communityCards.push_back(this->deck->draw());
        }
    }
}

void PokerGame::playRound() {
    this->state.communityCards.clear();
    this->state.dealerPosition = (this->state.dealerPosition + 1) % this->players.size();
    this->state.currentPlayerIndex = 0;
    
    this->dealCards();
    
    this->updateGameState();
    
    this->postBlinds();
    
    this->bettingRound();
    
    int activeCount = 0;
    for (auto& player : this->players) {
        if (player->isActive() && player->getBalance() > 0) activeCount++;
    }
    if (activeCount <= 1) {
        this->determineWinner();
        return;
    }
    
    this->revealCommunityCards(3);
    this->updateGameState();
    this->view->displayGameState(this->state);
    this->bettingRound();
    
    activeCount = 0;
    for (auto& player : this->players) {
        if (player->isActive() && player->getBalance() > 0) activeCount++;
    }
    if (activeCount <= 1) {
        this->determineWinner();
        return;
    }
    
    this->revealCommunityCards(1);
    this->updateGameState();
    this->view->displayGameState(this->state);
    this->bettingRound();
    
    activeCount = 0;
    for (auto& player : this->players) {
        if (player->isActive() && player->getBalance() > 0) activeCount++;
    }
    if (activeCount <= 1) {
        this->determineWinner();
        return;
    }
    
    this->revealCommunityCards(1);
    this->updateGameState();
    this->view->displayGameState(this->state);
    this->bettingRound();
    
    this->showdown();
}

void PokerGame::showdown() {
    this->view->displayMessage("Showdown! Revealing all cards...");
    this->view->waitForInput();
    
    this->determineWinner();
}

void PokerGame::determineWinner() {
    std::vector<std::shared_ptr<IPlayer>> activePlayers;
    for (auto& player : this->players) {
        if (player->isActive() && player->getBalance() >= 0) {
            activePlayers.push_back(player);
        }
    }
    
    if (activePlayers.empty()) {
        this->view->displayMessage("No active players!");
        return;
    }
    
    if (activePlayers.size() == 1) {
        activePlayers[0]->addChips(this->state.pot);
        this->view->displayMessage(activePlayers[0]->getName() + " wins $" + std::to_string(this->state.pot) + "!");
    } else {
        int bestHandValue = -1;
        std::vector<size_t> winners;
        
        for (size_t i = 0; i < activePlayers.size(); ++i) {
            auto hand = activePlayers[i]->getHand();
            if (hand.size() == 2) {
                int handValue = this->evaluator->evaluateBestHand(hand, this->state.communityCards);
                
                if (handValue > bestHandValue) {
                    bestHandValue = handValue;
                    winners.clear();
                    winners.push_back(i);
                } else if (handValue == bestHandValue) {
                    winners.push_back(i);
                }
            }
        }
        
        if (winners.empty()) {
            std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
            std::uniform_int_distribution<size_t> dist(0, activePlayers.size() - 1);
            size_t winnerIndex = dist(rng);
            activePlayers[winnerIndex]->addChips(this->state.pot);
            this->view->displayMessage(activePlayers[winnerIndex]->getName() + " wins $" + std::to_string(this->state.pot) + "!");
        } else {
            int potPerWinner = this->state.pot / winners.size();
            int remainder = this->state.pot % winners.size();
            
            for (size_t i = 0; i < winners.size(); ++i) {
                int winnings = potPerWinner + (i < remainder ? 1 : 0);
                activePlayers[winners[i]]->addChips(winnings);
            }
            
            if (winners.size() == 1) {
                this->view->displayMessage(activePlayers[winners[0]]->getName() + " wins $" + std::to_string(this->state.pot) + "!");
            } else {
                std::string winnerNames;
                for (size_t i = 0; i < winners.size(); ++i) {
                    if (i > 0) winnerNames += (i == winners.size() - 1 ? " and " : ", ");
                    winnerNames += activePlayers[winners[i]]->getName();
                }
                this->view->displayMessage(winnerNames + " tie and split $" + std::to_string(this->state.pot) + "!");
            }
        }
    }
    
    this->state.pot = 0;
    this->view->waitForInput();
}

void PokerGame::resetRound() {
    this->state.communityCards.clear();
    this->state.currentBet = 0;
    this->state.pot = 0;
    this->state.currentPlayerIndex = 0;
    this->state.winChance = std::nullopt;
    this->state.outs = std::nullopt;
    
    for (auto& player : this->players) {
        player->clearHand();
        player->resetCurrentBet();
        player->setActive(true);
    }
    
    this->updateGameState();
}

int PokerGame::getNextActivePlayer(int startIndex)  {
    for (int i = 0; i < this->players.size(); ++i) {
        int index = (startIndex + i) % this->players.size();
        if (this->players[index]->isActive() && this->players[index]->getBalance() > 0) {
            return index;
        }
    }
    return -1;
}

bool PokerGame::allPlayersActed()  {
    for ( auto& player : this->players) {
        if (player->isActive() && player->getBalance() > 0) {
            if (player->getCurrentBet() != this->state.currentBet) {
                return false;
            }
        }
    }
    return true;
}

void PokerGame::processAction(std::shared_ptr<IPlayer> player,  Action& action) {
    this->processAction(player, action, true);
}

void PokerGame::processAction(std::shared_ptr<IPlayer> player,  Action& action, bool verbose) {
    switch (action.type) {
        case ACTION_FOLD:
            player->setActive(false);
            if (verbose) {
                this->view->displayMessage(player->getName() + " folds.");
            }
            break;
            
        case ACTION_CHECK:
            if (verbose) {
                this->view->displayMessage(player->getName() + " checks.");
            }
            break;
            
        case ACTION_CALL: {
            int callAmount = this->state.currentBet - player->getCurrentBet();
            if (callAmount > 0) {
                int actualAmount = std::min(callAmount, player->getBalance());
                player->removeChips(actualAmount);
                player->setCurrentBet(player->getCurrentBet() + actualAmount);
                this->state.pot += actualAmount;
                if (verbose) {
                    this->view->displayMessage(player->getName() + " calls $" + std::to_string(actualAmount) + ".");
                }
            }
            break;
        }
        
        case ACTION_BET:
        case ACTION_RAISE: {
            int totalBet = std::min(action.amount, player->getBalance() + player->getCurrentBet());
            int currentPlayerBet = player->getCurrentBet();
            int additionalChips = totalBet - currentPlayerBet;
            
            if (additionalChips > 0) {
                player->removeChips(additionalChips);
                player->setCurrentBet(totalBet);
                this->state.currentBet = totalBet;
                this->state.pot += additionalChips;
                
                if (verbose) {
                    std::string actionStr = (action.type == ACTION_BET) ? "bets" : "raises";
                    this->view->displayMessage(player->getName() + " " + actionStr + " to $" + std::to_string(totalBet) + ".");
                }
            }
            break;
        }
        
        case ACTION_ALL_IN: {
            int allInAmount = std::min(action.amount, player->getBalance());
            player->removeChips(allInAmount);
            player->setCurrentBet(player->getCurrentBet() + allInAmount);
            if (player->getCurrentBet() > this->state.currentBet) {
                this->state.currentBet = player->getCurrentBet();
            }
            this->state.pot += allInAmount;
            if (verbose) {
                this->view->displayMessage(player->getName() + " goes all-in with $" + std::to_string(allInAmount) + "!");
            }
            break;
        }
    }
}

void PokerGame::updateGameState() {
    this->state.players = this->players;
    this->state.evaluator = this->evaluator;
    
    if (this->players.size() > 0) {
        auto playerHand = this->players[0]->getHand();
        
        if (this->settings.calculateWinChance && playerHand.size() == 2) {
            int numOpponents = 0;
            for (size_t i = 1; i < this->players.size(); ++i) {
                if (this->players[i]->isActive() && this->players[i]->getBalance() > 0) {
                    numOpponents++;
                }
            }
            
            this->state.winChance = this->evaluator->calculateWinProbability(
                playerHand,
                this->state.communityCards,
                numOpponents
            );
        } else {
            this->state.winChance = std::nullopt;
        }
        
        if (this->settings.showOuts && playerHand.size() == 2 && !this->state.communityCards.empty()) {
            std::vector<std::vector<Card>> opponentHands;
            for (size_t i = 1; i < this->players.size(); ++i) {
                if (this->players[i]->isActive()) {
                    opponentHands.push_back(this->players[i]->getHand());
                }
            }
            
            this->state.outs = this->evaluator->findOuts(
                playerHand,
                this->state.communityCards,
                opponentHands
            );
        } else {
            this->state.outs = std::nullopt;
        }
    }
}

void PokerGame::startGame() {
    this->setupPlayers();
    this->gameRunning = true;
}

void PokerGame::run() {
    this->initialize();
    this->startGame();
    
    while (this->gameRunning) {
        bool playerHasMoney = false;
        for ( auto& player : this->players) {
            if (player->getBalance() > 0) {
                playerHasMoney = true;
                break;
            }
        }
        
        if (!playerHasMoney) {
            this->view->displayMessage("Game over! No players have money left.");
            break;
        }
        
        this->playRound();
        this->resetRound();
        
        if (this->players.size() > 0 && this->players[0]->getBalance() <= 0) {
            this->view->displayMessage("Кончились деньги, начните новую игру");
            this->view->waitForInput();
            this->gameRunning = false;
            break;
        }
        
        std::vector<std::string> options = {"Continue", "Exit"};
        this->view->displayMenu(options);
        int choice = this->view->getMenuChoice(1, 2);
        if (choice == 2) {
            this->gameRunning = false;
        } else {
            if (this->players.size() > 0 && this->players[0]->getBalance() <= 0) {
                this->view->displayMessage("Кончились деньги, начните новую игру");
                this->view->waitForInput();
                this->gameRunning = false;
                break;
            }
            
            this->view->clearScreen();
            this->updateGameState();
        }
    }
}

