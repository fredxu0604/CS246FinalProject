#ifndef __PROPERTY_H__
#define __PROPERTY_H__
#include "square.h"

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

#endif
