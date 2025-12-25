#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Card.h"
#include "../interfaces/IHandEvaluator.h"
#include <vector>
#include <memory>
#include <optional>

class IPlayer;

enum {
    ACTION_FOLD = 0,
    ACTION_CHECK = 1,
    ACTION_CALL = 2,
    ACTION_BET = 3,
    ACTION_RAISE = 4,
    ACTION_ALL_IN = 5
};

struct GameState {
    int pot = 0;
    int currentBet = 0;
    int smallBlind = 10;
    std::vector<Card> communityCards;
    std::vector<std::shared_ptr<IPlayer>> players;
    int dealerPosition = 0;
    int currentPlayerIndex = 0;
    
    std::optional<float> winChance;
    std::optional<std::vector<Card>> outs;
    
    std::shared_ptr<IHandEvaluator> evaluator = nullptr;
};

struct Action {
    int type;
    int amount = 0;
};

#endif

