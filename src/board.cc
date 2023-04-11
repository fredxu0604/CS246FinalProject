#include "board.h"
#include "player.h"
#include "textdisplay.h"
#include <iostream>

Board::Board(std::vector<Square *> &squares, std::vector<Player *> players,
             Player *currPlayer, TextDisplay *td)
    : theBoard(squares), thePlayers(players), currPlayer(currPlayer), td(td) {}

BoardInfo Board::getInfo() const {
  return BoardInfo{currPlayer, theBoard, thePlayers};
}

void Board::update(std::string message, bool printBoard) {
  if (printBoard) { // only call the version that prints the board if printBoard
                    // is true.
    td->print(
        std::cout, getInfo(),
        message); // calls the print method of td, passes cout as ostream &out.
  } else {
    td->print(std::cout, message);
  }
}

void Board::setCurrPlayer(Player *np) { currPlayer = np; }

Board::~Board() { delete td; }

Command Board::readCommand() { return td->readCommand(); }

void Board::setPlayers(const std::vector<Player *> &newPlayers) {
  thePlayers = newPlayers;
}
