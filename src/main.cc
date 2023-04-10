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
    bool is_testing = false;
    bool is_loading = false;
    std::string loadName;
    for (int i = 1; i < argc; i++) {
        if (flagTesting.compare(argv[i])) {
            is_testing = true;
        }
        if (flagLoad.compare(argv[i])) {
            is_loading = true;
            loadName += argv[i+1];
        }

    // Initialize Game object with testing flag
    Game game(is_testing);

    if (is_loading) {
        // Load game state from file
        game.loadFromFile(loadName + ".txt");
    } else {
        // Start a new game
        game.newGame();
    }

    // Run game loop
    game.runGameLoop();

    // Save game state to file
    game.saveToFile(loadName + ".txt");

    return 0;

    }
}
