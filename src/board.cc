#include "board.h"
#include "square.h"
#include "player.h"
#include "textdisplay.h"
#include <iostream>

Board::Board(std::vector<Square *> &squares, std::vector<Player *> players, Player *currPlayer, TextDisplay *td)
    : theBoard(squares), thePlayers(players), currPlayer(currPlayer), td(td) {}

BoardInfo Board::getInfo() const {
    BoardInfo boardInfo;
    boardInfo.currPlayer = currPlayer;
    boardInfo.squares = theBoard;
    boardInfo.players = thePlayers;
    return boardInfo;
}

void Board::update(std::string message, bool printBoard) {
    if (printBoard) {
        td->print(std::cout, getInfo(), message);
    } else {
        td->print(std::cout, message);
    }
}

void Board::setCurrPlayer(Player *np) {
    currPlayer = np;
}
