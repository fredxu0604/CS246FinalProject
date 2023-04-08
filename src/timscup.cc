#include "timscup.h"

int TimsCup::getAvailable() {
    return available;
}

bool TimsCup::allocateOne() {
    if (available == 0)
        return false;

    --available;
    ++inUse;
    return true;
}

bool TimsCup::freeOne() {
    if (inUse == 0)
        return false;

    ++available;
    --inUse;
    return true;
}
