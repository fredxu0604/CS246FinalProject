#include "property.h"
#include "squareinfo.h"

class Acedemic: public Property {
    protected:
    unsigned int numImprovement;
    public:
    Acedemic();
    void addImprovement();
    int getTuition();
    //more to come


};