#ifndef __SQUARE_H__
#define __SQUARE_H__
#include <string>
#include <vector>
#include "player.h"
using namespace std;

class Square {
    protected:
    string name;
    vector<Player*> visitors;
    public:
    Square(string name);
    string getName() const;
    virtual ~Square() = 0;
    virtual bool isProperty() = 0;
    virtual bool isOwned() = 0;
    virtual Player* getOwner() = 0;
    void addVisitor(Player* player);
    void removeVisitor(Player* player);
    vector<Player*> getVisitors() const;
};
#endif
