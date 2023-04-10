#ifndef _BOARD_H_
#define _BOARD_H_
#include "boardinfo.h"
#include "command.h"
#include "square.h"
#include "squareinfo.h"

class Player;
class TextDisplay;

class Board {
    std::vector<Square *> theBoard;
    std::vector<Player *> thePlayers;
    Player *currPlayer;

    TextDisplay *td;

    public:
    Board(std::vector<Square *> &squares, std::vector<Player *> players, Player *currPlayer, TextDisplay *td);
    BoardInfo getInfo() const;
    void update(std::string message = "", bool printBoard = true); // calls the print method of td, passes cout as ostream &out.
                                                                  // only call the version that prints the board if printBoard is true.
    void setCurrPlayer(Player *np);
    Command readCommand();
    ~Board();
};




#endif

