#ifndef __GAME_H__
#define __GAME_H__
#include "board.h"
#include "command.h"
#include "state.h"
#include <string>

class Game {
    bool initialized;
    bool gameOn;
    bool testing;
    Board *gameBoard;
    Player *currPlayer;
    // GameState gameState; idk if i actually need this

    
    void saveToFile(std::string fileName);
    void roll();
    void next();

    void handleArrival();

    void trade(std::string name, std::string give, std::string receive);
    void trade(std::string name, size_t give, std::string receive);
    void trade(std::string name, std::string give, size_t receive);

    void improve(std::string property, std::string mode);
    void mortgage(std::string property);
    void unmortgage(std::string property);
    void bankrupt();

    void auction();
    void auctionBid();
    void auctionWithdraw();

    void useCup();

    public:
    Game(bool testing = false);
    void loadFromFile(std::string fileName);
    void newGame();
    bool execute(Command cmd);
    void runGameLoop();

};



#endif
