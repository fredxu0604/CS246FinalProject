#include "game.h"
#include <iostream>
#include <string>

const std::string flagTesting = "-testing";
const std::string flagLoad = "-load";

int main(int argc, char **argv) {
    if (argc != 1 && argc != 3 && argc != 4) {
        std::cout << "Wrong number of arguments" << endl;
        return 1;
    }

    
    
}
