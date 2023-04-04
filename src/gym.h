#include "squareinfo.h"
#include "property.h"

class Gym: public Property {
    public:
    Gym();
    void addImprovement();
    int getVisitFee() const override;
    SquareInfo getInfo() const override;
};