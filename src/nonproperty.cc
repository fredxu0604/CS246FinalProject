#include "nonproperty.h"
#include "gameexception.h"
#include "iostream"
#include "player.h"
#include <cstdlib>
#include <ctime>


class Player;

void GooseNesting::triggerEvent(Player *p) {
    cout<< "You are just attacked by geese!"<<endl;
}

void Tuition::triggerEvent(Player *p) {
    size_t input = 0;
    while (true) {
        std::cout << "Enter '1' to pay $"<< tuitionPayment<< " or '2' to pay 10%% of your total asset: ";
        std::cin >> input;
        if (input == 1) {
            if (p->makePayment(tuitionPayment)) {
                std::cout << "You have chosen to pay $"<< tuitionPayment<< "." << std::endl;
                break;
            } else {
                throw InsufficientFunds{"You have insufficient funds!"};
            }
        } else if (input == 2) {
            size_t totalAsset = p->getInfo().netWorth;
            p->makePayment(totalAsset);
            std::cout << "You have chosen to pay 10%% of your total assets." << std::endl;
            break;
        } else {
            std::cout << "Invalid input. Please enter '1' or '2'." << std::endl;
        }
    }
}

void CoopFee::triggerEvent(Player *p) {
    if (p->makePayment(coopFee)) {
        std::cout<<"You have paid " << coopFee << " as Coop Fee. Cali or Busted!"<< std::endl;
    } else {
        throw InsufficientFunds{"You have insufficient funds!"};
    }
}


void GoToTims::triggerEvent(Player *p) {

}

void NeedlesHall::triggerEvent(Player *p) {
    size_t payment;
    // more to come
}


