#ifndef __COMMAND_H__
#define __COMMAND_H__
#include <string>
#include <vector>

enum class CommandType {
  Trade,
  Improve,
  Mortgage,
  Unmortgage,
  Bankrupt,
  Assets,
  All,
  Save,
  Roll,
  Next,

};

struct Command {
  CommandType type;
  std::vector<std::string> args;
};

#endif
