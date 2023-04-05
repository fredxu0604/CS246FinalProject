#include "property.h"
#include "squareinfo.h"
#include <string>

class Acedemic: public Property {
    protected:
    unsigned int numImprove;
    public:
    Acedemic(string name);
    void improve();
    void unimprove();
    int getVisitFee() const override;
};