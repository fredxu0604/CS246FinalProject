#ifndef __BOARDINFO_H__
#define __BOARDINFO_H__
#include <vector>
#include <string>

class Square;
class Player;

struct BoardInfo {
    Player *currPlayer;
    const std::vector<Square *> &squares;
    const std::vector<Player *> &players;
};

#endif
