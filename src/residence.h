#include "property.h"
#include "squareinfo.h"

class Residence: public Property {
    public:
    Residence();
    int getVisitFee() const override;
    SquareInfo getInfo() const override;
};