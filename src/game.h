#ifndef __GAME_H__
#define __GAME_H__
#include "board.h"
#include "command.h"
#include "property.h"
#include "state.h"
#include <string>

class NonProperty;
class TimsCup;

class Game {
  int numPlayers;
  int numSquares;
  int numProperties;
  bool initialized;
  bool testing;
  Board *gameBoard;
  Player *currPlayer;
  GameState gameState;
  TimsCup *tc;

  std::vector<Square *> squares;
  std::vector<Property *> properties;
  std::vector<NonProperty *> nonProperties;
  std::vector<Player *> players;

  void roll(int die1 = 0, int die2 = 0); // implemented
  void next();                           // implemented

  void createSquares(); // implemented

  bool isIn(const std::vector<string> &pNames,
            const std::string &targetName); // implemented
  bool isIn(const map<std::string, PropertyType> &pNames,
            const std::string &targetName); // implemented

  int getSquareIdx(std::string squareName); // implemented
  int getSquareIdx(Square *sp);             // implemented

  Player *findPlayerByName(std::string name); // implemented

  int getPlayerIdx(Player *p); // implemented

  Property *findPropertyByName(std::string propertyName); // implemented
  NonProperty *
  findNonPropertyByName(std::string nonPropertyName); // implemented

  void handleArrival(); // implemented

  void trade(std::string name, std::string give,
             std::string receive);                                // implemented
  void trade(std::string name, size_t give, std::string receive); // implemented
  void trade(std::string name, std::string give, size_t receive); // implemented

  void tradeSwitch(std::string name, std::string give,
                   std::string receive); // implemented

  void tradeLoop(const std::vector<std::string> &args); // implemented

  void improve(std::string property, std::string mode); // implemented
  // void mortgage(std::string property);
  // void unmortgage(std::string property);
  void bankrupt(Player *owedTo);

  void displayAll();    // implemented
  void displayAssets(); // implemented

  void auctionLoop(Property *p); // implemented

  // void useCup();

  void cannotUseThisCommand();                                   // implemented
  void moneyCriticalLoop(size_t owes, Player *owedTo = nullptr); // implemented
  void stopGame();                                               // implemented

  void buyOrAuctionLoop(); // implemented

public:
  Game(bool testing = false); // implemented

  void loadFromFile(std::string fileName); // implemented
  void saveToFile(std::string fileName);   // implemented

  void newGame();     // implemented
  void runGameLoop(); // implemented
};

#endif
