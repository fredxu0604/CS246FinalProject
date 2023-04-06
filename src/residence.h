#include "property.h"
#include "squareinfo.h"
#include <string>

class Residence: public Property {
    public:
    Residence(string name);
    size_t getVisitFee() const override;
    SquareInfo getInfo() const override;
};
