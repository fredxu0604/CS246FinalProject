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
    SquareInfo getName() const;
    virtual bool isProperty() const = 0;
    virtual bool isOwned() const = 0;
    virtual Player* getOwner() = 0;
    void addVisitor(Player* player);
    void removeVisitor(Player* player);
    vector<Player*> getVisitors() const;
};

class Property: public Square {
    protected:
    size_t cost;
    Player* owner;
    public:
    Property(string name, size_t cost);
    bool isProperty() const override;
    bool isOwned() const override;
    size_t getCost() const;
    Player* getOwner() override;
    void setOwner(Player* owner);
};

class NonProperty: public Square {
    protected:
    virtual void triggerEvent();
};

#endif

