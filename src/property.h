#ifndef __PROPERTY_H__
#define __PROPERTY_H__
#include "square.h"
#include <map>
#include <string>
using namespace std;



enum class Group {Arts1, Arts2, Eng, Health, Env, Sci1, Sci2, Math};

const map<string, Group> groupMap = {
    {"AL", Group::Arts1},
    {"ML", Group::Arts1},
    {"ECH", Group::Arts2},
    {"PAS", Group::Arts2},
    {"HH", Group::Arts2},
    {"RCH", Group::Eng},
    {"DWE", Group::Eng},
    {"CPH", Group::Eng},
    {"LHI", Group::Health},
    {"BMH", Group::Health},
    {"OPT", Group::Health},
    {"EV1", Group::Env},
    {"EV2", Group::Env},
    {"EV3", Group::Env},
    {"PHYS", Group::Sci1},
    {"B1", Group::Sci1},
    {"B2", Group::Sci1},
    {"EIT", Group::Sci2},
    {"ESC", Group::Sci2},
    {"C2", Group::Sci2},
    {"MC", Group::Math},
    {"DC", Group::Math}
};

const map<string, int> purchaseCostMap = {
    {"AL", 40},
    {"ML", 60},
    {"ECH", 100},
    {"PAS", 100},
    {"HH", 120},
    {"RCH", 140},
    {"DWE", 140},
    {"CPH", 160},
    {"LHI", 180},
    {"BMH", 180},
    {"OPT", 200},
    {"EV1", 220},
    {"EV2", 220},
    {"EV3", 240},
    {"PHYS", 260},
    {"B1", 260},
    {"B2", 280},
    {"EIT", 300},
    {"ESC", 300},
    {"C2", 320},
    {"MC", 350},
    {"DC", 400}
};

const map<string, int> improvementCostMap = {
    {"Arts1", 50},
    {"Arts2", 50},
    {"Eng", 100},
    {"Health", 100},
    {"Env", 150},
    {"Sci1", 150},
    {"Sci2", 200},
    {"Math", 200}
};


const map<string, map<int, int>> tuitionCost = {
    {"AL", {{0, 2}, {1, 10}, {2, 30}, {3, 90}, {4, 160}, {5, 250}}},
    {"ML", {{0, 4}, {1, 20}, {2, 60}, {3, 180}, {4, 320}, {5, 450}}},
    {"ECH", {{0, 6}, {1, 30}, {2, 90}, {3, 270}, {4, 400}, {5, 550}}},
    {"PAS", {{0, 6}, {1, 30}, {2, 90}, {3, 270}, {4, 400}, {5, 550}}},
    {"HH", {{0, 8}, {1, 40}, {2, 100}, {3, 300}, {4, 450}, {5, 600}}},
    {"RCH", {{0, 10}, {1, 50}, {2, 150}, {3, 450}, {4, 625}, {5, 750}}},
    {"DWE", {{0, 10}, {1, 50}, {2, 150}, {3, 450}, {4, 625}, {5, 750}}},
    {"CPH", {{0, 12}, {1, 60}, {2, 180}, {3, 500}, {4, 700}, {5, 900}}},
    {"LHI", {{0, 14}, {1, 70}, {2, 200}, {3, 550}, {4, 750}, {5, 950}}},
    {"BMH", {{0, 14}, {1, 70}, {2, 200}, {3, 550}, {4, 750}, {5, 950}}},
    {"OPT", {{0, 16}, {1, 80}, {2, 220}, {3, 600}, {4, 800}, {5, 1000}}},
    {"EV1", {{0, 18}, {1, 90}, {2, 250}, {3, 700}, {4, 875}, {5, 1050}}},
    {"EV2", {{0, 18}, {1, 90}, {2, 250}, {3, 700}, {4, 875}, {5, 1050}}},
    {"EV3", {{0, 20}, {1, 100}, {2, 300}, {3, 750}, {4, 925}, {5, 1100}}},
    {"PHYS", {{0, 22}, {1, 110}, {2, 330}, {3, 800}, {4, 975}, {5, 1150}}},
    {"B1", {{0, 22}, {1, 110}, {2, 330}, {3, 800}, {4, 975}, {5, 1150}}},
    {"B2", {{0, 24}, {1, 120}, {2, 360}, {3, 850}, {4, 1025}, {5, 1200}}},
    {"EIT", {{0, 26}, {1, 130}, {2, 390}, {3, 900}, {4, 1100}, {5, 1275}}},
    {"ESC", {{0, 26}, {1, 130}, {2, 390}, {3, 900}, {4, 1100}, {5, 1275}}},
    {"C2", {{0, 28}, {1, 150}, {2, 450}, {3, 1000}, {4, 1200}, {5, 1400}}},
    {"MC", {{0, 35}, {1, 175}, {2, 500}, {3, 1100}, {4, 1300}, {5, 1500}}},
    {"DC", {{0, 50}, {1, 200}, {2, 600}, {3, 1400}, {4, 1700}, {5, 2000}}}
};



class Property: public Square {
    protected:
    Group group;
    Player* owner;
    public:
    Property(string name);
    virtual int getVisitFee() const;
    SquareInfo getInfo() const override;
    void setOwner(Player* player);
};

#endif
