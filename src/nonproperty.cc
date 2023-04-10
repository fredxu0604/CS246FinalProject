#include "nonproperty.h"
#include "gameexception.h"
#include "iostream"
#include "player.h"
#include "property.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
using namespace std;

class Player;

// helper
// find the index of square on the board
int findIndexByName(vector<Square *> &squares, string name) {
  int index = -1;
  for (int i = 0; i < squares.size(); i++) {
    if (squares[i]->getInfo().name == name) {
      index = i;
    }
  }
  return index;
}

// helper
// ask the player to use roll up rim cup or balance to escape from DC Tims Line.

void payBalanceOrCup(Player *p) {
  int input = 0;
  // if player doesn't have enough fund but he has tims cup
  if (p->getInfo().balance < DCTimsLineFee && p->getInfo().timsCups > 0) {
    p->useTimsCup(); // use the tims cup
    std::cout << "You have chosen to use the Roll Up the Rim Cup. And you will "
                 "get out of here!"
              << endl;
    return;
  } else if (p->getInfo().balance > DCTimsLineFee &&
             p->getInfo().timsCups < 0) {
    p->makePayment(DCTimsLineFee);
    std::cout << "You have chosen to pay $" << DCTimsLineFee
              << ". And you will get out of here!" << endl;
    return;
  } else { // player's choice now
    while (true) {
      std::cout << "Enter '1' to pay $" << DCTimsLineFee
                << " or '2' to use Roll Up the Rim Cup";
      string inStr;
      getline(cin, inStr);
      istringstream iss{inStr};
      iss >> input;
      if (input == 1) {
        p->makePayment(DCTimsLineFee);
        p->makeUnstuck();
        std::cout << "You have chosen to pay $" << DCTimsLineFee
                  << ". And you will get out of here!" << endl;
        break;
      } else if (input == 2) {
        p->useTimsCup();
        p->makeUnstuck();
        std::cout << "You have chosen to use the Roll Up the Rim Cup. And you "
                     "will get out of here!"
                  << endl;
        break;
      } else {
        std::cout << "Invalid input. Please enter '1' or '2'." << endl;
      }
    }
  }
}

NonProperty::NonProperty(string name) : Square{name} {}

NonProperty::~NonProperty() {}

SquareInfo NonProperty::getInfo() const {
  return SquareInfo{name,
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
                    0};
}

CollectOSAP::CollectOSAP(string name) : NonProperty{name} {}

CollectOSAP::~CollectOSAP() {}

void CollectOSAP::triggerEvent(Player *p, std::vector<Square *> &squares,
                               TimsCup *timsCups) {
  p->addFunds(OSAPFee);
}

DCTimsLine::DCTimsLine(string name) : NonProperty{name} {}

DCTimsLine::~DCTimsLine() {}

void DCTimsLine::triggerEvent(Player *p, std::vector<Square *> &squares,
                              TimsCup *timsCups) {
  // roll the dice twice
  int index = p->findIndex(squares);
  int move1 = p->roll();
  int move2 = p->roll();
  int newIndex = (index + move1 + move2 + squares.size()) % squares.size();
  // if player just land on this
  if (p->getInfo().turnsStuck == 0) {
    p->moveTo(squares[newIndex]);
    return;
  }
  // if it's a double roll
  if (move1 == move2) { // double roll, player got released
    p->moveTo(squares[newIndex]);
    p->makeUnstuck();
    return;
  }
  // if it's the third time play stuck here
  if (p->getInfo().turnsStuck == 1) { // player must leave
    if (p->getInfo().balance < DCTimsLineFee && p->getInfo().timsCups == 0) {
      throw InsufficientFunds{
          DCTimsLineFee, nullptr,
          "You don't have enough balance or Roll Up the Rim Cup!"};
    } else {
      payBalanceOrCup(p);
      p->moveTo(squares[newIndex]); // good to go (and must go)
      p->makeUnstuck();
    }
  } else { // player can choose to wait or pay or use cup
    while (true) {
      int input;
      std::cout << "Enter '1' to wait or '2' to use balance or cup";
      string inStr;
      getline(cin, inStr);
      istringstream iss{inStr};
      iss >> input;
      if (input == 1) {
        std::cout << "You just choose to wait." << endl;
        p->passTurnStuck();
        break;
      } else if (input == 2) {
        payBalanceOrCup(p);
        p->makeUnstuck();
        p->moveTo(squares[newIndex]);
        break;
      } else {
        std::cout << "Invalid input. Please enter '1' or '2'." << endl;
      }
    }
  }
}

GoToTims::GoToTims(string name) : NonProperty{name} {}

GoToTims::~GoToTims() {}

void GoToTims::triggerEvent(Player *p, std::vector<Square *> &squares,
                            TimsCup *timsCups) {
  int timsIndex = findIndexByName(squares, "DC TIMS LINE");
  p->moveTo(squares[timsIndex]);
  p->makeStuck();
}

GooseNesting::GooseNesting(string name) : NonProperty{name} {}

GooseNesting::~GooseNesting() {}

void GooseNesting::triggerEvent(Player *p, std::vector<Square *> &squares,
                                TimsCup *timsCups) {
  std::cout << "You are attacked by geese!" << endl;
}

Tuition::Tuition(string name) : NonProperty{name} {}

Tuition::~Tuition() {}

void Tuition::triggerEvent(Player *p, std::vector<Square *> &squares,
                           TimsCup *timsCups) {
  size_t input = 0;
  while (true) {
    std::cout << "Enter '1' to pay $" << tuitionPayment
              << " or '2' to pay 10% of your total asset: ";
    string inStr;
    getline(cin, inStr);
    istringstream iss{inStr};
    iss >> input;
    if (input == 1) {
      if (p->makePayment(tuitionPayment)) {
        std::cout << "You have chosen to pay $" << tuitionPayment << "."
                  << endl;
        break;
      } else {
        throw InsufficientFunds{tuitionPayment, nullptr,
                                "You have insufficient funds!"};
      }
    } else if (input == 2) {
      size_t totalAsset = p->getInfo().netWorth;
      p->makePayment(totalAsset);
      std::cout << "You have chosen to pay 10%% of your total assets." << endl;
      break;
    } else {
      std::cout << "Invalid input. Please enter '1' or '2'." << endl;
    }
  }
}

CoopFee::CoopFee(string name) : NonProperty{name} {}

CoopFee::~CoopFee() {}

void CoopFee::triggerEvent(Player *p, std::vector<Square *> &squares,
                           TimsCup *timsCups) {
  if (p->makePayment(coopFee)) {
    std::cout << "You have paid " << coopFee << " as Coop Fee. Cali or Busted!"
              << endl;
  } else {
    throw InsufficientFunds{coopFee, nullptr,
                            "You have insufficient funds!"};
  }
}

SLC::SLC(string name) : NonProperty{name} {}

SLC::~SLC() {}

void SLC::triggerEvent(Player *p, std::vector<Square *> &squares,
                       TimsCup *timsCups) {
  if (timsCups->getAvailable() > 0) {
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
  int timsIndex = findIndexByName(squares, "GO TO TIMS");
  int OSAPIndex = findIndexByName(squares, "COLLECT OSAP");

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
    std::cout << "You just moved to DC Tims Line!" << endl;
    return;
  } else {
    p->moveTo(squares[OSAPIndex]);
    std::cout << "You just moved to Collect OSAP!" << endl;
    return;
  }
  // send the player to the new location
  int newIndex = (index + move + squares.size()) % squares.size();
  p->moveTo(squares[newIndex]);
  std::cout << "You just moved " << newIndex << " and landed on "
            << squares[newIndex]->getInfo().name << "!" << endl;
  return;
}

NeedlesHall::NeedlesHall(string name) : NonProperty{name} {}

NeedlesHall::~NeedlesHall() {}

void NeedlesHall::triggerEvent(Player *p, std::vector<Square *> &squares,
                               TimsCup *timsCups) {
  if (timsCups->getAvailable() > 0) {
    // Generate a random number between 1 and 100
    int rand_num = rand() % 100 + 1;
    if (rand_num == 1) {
      p->addTimsCup();
      timsCups->allocateOne();
    }
  }
  size_t payment = 0;
  size_t gain = 0;

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
      cout << "You have paid " << payment << "." << endl;
    } else {
      throw InsufficientFunds{payment, nullptr,
                              "You do not have enough fund"};
    }
  } else {
    p->addFunds(gain);
    cout<< "You have gained $" << gain << "." << endl;
  }
}
