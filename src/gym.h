#include "squareinfo.h"
#include "property.h"
#include <string>

class Gym: public Property {
    public:
    Gym(string name);
    void improve() override;
    void unimprove() override;
    size_t getVisitFee() const override;
    SquareInfo getInfo() const override;
};


