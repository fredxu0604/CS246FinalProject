#ifndef __TIMSCUP_H__
#define __TIMSCUP_H__

class TimsCup {
    int available;
    int inUse;

    public:
    int getAvailable();
    bool allocateOne();
    bool freeOne();
};


#endif
