#include "game.h"
#include "acedemic.h"
#include "nonproperty.h"
#include "player.h"
#include "property.h"
#include "square.h"
#include <exception>
#include <fstream>
#include <sstream>
#include "gym.h"
#include "residence.h"

using namespace std;

Game::Game(bool testing, int die1, int die2)
    : numPlayers{0}, numSquares{40}, numProperties{28}, initialized{false},
      gameOn{false}, testing{testing}, die1{die1}, die2{die2},
      gameBoard{nullptr}, currPlayer{nullptr}, gameState{GameState::PreRoll},
      squares{vector<Square *>{}}, properties{vector<Property *>{}},
      nonProperties{vector<NonProperty *>{}}, players{vector<Player *>{}} {}

void Game::loadFromFile(string fileName) {
  createSquares();

  ifstream inFile{fileName};

  string line;

  getline(inFile, line);

  istringstream iss{line};

  iss >> numPlayers;

  for (int i = 0; i < numPlayers; ++i) {
    getline(inFile, line);

    istringstream iss{line};

    string name;
    char avatar;
    int timsCups;
    size_t money;
    int position;

    iss >> name;
    iss >> avatar;
    iss >> timsCups;
    iss >> money;
    iss >> position;

    int stuckSignal = 0;
    int turnsStuck = 0;

    if (position == getSquareIdx("DC Tims Line"))
      iss >> stuckSignal;

    if (stuckSignal == 1) {
      iss >> turnsStuck;
      turnsStuck = 3 - turnsStuck;
    }
      
    
    players.emplace_back(new Player{name, avatar, squares[position],
                                    vector<Property *>{}, money, money == -1,
                                    turnsStuck, timsCups});
  }
  

  for (int i = 0; i < numProperties; ++i) {
    getline(inFile, line);

    istringstream iss{line};

    string propertyName;
    string ownerName;
    int numImprovements;

    iss >> propertyName;
    iss >> ownerName;
    iss >> numImprovements;

    Player *owner;
    
    if (ownerName == "BANK") {
      owner = nullptr;
    } else {
      owner = findPlayerByName(ownerName);
    }
      
    
    Property *property = findPropertyByName(propertyName);
    for (int i = 0; i < numImprovements; ++i) {
      property->improve();
    }
    owner->addProperty(property);
    property->setOwner(owner);

  }

  currPlayer = players[0];

  runGameLoop();
}


void Game::createSquares() {
  for (auto sName : allSquareNames) {
    // is it a non-property?
    if (isIn(allNonOwnableSquares, sName)) {
      NonProperty *npp;
      if (sName == "COLLECT OSAP") {
        npp = new CollectOSAP{sName};
      } else if (sName == "DC TIMS LINE") {
        npp = new DCTimsLine{sName};
      } else if (sName == "GO TO TIMS") {
        npp = new GoToTims{sName};
      } else if (sName == "GOOSE NESTING") {
        npp = new GooseNesting{sName};
      } else if (sName == "TUITION") {
        npp = new Tuition{sName};
      } else if (sName == "COOP FEE") {
        npp = new CoopFee{sName};
      } else if (sName == "SLC") {
        npp = new SLC{sName};
      } else {
        npp = new NeedlesHall{sName};
      }

      squares.emplace_back(npp);
      nonProperties.emplace_back(npp);

    } else if (isIn(residenceGymMapByNames, sName)) {

      PropertyType sType = residenceGymMapByNames.at(sName);
      Property *pp;
      
      switch (sType) {
        case PropertyType::Gym:
          pp = new Gym{sName};

        case PropertyType::Residence:
          pp = new Residence{sName};
          
        case PropertyType::Acedemic:
          break;
      }
      squares.emplace_back(pp);
      properties.emplace_back(pp);
    } else {
      // sName corresponds to an academic property!
      Property *pp = new Acedemic{sName};
      squares.emplace_back(pp);
      properties.emplace_back(pp);
    }
  }
}

bool Game::isIn(const vector<string> &pNames, const string &targetName) {
  for (auto &s : pNames) {
    if (s == targetName)
      return true;
  }
  return false;
}

bool Game::isIn(const map<string, PropertyType> &pNames, const string &targetName) {
  return pNames.count(targetName) == 1;
}

int Game::getSquareIdx(string squareName) {
  for (int i = 0; i < squares.size(); ++i) {
    if (squares[i]->getInfo().name == squareName)
      return i;
  }
  return -1;
}

Player *Game::findPlayerByName(string name) {
  for (auto p : players) {
    if (p->getInfo().name == name)
      return p; 
  }
  return nullptr;
}

Property *Game::findPropertyByName(string propertyName) {
  for (auto p : properties) {
    if (p->getInfo().name == propertyName)
      return p;
  }
  return nullptr;
}
