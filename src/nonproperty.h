#ifndef __NONPROPERTY_H__
#define __NONPROPERTY_H__


#include "square.h"
#include "squareinfo.h"
#include "timscup.h"
#include <string>

const size_t tuitionPayment = 300;
const size_t coopFee = 150;
const size_t OSAPFee = 200;
const size_t DCTimsLineFee = 50;

const vector<std::string> allNonOwnableSquares{
    "COLLECT OSAP",
    "DC Tims Line",
    "GO TO TIMS",
    "GOOSE NESTING",
    "TUITION",
    "COOP FEE",
    "SLC",
    "NEEDLES HALL"
};

class NonProperty: public Square {
    public:
        NonProperty(string name);
        virtual ~NonProperty() = 0;
        virtual void triggerEvent(Player *p);
        SquareInfo getInfo() const override;
};


class CollectOSAP: public NonProperty {
    public:
        CollectOSAP(string name);
        ~CollectOSAP() override;
        void triggerEvent(Player *p) override;
};

class DCTimsLine: public NonProperty {
    std::vector<Square *> &squares;
    public:
        DCTimsLine(string name, std::vector<Square *> &squares);
        ~DCTimsLine() override;
        void triggerEvent(Player *p) override;
};

class GoToTims: public NonProperty {
    std::vector<Square *> &squares;
    public:
        GoToTims(string name, std::vector<Square *> &squares);
        ~GoToTims() override;
        void triggerEvent(Player *p) override;
};

class GooseNesting: public NonProperty {
    public:
        GooseNesting(string name);
        ~GooseNesting() override;
        void triggerEvent(Player *p) override;
};

class Tuition: public NonProperty {
    public:
        Tuition(string name);
        ~Tuition() override;
        void triggerEvent(Player *p) override;
};

class CoopFee: public NonProperty {
    public:
        CoopFee(string name);
        ~CoopFee() override;
        void triggerEvent(Player *p) override;
};

class SLC: public NonProperty {
    std::vector<Square *> &squares;
    TimsCup* timsCups;
    public:
        SLC(string name, std::vector<Square *> &squares, TimsCup * timsCups);
        ~SLC() override;
        void triggerEvent(Player *p) override;
};

class NeedlesHall: public NonProperty {
    TimsCup* timsCups;
    public:
        NeedlesHall(string name,TimsCup* timsCups);
        ~NeedlesHall() override;
        void triggerEvent(Player *p) override;
};


#endif
