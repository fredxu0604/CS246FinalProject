#ifndef __SQUARE_H__
#define __SQUARE_H__

#include <string>
#include <vector>
#include "player.h"
#include "squareinfo.h"

using namespace std;

class Player;

class Square {
    protected:
    string name;
    vector<Player*> visitors;
    public:
    Square(string name);
    virtual ~Square();
    virtual SquareInfo getInfo() const;
    virtual bool isProperty() const = 0;
    virtual bool isOwned() const = 0;
    virtual Player* getOwner() = 0;
    void addVisitor(Player* player);
    void removeVisitor(Player* player);
    vector<Player*> getVisitors() const;
    
};

#endif

