// textdisplay.cc

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "textdisplay.h"
#include "boardinfo.h"
#include "command.h"
#include "player.h"
#include "square.h"

void TextDisplay::print(std::ostream &out, const BoardInfo &board_info, std::string message) {
    
    // Load the game board from the board.txt file
    std::vector<std::vector<char>> board;   // 2D vector to store the Board.txt display
    std::ifstream board_text("board.txt"); 
    std::string line;
    while (std::getline(board_text, line)){
        std::vector<char> row;
        int string_size = line.length();
        for (int i=0; i<string_size; i++){
            row.push_back(line[i]);
        }
        board.push_back(row);
    }
    
    // Print the game board
    for (const auto &row : board) {
        for (char cell : row) {
            out << cell;
        }
        out << std::endl;
    }

    // Print players' information
    for (const auto &player : board_info.players) {
        auto playerInfo = player->getInfo();
        out << playerInfo.name << " (balance: $" << playerInfo.balance << ")" << std::endl;
    }

    // Print the current player
    auto currPlayerInfo = board_info.currPlayer->getInfo();
    out << "Current player: " << currPlayerInfo.name << std::endl;

    // Print the message, if provided
    if (!message.empty()) {
        out << message << std::endl;
    }
}


void TextDisplay::print(std::ostream &out, std::string message) {
    if (!message.empty()) {
        out << message << std::endl;
    }
}

// The readCommand() method
Command TextDisplay::readCommand() {
    Command command;
    std::string line;

    // Read a line from stdin
    std::getline(std::cin, line);
    std::istringstream iss(line);

    // Read the command type
    std::string cmdType;
    iss >> cmdType;

    // Convert the command type string to the corresponding CommandType enum
    if (cmdType == "trade") {
        command.type = CommandType::Trade;
    } else if (cmdType == "accept") {
        command.type = CommandType::TradeAccept;
    } else if (cmdType == "reject") {
        command.type = CommandType::TradeReject;
    } else if (cmdType == "improve") {
        command.type = CommandType::Improve;
    } else if (cmdType == "mortgage") {
        command.type = CommandType::Mortgage;
    } else if (cmdType == "unmortgage") {
        command.type = CommandType::Unmortgage;
    } else if (cmdType == "bankrupt") {
        command.type = CommandType::Bankrupt;
    } else if (cmdType == "assets") {
        command.type = CommandType::Assets;
    } else if (cmdType == "all") {
        command.type = CommandType::All;
    } else if (cmdType == "save") {
        command.type = CommandType::Save;
    } else if (cmdType == "roll") {
        command.type = CommandType::Roll;
    } else if (cmdType == "next") {
        command.type = CommandType::Next;
    } else if (cmdType == "pay") {
        command.type = CommandType::Pay;
    } else if (cmdType == "usecup") {
        command.type = CommandType::UseCup;
    } else if (cmdType == "tuition") {
        command.type = CommandType::TuitionChoice;
    } else if (cmdType == "bid") {
        command.type = CommandType::AuctionBid;
    } else if (cmdType == "withdraw") {
        command.type = CommandType::AuctionWithdraw;
    } else {
        throw std::runtime_error("Invalid command type");
    }

    // Read the command arguments
    std::string arg;
    while (iss >> arg) {
        command.args.push_back(arg);
    }

    return command;
}
