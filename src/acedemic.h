#ifndef __ACEDEMIC_H__
#define __ACEDEMIC_H__
#include "property.h"
#include "squareinfo.h"
#include <string>

class Acedemic : public Property {
protected:
  unsigned int numImprove;
  Group group;

public:
  Acedemic(string name);
  void improve() override;
  void unimprove() override;
  size_t getVisitFee() const override;
  SquareInfo getInfo() const override;
};
#endif
