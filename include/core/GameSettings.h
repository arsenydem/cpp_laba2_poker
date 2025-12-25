#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

struct GameSettings {
    int difficulty = 5;
    int numBots = 3;
    bool showOpponentCards = true;
    bool calculateWinChance = true;
    bool showOuts = true;
    bool showAllHandsAtEnd = true;
    
    int startingBalance = 1000;
    int smallBlind = 10;
};

#endif
