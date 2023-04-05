#include "property.h"
#include "squareinfo.h"

class Acedemic: public Property {
    protected:
    unsigned int numImprovement;
    public:
    Acedemic();
    void addImprovement();
    int getVisitFee() const override;
    SquareInfo getInfo() const override;
};