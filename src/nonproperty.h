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

const std::vector<std::string> allNonOwnableSquares{
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
        NonProperty(std::string name);
        virtual ~NonProperty();
        virtual void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) = 0;
        SquareInfo getInfo() const override;
};


class CollectOSAP: public NonProperty {
    public:
        CollectOSAP(std::string name);
        ~CollectOSAP() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class DCTimsLine: public NonProperty {
    public:
        DCTimsLine(std::string name);
        ~DCTimsLine() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class GoToTims: public NonProperty {
    public:
        GoToTims(std::string name);
        ~GoToTims() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class GooseNesting: public NonProperty {
    public:
        GooseNesting(std::string name);
        ~GooseNesting() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class Tuition: public NonProperty {
    public:
        Tuition(std::string name);
        ~Tuition() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class CoopFee: public NonProperty {
    public:
        CoopFee(std::string name);
        ~CoopFee() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};

class SLC: public NonProperty {
    public:
        SLC(std::string name);
        ~SLC() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup* timsCups) override;
};

class NeedlesHall: public NonProperty {
    public:
        NeedlesHall(std::string name);
        ~NeedlesHall() override;
        void triggerEvent(Player *p, std::vector<Square *> &squares, TimsCup *timsCups) override;
};


#endif
