#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include <vector>
#include <string>
#include "boardinfo.h"
#include "command.h"



class Game;
class Board;

class TextDisplay {
    
    Board *game;
    
    std::vector<std::vector<char>> board;// 2D vector to store the Board.txt file

public:
    TextDisplay(Board *game);
    
    void print(std::ostream &out, const BoardInfo &board, std::string message = ""); // print the board and message (if exists) in this version
    
    void print(std::ostream &out, std::string message = ""); // if message is "" then return (though that should never happen)
    
    Command readCommand(); // reads a command from stdin into a Command object. see command.h for info
};

#endif

