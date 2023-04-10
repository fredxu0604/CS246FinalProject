#ifndef __GYM_H__
#define __GYM_H__
#include "property.h"
#include "squareinfo.h"
#include <string>

class Gym : public Property {
public:
  Gym(string name);
  void improve() override;
  void unimprove() override;
  size_t getVisitFee() const override;
  SquareInfo getInfo() const override;
};

#endif
