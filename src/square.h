#ifndef __SQUARE_H__
#define __SQUARE_H__

#include <string>
#include <vector>
#include "squareinfo.h"

using namespace std;

class Player;

const std::vector<string> allSquareNames{
  "COLLECT OSAP",
  "AL",
  "SLC",
  "ML",
  "TUITION",
  "MKV",
  "ECH",
  "NEEDLES HALL",
  "PAS",
  "HH",
  "DC TIMS LINE",
  "RCH",
  "PAC",
  "DWE",
  "CPH",
  "UWP",
  "LHI",
  "SLC",
  "BMH",
  "OPT",
  "GOOSE NESTING",
  "EV1",
  "NEEDLES HALL",
  "EV2",
  "EV3",
  "V1",
  "PHYS",
  "B1",
  "CIF",
  "B2",
  "GO TO TIMS",
  "EIT",
  "ESC",
  "SLC",
  "C2",
  "REV",
  "NEEDLES HALL",
  "MC",
  "COOP FEE",
  "DC"
  
};

class Square {
    protected:
    string name;
    vector<Player*> visitors;
    public:
    Square(string name);
    virtual ~Square() = 0;
    virtual SquareInfo getInfo() const = 0;
    
};

#endif
