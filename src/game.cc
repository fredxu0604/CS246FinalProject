#include "game.h"
#include "acedemic.h"
#include "board.h"
#include "command.h"
#include "gameexception.h"
#include "gym.h"
#include "nonproperty.h"
#include "player.h"
#include "property.h"
#include "residence.h"
#include "square.h"
#include "state.h"
#include "textdisplay.h"
#include "timscup.h"
#include <exception>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

Game::Game(bool testing, int die1, int die2)
    : numPlayers{0}, numSquares{40}, numProperties{28},
      initialized{false}, gameOn{false}, testing{testing}, die1{die1},
      die2{die2}, gameBoard{nullptr},
      currPlayer{nullptr}, gameState{GameState::PreRoll}, tc{new TimsCup{}},
      squares{vector<Square *>{}}, properties{vector<Property *>{}},
      nonProperties{vector<NonProperty *>{}}, players{vector<Player *>{}} {}

void Game::loadFromFile(string fileName) {
  createSquares();

  ifstream inFile{fileName};

  string line;

  getline(inFile, line);

  istringstream iss{line};

  iss >> numPlayers;

  int timsCupsInUse = 0;

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
    timsCupsInUse += timsCups;
  }

  for (int i = 0; i < timsCupsInUse; ++i) {
    tc->allocateOne();
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
  gameBoard = new Board{squares, players, currPlayer, new TextDisplay{}};
  initialized = true;
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
        break;

      case PropertyType::Residence:
        pp = new Residence{sName};
        break;

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

bool Game::isIn(const map<string, PropertyType> &pNames,
                const string &targetName) {
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

NonProperty *Game::findNonPropertyByName(string nonPropertyName) {
  for (auto np : nonProperties) {
    if (np->getInfo().name == nonPropertyName)
      return np;
  }
  return nullptr;
}

void Game::handleArrival() {
  gameState = GameState::SquareArrival;
  PlayerInfo currPlayerInfo = currPlayer->getInfo();
  SquareInfo currSquareInfo = currPlayerInfo.currSquare->getInfo();

  string sName = currSquareInfo.name;
  NonProperty *npp = findNonPropertyByName(sName);

  try {
    if (npp != nullptr) {

      Square *currSquare = currPlayerInfo.currSquare;
      gameBoard->update("You are now at " + currSquare->getInfo().name + ".",
                        false);
      npp->triggerEvent(currPlayer, squares, tc);

      if (currPlayer->getInfo().currSquare != currSquare) {
        handleArrival();
        return;
      }

      gameState = GameState::IdleTurn;
      return;
    }

    Property *pp = findPropertyByName(currSquareInfo.name);

    if (pp != nullptr) {
      SquareInfo pInfo = pp->getInfo();
      gameBoard->update("You are now at " + pInfo.name + ".", false);
      Player *pOwner = pInfo.owner;

      if (pOwner != nullptr) {

        size_t visitFee = pp->getVisitFee();

        gameBoard->update("This property is owned by " +
                          pOwner->getInfo().name + "\nYou must pay $" +
                          to_string(visitFee) + "!");

        if (!currPlayer->makePayment(visitFee))
          throw InsufficientFunds{visitFee, pInfo.owner, "Cannnot make rent!"};

        pOwner->addFunds(visitFee);
        gameState = GameState::IdleTurn;
        gameBoard->update("You have paid rent.", false);
        return;

      } else {
        buyOrAuctionLoop();
        return;
      }
    }

    throw Disallowed{"Somehow encountered a square that is neither property "
                     "nor nonproperty!"};

  } catch (InsufficientFunds e) {
    moneyCriticalLoop(e.getOwedAmount(), e.getOwedTo());
    gameState = GameState::MoneyCritical;
    return;
  }
}

void Game::saveToFile(string fileName) {
  std::ofstream file(
      fileName + ".txt"); // Create or overwrite the file with the given name
  if (file.is_open()) {
    // Write the number of players to the file
    file << numPlayers << endl;
    // Write each player's data to the file
    for (auto player : players) {
      file << player->getInfo().name << " " << player->getInfo().avatar << " "
           << player->getInfo().timsCups << " " << player->getInfo().balance
           << " "
           << this->getSquareIdx(player->getInfo().currSquare->getInfo().name);
      if (this->getSquareIdx(player->getInfo().currSquare->getInfo().name) ==
          10) {
        if (player->getInfo().turnsStuck > 0) {
          file << 1 << 3 - player->getInfo().turnsStuck;
        } else {
          file << 0;
        }
      }
      file << endl;
    }
    // Write each square's data to the file
    for (auto property : properties) {
      file << property->getInfo().name << " ";
      if (property->getInfo().isOwned) {
        file << property->getInfo().owner << " ";
      } else {
        file << "BANK ";
      }
      file << property->getInfo().numImprove << endl;
    }
    file.close();
  }
}

void Game::runGameLoop() {
  while (true) {
    if (numPlayers < 2)
      break;

    Command cmd = gameBoard->readCommand();

    try {

      // ASDKJHASKJFHJKASJHDKJHASASDKJHKJASHD

    } catch (Disallowed e) {

      gameBoard->update(e.getMessage(), false);
    }
    stopGame();
  }
}

// trade property with property
void Game::trade(std::string name, std::string give, std::string receive) {
  Player *otherPlayer = findPlayerByName(name);
  currPlayer->trade(otherPlayer, findPropertyByName(give),
                    findPropertyByName(receive));
}

// trade property with balance
void Game::trade(std::string name, size_t give, std::string receive) {
  Player *otherPlayer = findPlayerByName(name);
  currPlayer->trade(otherPlayer, give, findPropertyByName(receive));
}

// trade balance with property
void Game::trade(std::string name, std::string give, size_t receive) {
  Player *otherPlayer = findPlayerByName(name);
  currPlayer->trade(otherPlayer, findPropertyByName(give), receive);
}

void Game::improve(string property, string mode) {
  if (mode == "buy") {
    currPlayer->buyImprovement(property);
    return;
  }
  if (mode == "sell") {
    currPlayer->sellImprovement(property);
    return;
  }
}

void Game::cannotUseThisCommand() {
  gameBoard->update("Cannot use this command right now.");
}

void Game::tradeSwitch(string name, string give, string receive) {
  stringstream ss1(give), ss2(receive);
  size_t give_value, receive_value;

  if ((ss1 >> give_value) && ss1.eof()) {
    // The second parameter can be cast as a size_t
    trade(name, give_value, receive);
  } else if ((ss2 >> receive_value) && ss2.eof()) {
    // The third parameter can be cast as a size_t
    trade(name, give, receive_value);
  } else {
    // Neither parameter can be cast as a size_t
    trade(name, give, receive);
  }
}

void Game::moneyCriticalLoop(size_t owes, Player *owedTo) {
  gameBoard->update("You owe more money than you have!\n Mortgage properties "
                    "or trade with other players to raise money (or declare "
                    "bankruptcy if you're truly done for)",
                    false);
  gameBoard->update(
      "You need to raise your balance to $" + to_string(owes) + "!", false);

  while (true) {
    Command cmd = gameBoard->readCommand();

    switch (cmd.type) {
    case CommandType::Mortgage:
      try {
        currPlayer->mortgage(cmd.args[0]);
        gameBoard->update("You have mortgaged " + cmd.args[0] + "!");
      } catch (out_of_range) {
        gameBoard->update("please provide a property name.");
      } catch (Disallowed e) {
        gameBoard->update(e.getMessage(), false);
      }
      break;

    case CommandType::Trade:
      try {
        tradeLoop(cmd.args);
      } catch (Disallowed e) {
        gameBoard->update(e.getMessage());
      }
      break;

    case CommandType::Bankrupt:
      bankrupt(owedTo);
      break;

    default:
      cannotUseThisCommand();
      break;
    }

    if (currPlayer->getInfo().balance >= owes) {
      gameBoard->update(
          "You have raised enough money to pay off your debt! Good job!",
          false);
      currPlayer->makePayment(owes);

      if (owedTo != nullptr)
        owedTo->addFunds(owes);

      return;
    }

    gameBoard->update("You need to raise more money! Still missing $" +
                          to_string(owes - currPlayer->getInfo().balance) + ".",
                      false);
  }
}
