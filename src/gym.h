#include "squareinfo.h"
#include "property.h"

class Gym: public Property {
    public:
    Gym();
    void improve() override;
    void unimprove() override;
    size_t getVisitFee() const override;
    SquareInfo getInfo() const override;
};


