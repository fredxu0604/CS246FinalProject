#ifndef __NONPROPERTY_H__
#define __NONPROPERTY_H__


#include "square.h"
#include "squareinfo.h"
#include <string>

const size_t tuitionPayment = 300;
const size_t coopFee = 150;

class NonProperty: public Square {
    public:
        NonProperty(string name);
        virtual ~NonProperty() = 0;
        virtual void triggerEvent(Player *p);
};


class CollectOSAP: public NonProperty {
    public:
        CollectOSAP(string name);
        ~CollectOSAP() override;
        void triggerEvent(Player *p) override;
        SquareInfo getinfo();
};

class DCTimsLine: public NonProperty {
    public:
        DCTimsLine(string name);
        ~DCTimsLine() override;
        void triggerEvent(Player *p) override;
        SquareInfo getinfo();
};

class GoToTims: public NonProperty {
    public:
        GoToTims(string name);
        ~GoToTims() override;
        void triggerEvent(Player *p) override;
        SquareInfo getinfo();
};

class GooseNesting: public NonProperty {
    public:
        GooseNesting(string name);
        ~GooseNesting() override;
        void triggerEvent(Player *p) override;
        SquareInfo getinfo();
};

class Tuition: public NonProperty {
    public:
        Tuition(string name);
        ~Tuition() override;
        void triggerEvent(Player *p) override;
        SquareInfo getinfo();
};

class CoopFee: public NonProperty {
    public:
        void triggerEvent(Player *p) override;
};

class SLC: public NonProperty {
    public:
        SLC(string name);
        ~SLC() override;
        void triggerEvent(Player *p) override;
};

class NeedlesHall: public NonProperty {
    public:
        NeedlesHall(string name);
        ~NeedlesHall() override;
        void triggerEvent(Player *p) override;
};


#endif