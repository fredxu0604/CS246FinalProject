#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "gameexception.h"
#include "playerinfo.h"
#include "square.h"
#include "state.h"
#include "subject.h"
#include <random>
#include <vector>

class Player : public Subject<PlayerInfo, State> {
  std::string name;
  char avatar;
  size_t balance;
  State currState;
  Square *currSquare;
  std::vector<Square *> ownedProperties;

  bool hasMonopoly(std::string targetGroup, const int groupSize);
  Square *validateImprovement(std::string propertyName);
  Square *validateMortgage(std::string propertyName);

public:
  Player(std::string name, char avatar, Square *currSquare,
         std::vector<Square *> ownedProperties, size_t balance = 0);

  PlayerInfo getInfo() const override;

  void declareBankruptcy(); // player declares bankruptcy

  void buyImprovement(std::string propertyName); // throws exn if fail

  void sellImprovement(std::string propertyName); // similar to above

  void mortgage(std::string propertyName); // throws exn if fail

  void unmortgage(std::string propertyName); // similar to above

  bool makePayment(size_t amount); // Returns true if the player has enough
                                   // money to make the payment
                                   //   The player can declare bankruptcy if
                                   //   this returns false

  void moveTo(Square *newLocation); // moves the player to a new square
};

#endif
