#ifndef __COMMAND_H__
#define __COMMAND_H__
#include <vector>
#include <string>

enum class CommandType {
    Trade,
    TradeAccept,
    TradeReject,
    Improve,
    Mortgage,
    Unmortgage,
    Bankrupt,
    Assets,
    All,
    Save,
    Roll,
    Pay,
    UseCup,
    TuitionChoice,
    AuctionBid,
    AuctionWithdraw

};

struct Command {
    CommandType type;
    std::vector<std::string> args;

};

#endif
