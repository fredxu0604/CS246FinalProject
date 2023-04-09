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
        virtual void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) = 0;
        SquareInfo getInfo() const override;
};


class CollectOSAP: public NonProperty {
    public:
        CollectOSAP(string name);
        ~CollectOSAP() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class DCTimsLine: public NonProperty {
    public:
        DCTimsLine(string name);
        ~DCTimsLine() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class GoToTims: public NonProperty {
    public:
        GoToTims(string name);
        ~GoToTims() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class GooseNesting: public NonProperty {
    public:
        GooseNesting(string name);
        ~GooseNesting() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class Tuition: public NonProperty {
    public:
        Tuition(string name);
        ~Tuition() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class CoopFee: public NonProperty {
    public:
        CoopFee(string name);
        ~CoopFee() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class SLC: public NonProperty {
    public:
        SLC(string name);
        ~SLC() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup* timsCups) override;
};

class NeedlesHall: public NonProperty {
    public:
        NeedlesHall(string name);
        ~NeedlesHall() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};


#endif
