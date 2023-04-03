#ifndef __PLAYERINFO_H__
#define __PLAYERINFO_H__
#include "square.h"
#include <string>
#include <vector>
#include "property.h"

struct PlayerInfo {
  std::string name;
  char avatar;
  size_t balance;
  std::vector<Property *> ownedProperties;
  Square &currSquare;
};

#endif
