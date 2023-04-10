#ifndef __TIMSCUP_H__
#define __TIMSCUP_H__

class TimsCup {
  int available;
  int inUse;

public:
  TimsCup(int available = 4, int inUse = 0);
  int getAvailable();
  bool allocateOne();
  bool freeOne();
};

#endif
