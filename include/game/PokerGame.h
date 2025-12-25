#ifndef POKERGAME_H
#define POKERGAME_H

#include "../core/GameSettings.h"
#include "../core/GameState.h"
#include "../core/Deck.h"
#include "../interfaces/IPlayer.h"
#include "../interfaces/IGameView.h"
#include "../interfaces/IHandEvaluator.h"
#include "../game/HandEvaluator.h"
#include <vector>
#include <memory>

class PokerGame {
    GameSettings settings;
    std::vector<std::shared_ptr<IPlayer>> players;
    std::shared_ptr<IGameView> view;
    std::shared_ptr<IHandEvaluator> evaluator;
    std::unique_ptr<Deck> deck;
    GameState state;
    
    bool gameRunning;
    
    void setupPlayers();
    void dealCards();
    void postBlinds();
    void bettingRound();
    void revealCommunityCards(int count);
    void showdown();
    void determineWinner();
    void resetRound();
    int getNextActivePlayer(int startIndex) ;
    bool allPlayersActed() ;
    void processAction(std::shared_ptr<IPlayer> player,  Action& action);
    void processAction(std::shared_ptr<IPlayer> player,  Action& action, bool verbose);
    void updateGameState();

public:
    PokerGame(std::shared_ptr<IGameView> view);
    PokerGame(std::shared_ptr<IGameView> view,  GameSettings& settings);
    
    void initialize();
    void startGame();
    void playRound();
    void run();
    
    GameSettings& getSettings() { return settings; }
 GameState& getState()  { return state; }
};

#endif



