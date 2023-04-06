#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "playerinfo.h"
#include "state.h"
#include "subject.h"
#include <vector>

class Square;
class Property;

class Player : public Subject<PlayerInfo, State> {
  std::string name;
  char avatar;
  size_t balance;
  State currState;
  Square *currSquare;
  std::vector<Property *> ownedProperties;

  bool owns(const std::string &propertyName);
  bool hasMonopoly(const std::string &propertyName);
  Property *validateImprovement(const std::string &propertyName);
  Property *validateMortgage(const std::string &propertyName);

public:
  Player(const std::string &name, char avatar, Square *currSquare,
         std::vector<Property *> ownedProperties, size_t balance = 0);

  PlayerInfo getInfo() const override;

  void declareBankruptcy(); // player declares bankruptcy

  void buyImprovement(const std::string &propertyName); // throws exn if fail

  void sellImprovement(const std::string &propertyName); // similar to above

  void mortgage(const std::string &propertyName); // throws exn if fail

  void unmortgage(const std::string &propertyName); // similar to above

  bool makePayment(size_t amount,
                   bool notify = false); // Returns true if the player has
                                         // enough money to make the payment
                                         //   The player can declare bankruptcy
                                         //   if this returns false

  void moveTo(Square *newLocation); // moves the player to a new square

  void addFunds(size_t amount);

  void addProperty(Property *property);

  void removeProperty(Property *property);

  void buyProperty(Property *property);
};

#endif
