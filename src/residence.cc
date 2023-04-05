#include "property.h"
#include "residence.h"
#include <map>
#include <string>


Residence::Residence(string name): Property{name} {}

int Residence::getVisitFee() const {
    int tuition = tuitionCostMap.at(name).at(numImprove);
    return tuition;
}
