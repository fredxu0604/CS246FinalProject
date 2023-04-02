#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "subject.h"
#include "state.h"
#include "playerinfo.h"
#include "square.h"
#include <vector>

class Player: public Subject<PlayerInfo, State> {
    std::string name;
    char avatar;
    size_t balance;
    State currState;
    Square &currSquare;
    std::vector<Square> ownedProperties;

    public:
    Player(std::string name, char avatar, Square &currSquare, 
           std::vector<Square> ownedProperties, size_t balance = 0);

    PlayerInfo getInfo() const override;

    void declareBankruptcy() const;

    bool buyImprovement(std::string propertyName);
    
    bool sellImprovement(std::string propertyName);

    bool mortgage(std::string propertyName);

    bool unmortgage(std::string propertyName);

    

};

#endif
