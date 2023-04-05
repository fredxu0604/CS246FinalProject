#include "square.h"
#include "squareinfo.h"
#include <string>

class NonProperty: public Square {
    public:
        NonProperty(string name);
        virtual ~NonProperty() = 0;
        virtual void triggerEvent();
        SquareInfo getInfo() const override;
};


class CollectOSAP: public NonProperty {
    public:
        CollectOSAP(string name);
        ~CollectOSAP() override;
        void triggerEvent() override;
};

class DCTimsLine: public NonProperty {
    public:
        DCTimsLine(string name);
        ~DCTimsLine() override;
        void triggerEvent() override;
};

class GoToTims: public NonProperty {
    public:
        GoToTims(string name);
        ~GoToTims() override;
        void triggerEvent() override;
};

class GooseNesting: public NonProperty {
    public:
        GooseNesting(string name);
        ~GooseNesting() override;
        void triggerEvent() override;
};

class Tuition: public NonProperty {
    public:
        Tuition(string name);
        ~Tuition() override;
        void triggerEvent() override;
};

class CoopFee: public NonProperty {
    public:
        void triggerEvent() override;
};

class SLC: public NonProperty {
    public:
        SLC(string name);
        ~SLC() override;
        void triggerEvent() override;
};


class NeedlesHall: public NonProperty {
    public:
        NeedlesHall(string name);
        ~NeedlesHall() override;
        void triggerEvent() override;
};