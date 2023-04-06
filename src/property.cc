#include "property.h"
#include "squareinfo.h"
#include "gameexception.h"
#include <string>
#include <map>
using namespace std;

Property::Property(string name): Square(name) {
    owner = nullptr;
    group = groupMap.at(name);   //using at because map[] only support non constant value; here is constant map
}


void Property::setOwner(Player* player) {
    owner = player;
}


