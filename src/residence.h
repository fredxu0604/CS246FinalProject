#include "property.h"
#include "squareinfo.h"
#include <string>

class Residence: public Property {
    public:
    Residence(string name);
    void improve() override;
    void unimprove() override;
    size_t getVisitFee() const override;
    SquareInfo getInfo() const override;
};
