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
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

Game::Game(bool testing, int die1, int die2)
    : numPlayers{0}, numSquares{40}, numProperties{28},
      initialized{false}, testing{testing}, die1{die1},
      die2{die2}, gameBoard{nullptr},
      currPlayer{nullptr}, gameState{GameState::PreRoll}, tc{nullptr},
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

  tc = new TimsCup{4 - timsCupsInUse, timsCupsInUse};

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

      gameState = GameState::PostRoll;
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
        gameState = GameState::PostRoll;
        gameBoard->update("You have paid rent.", false);
        return;

      } else {
        buyOrAuctionLoop();
        return;
      }
    }

    throw Disallowed{"Somehow encountered a square that is neither property "
                     "nor nonproperty!"};

  } catch (InsufficientFunds &e) {
    moneyCriticalLoop(e.getOwedAmount(), e.getOwedTo());
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
  if (!initialized) {
    cout << "Game is not initialized." << endl;
    return;
  }
    
  while (true) {
    if (numPlayers < 2)
      break;

    Command cmd = gameBoard->readCommand();

    try {

      switch (cmd.type) {

      case CommandType::All:
        displayAll();
        break;

      case CommandType::Assets:
        displayAssets();
        break;

      case CommandType::Bankrupt:
        cannotUseThisCommand();
        break;

      case CommandType::Improve:
        try {
          improve(cmd.args[0], cmd.args[1]);
        } catch (out_of_range) {
          gameBoard->update(
              "Please provide the name of the desired property and whether you "
              "want to buy or sell an improvement.");
        }
        break;

      case CommandType::Mortgage:
        try {

          currPlayer->mortgage(cmd.args[0]);
        } catch (out_of_range) {
          gameBoard->update(
              "Please provide the name of the property you want to mortgage.");
        }
        break;

      case CommandType::Next:
        if (gameState != GameState::PostRoll) {
          cannotUseThisCommand();
        } else {
          next();
        }
        break;

      case CommandType::Roll:
        if (gameState != GameState::PreRoll) {
          cannotUseThisCommand();
        } else {
          roll();
        }
        break;

      case CommandType::Save:
        try {
          saveToFile(cmd.args[0]);
        } catch (out_of_range) {
          gameBoard->update("Please specify a file name.");
        }
        break;

      case CommandType::Trade:
        tradeLoop(cmd.args);

      case CommandType::Unmortgage:
        try {
          currPlayer->unmortgage(cmd.args[0]);
        } catch (out_of_range) {
          gameBoard->update("Please provide the name of the property you want "
                            "to unmortgage.");
        }
        break;
      }

    } catch (Disallowed &e) {

      gameBoard->update(e.getMessage());
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

// Display all properties for all players
void Game::displayAll() {
  std::cout << "All properties for all players:" << std::endl;
  // Since we have a vector of Player pointers called players which include all
  // players.
  for (const auto &player : players) {
    PlayerInfo playerInfo = player->getInfo();

    std::cout << "Player " << playerInfo.name << "'s properties:" << std::endl;
    // loop over all properties for current player
    for (const auto &property : playerInfo.ownedProperties) {
      SquareInfo propertyInfo = property->getInfo();
      std::cout << propertyInfo.name << std::endl;
    }
  }
}

// Display all properties for the current player
void Game::displayAssets() {
  PlayerInfo currPlayerInfo = currPlayer->getInfo();
  std::cout << "Current player " << currPlayerInfo.name
            << "'s properties:" << std::endl;
  // loop over all properties for current player
  for (const auto &property : currPlayerInfo.ownedProperties) {
    SquareInfo propertyInfo = property->getInfo();
    std::cout << propertyInfo.name << std::endl;
  }
}

void Game::cannotUseThisCommand() {
  gameBoard->update("Cannot use this command right now.");
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
      } catch (Disallowed &e) {
        gameBoard->update(e.getMessage(), false);
      }
      break;

    case CommandType::Trade:
      try {
        tradeLoop(cmd.args);
      } catch (Disallowed &e) {
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

void Game::tradeLoop(const vector<std::string> &args) {
  std::string answer;
  while (true) {
    cout << "Do you accept the action? (accept/refuse): ";
    getline(cin, answer);
    if (answer == "accept") {
      tradeSwitch(args[0], args[1], args[2]);
      break;
    } else if (answer == "refuse") {
      break;
    } else {
      cout << "Invalid answer. Please enter 'accept' or 'refuse'." << std::endl;
    }
  }
}

void Game::tradeSwitch(string name, string give, string receive) {
  stringstream ss1(give), ss2(receive);
  size_t give_value, receive_value;

  if ((ss1 >> give_value) && ss1.eof()) {
    // The second parameter can be cast as a size_t
    if (!findPropertyByName(receive)) {
      throw Disallowed{"No property can be found for this receive."};
    }
    trade(name, give_value, receive);
  } else if ((ss2 >> receive_value) && ss2.eof()) {
    // The third parameter can be cast as a size_t
    if (!findPropertyByName(give)) {
      throw Disallowed{"No property can be found for this give."};
    }
    trade(name, give, receive_value);
  } else {
    if (!findPropertyByName(give) || !findPropertyByName(receive)) {
      throw Disallowed{"No property can be found for this trade."};
    }
    // Neither parameter can be cast as a size_t
    trade(name, give, receive);
  }
}

int Game::getPlayerIdx(Player *p) {
  for (int i = 0; i < players.size(); ++i) {
    if (players[i] == p)
      return i;
  }
  return -1;
}

void Game::next() {
  gameState = GameState::PreRoll;
  int cpIdx = getPlayerIdx(currPlayer);

  currPlayer = players[(cpIdx + 1) % players.size()];
  gameBoard->setCurrPlayer(currPlayer);
  gameBoard->update("It is now " + currPlayer->getInfo().name + "'s turn.");
}

void Game::roll() {

  if (currPlayer->getInfo().turnsStuck != 0) {
    gameBoard->update(
        "You are stuck at DC Tims and cannot move (you NEED coffee)!");
    gameState = GameState::PostRoll;
    return;
  }

  int r1, r2;

  if (testing) {
    r1 = die1;
    r2 = die2;
  } else {
    r1 = currPlayer->roll();
    r2 = currPlayer->roll();
  }

  gameBoard->update("You rolled " + to_string(r1) + " and " + to_string(r2) +
                    "!");
  int totalMove = r1 + r2;

  int currSquareIdx = getSquareIdx(currPlayer->getInfo().currSquare);

  int newSquareIdx = (currSquareIdx + totalMove) % squares.size();

  if (newSquareIdx < currSquareIdx) {
    NonProperty *osap = nonProperties[0];
    osap->triggerEvent(currPlayer, squares, tc);
  }

  currPlayer->moveTo(squares[newSquareIdx]);
  handleArrival();
}

int Game::getSquareIdx(Square *sp) {
  for (int i = 0; i < squares.size(); ++i) {
    if (squares[i] == sp)
      return i;
  }
  return -1;
}

void Game::auctionLoop(Property *p) {
  std::vector<Player *> biddingVector(players);
  size_t highestBid = 0;
  Player *highestBidder = nullptr;
  gameBoard->update("Now we are bidding for the asset " + p->getInfo().name);
  while (biddingVector.size() > 0) {
    if (biddingVector.size() == 1) { // when there is only one player left
      highestBidder->makePayment((size_t)highestBid);
      highestBidder->addProperty(p);
    }

    for (auto iter = biddingVector.begin(); iter != biddingVector.end();
         ++iter) {
      Player *currentPlayer = *iter;
      gameBoard->update("Now it's " + currentPlayer->getInfo().name +
                        "'s turn.");

      if (currentPlayer == highestBidder) {
        // If the current player has already bid the highest amount, they can
        // pass.
        gameBoard->update("You have already bid the highest amount. Type "
                          "'pass' to skip this turn.");
        string input;
        getline(cin, input);
        if (input == "pass") {
          continue;
        }
      }
      gameBoard->update("The current highest bid is " + to_string(highestBid) +
                        ".");
      gameBoard->update("How much do you want to bid? (Type 'quit' to withdraw "
                        "from the auction)");

      string input;
      getline(std::cin, input);

      if (input == "quit") {
        biddingVector.erase(iter);
        --iter;
        continue;
      }

      int bid = stoi(input);
      if ((size_t)bid > currentPlayer->getInfo().balance) {
        throw Disallowed{"You don't have enough money to make this bid."};
      }

      if (bid <= highestBid) {
        gameBoard->update(
            "Your bid must be higher than the current highest bid.");
        continue;
      }

      highestBid = bid;
      highestBidder = currentPlayer;
    }
  }
}

void Game::newGame() {
  createSquares();

  numPlayers = 6;
  vector<char> allowedAvatars{'G', 'B', 'D', 'P', 'S', '$', 'L', 'T'};

  for (int i = 0; i < numPlayers; ++i) {
    string name;

    while (!(cin >> name)) {
      cout << "Please enter a name for this player." << endl;
    }

    char avatar;

    while (true) {
      cout << "Please select this player's avatar." << endl;
      if (!(cin >> avatar)) {
        continue;
      }

      vector<char>::iterator it = allowedAvatars.begin();

      bool found = false;
      for (; it != allowedAvatars.end(); it++) {
        if (*it == avatar) {
          found = true;
          allowedAvatars.erase(it);
          break;
        }
      }

      if (found) {
        break;
      }
    }

    players.emplace_back(
        new Player{name, avatar, squares[0], vector<Property *>{}}, 1500, false, 0, 0);
  }
  currPlayer = players[0];
  gameBoard = new Board{squares, players, currPlayer, new TextDisplay{}};
  tc = new TimsCup{};
  gameState = GameState::PreRoll;
  initialized = true;
  runGameLoop();

}

void Game::buyOrAuctionLoop() {
  gameBoard->update(
      "Would you like to buy " +
      currPlayer->getInfo().currSquare->getInfo().name + " for $" +
      to_string(currPlayer->getInfo().currSquare->getInfo().cost) + "? (y/n) ");
  char choice;
  cin >> choice;

  while (choice != 'y' && choice != 'n') {
    gameBoard->update("Invalid input. Please enter 'y' or 'n': ");
    cin >> choice;
  }

  if (choice == 'y') {
    currPlayer->buyProperty(
        findPropertyByName(currPlayer->getInfo().currSquare->getInfo().name));
    gameBoard->update("Congratulations! You are now the owner of " +
                      currPlayer->getInfo().currSquare->getInfo().name + ".");
  } else {
    auctionLoop(
        findPropertyByName(currPlayer->getInfo().currSquare->getInfo().name));
  }
}

void Game::stopGame() {
  delete gameBoard;
  gameBoard = nullptr;
  delete tc;
  tc = nullptr;

  for (auto p : squares) {
    delete p;
    p = nullptr;
  }

  for (auto p : players) {
    delete p;
     p = nullptr;
  }
}
