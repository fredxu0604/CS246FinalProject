#ifndef __PLAYERINFO_H__
#define __PLAYERINFO_H__
#include <string>
#include <vector>

class Square;
class Property;

struct PlayerInfo {
  std::string name;
  char avatar;
  size_t balance;
  std::vector<Property *> ownedProperties;
  Square *currSquare;
};

#endif
