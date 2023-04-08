#include "nonproperty.h"
#include "property.h"
#include "gameexception.h"
#include "iostream"
#include "player.h"
#include <cstdlib>
#include <ctime>


class Player;

// helper
// find the index of square on the board
int findIndexByName(std::vector<Square *> &squares, std::string name) {
    int index = -1;
    for (int i = 0; i < squares.size(); i++) {
        if (squares[i]->getInfo().name == name) {
            index = i;
        }
    }
    return index;
}

NonProperty::NonProperty(string name): Square{name} {}

SquareInfo NonProperty::getInfo() const {
    return SquareInfo{
        name,
        false,
        PropertyType::Gym, // doesn't matter
        false,
        false,
        0,
        nullptr,
        0,
        0,
        0,
        0,
        0
    };
}

CollectOSAP::CollectOSAP(string name): NonProperty{name} {}

CollectOSAP::~CollectOSAP() {}

void CollectOSAP::triggerEvent(Player *p) {
    p->addFunds(OSAPFee);
}

DCTimsLine::DCTimsLine(string name): NonProperty{name} {}

DCTimsLine::~DCTimsLine() {}

void DCTimsLine::triggerEvent(Player *p) {
    // need to implement stuck
}

GoToTims::GoToTims(string name, std::vector<Square *> &squares): NonProperty{name}, squares{squares}{}

GoToTims::~GoToTims() {
    for (auto s:squares) {
        delete s;
    }
    squares.clear();
}

void GoToTims::triggerEvent(Player *p) {
    int timsIndex = findIndexByName(squares, "DCTimsLine");
    p->moveTo(squares[timsIndex]);
}

GooseNesting::GooseNesting(string name): NonProperty{name} {}

GooseNesting::~GooseNesting() {}

void GooseNesting::triggerEvent(Player *p) {
    cout<< "You are attacked by geese!"<<endl;
}

Tuition::Tuition(string name): NonProperty{name} {}

Tuition::~Tuition() {}

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

CoopFee::CoopFee(string name): NonProperty{name} {}

CoopFee::~CoopFee() {}

void CoopFee::triggerEvent(Player *p) {
    if (p->makePayment(coopFee)) {
        std::cout<<"You have paid " << coopFee << " as Coop Fee. Cali or Busted!"<< std::endl;
    } else {
        throw InsufficientFunds{"You have insufficient funds!"};
    }
}


SLC::SLC(string name, std::vector<Square *> &squares, TimsCup * timsCups): NonProperty{name}, squares{squares},
timsCups{timsCups} {}

SLC::~SLC() {
    delete timsCups;
    for (auto s: squares) {
        delete s;
    }
    squares.clear();
}

void SLC::triggerEvent(Player *p) {
    if (timsCups->getAvailable() > 0) {
        // Seed the random number generator with the current time
        srand(time(0));
        // Generate a random number between 1 and 100
        int rand_num = rand() % 100 + 1;
        if (rand_num == 1) {
            p->addTimsCup();
            timsCups->allocateOne();
        }
    }
    // find the player's location in the board
    int index = p->findIndex(squares);
    int move;
    // find GotoTims and collectOSAP index
    int timsIndex = findIndexByName(squares, "GoToTims");
    int OSAPIndex = findIndexByName(squares, "collectOSAP");

    // Seed the random number generator with the current time
    srand(time(0));
    // Generate a random number between 1 and 24
    int rand_num = rand() % 24 + 1;
    // Determine which thing will happen based on the value generated.
    if (rand_num <= 3) {
        move = -3;
    } else if (rand_num <= 7) {
        move = -2;
    } else if (rand_num <= 11) {
        move = -1;
    } else if (rand_num <= 14) {
        move = 1;
    } else if (rand_num <= 18) {
        move = 2;
    } else if (rand_num <= 22) {
        move = 3;
    } else if (rand_num <= 23) {
        p->moveTo(squares[timsIndex]);
        std::cout<<"You just moved to DC Tims Line!"<< std::endl;
        return;
    } else {
        p->moveTo(squares[OSAPIndex]);
        std::cout<<"You just moved to Collect OSAP!"<< std::endl;
        return;
    }
    // send the player to the new location
    int newIndex = (index + move + squares.size()) % squares.size();
    p->moveTo(squares[newIndex]);
    std::cout<<"You just moved "<<newIndex<<" and landed on "<<squares[newIndex]->getInfo().name<< "!"<<std::endl;
    return;
}


NeedlesHall::NeedlesHall(string name, TimsCup* timsCup): NonProperty{name}, timsCups{timsCups} {}

NeedlesHall::~NeedlesHall() {
    delete timsCups;
}

void NeedlesHall::triggerEvent(Player *p) {
    if (timsCups->getAvailable() > 0) {
        // Seed the random number generator with the current time
        srand(time(0));
        // Generate a random number between 1 and 100
        int rand_num = rand() % 100 + 1;
        if (rand_num == 1) {
            p->addTimsCup();
            timsCups->allocateOne();
        }
    }
    size_t payment = 0;
    size_t gain = 0;
    // Seed the random number generator with the current time
    srand(time(0));

    // Generate a random number between 1 and 18
    int rand_num = rand() % 18 + 1;

    // Determine which value to use based on the random number
    if (rand_num <= 1) {
        payment = 200;
    } else if (rand_num <= 3) {
        payment = 100;
    } else if (rand_num <= 6) {
        payment = 50;
    } else if (rand_num <= 12) {
        gain = 25;
    } else if (rand_num <= 15) {
        gain = 50;
    } else if (rand_num <= 17) {
        gain = 100;
    } else {
        gain = 200;
    }
    if (payment > 0) {
        if (p->makePayment(payment)) {
            std::cout<<"You have paid " << payment << "." << std::endl;
        } else {
            throw InsufficientFunds{"You do not have enough fund"};
        }
    } else {
        p->addFunds(gain);
    }
}
