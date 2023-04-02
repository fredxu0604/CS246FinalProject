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

    void declareBankruptcy() const; // player declares bankruptcy

    bool buyImprovement(std::string propertyName);  // returns true if purchase is successful,
                                                    //   false otherwise.
                                                    //   THROWS EXN if insufficient funds
    
    bool sellImprovement(std::string propertyName); // similar to above

    bool mortgage(std::string propertyName); // returns true if mortgage successful,
                                            //    false otherwise


    bool unmortgage(std::string propertyName); // similar to above except it THROWS EXN if insufficient funds

    void roll(std::vector<Square> allSquares); // Let's have the player roll and travel for themselves
                                              //    instead, and have the player report to Game
                                              //    with its new location

    bool makePayment(size_t amount); // Returns true if the player has enough money to make the payment
                                    //   The player can declare bankruptcy if this returns false
    
    void moveTo(const Square &newLocation); // moves the player to a new square

    

};

#endif
