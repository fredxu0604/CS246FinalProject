#ifndef __PLAYERINFO_H__
#define __PLAYERINFO_H__
#include <string>
#include <vector>

class Square;
class Property;

struct PlayerInfo {
  const std::string &name;
  char avatar;
  size_t balance;
  size_t netWorth;
  const std::vector<Property *> &ownedProperties;
  Square *currSquare;
  bool isBankrupt;
};

#endif
