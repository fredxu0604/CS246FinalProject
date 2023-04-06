#include "squareinfo.h"
#include "property.h"

class Gym: public Property {
    public:
    Gym();
    void addImprovement();
    size_t getVisitFee() const override;
    SquareInfo getInfo() const override;
};


