#include "game.h"
#include <iostream>
#include <stdexcept>
#include <string>

const std::string flagTesting = "-testing";
const std::string flagLoad = "-load";

int main(int argc, char **argv) {
  if (argc > 4) {
    std::cout << "Too many arguments." << endl;
    return 1;
  }
  bool is_testing = false;
  bool is_loading = false;
  std::string loadName;
  for (int i = 1; i < argc; i++) {
    if (flagTesting.compare(argv[i]) == 0) {
      is_testing = true;
    }
    if (flagLoad.compare(argv[i]) == 0) {
      is_loading = true;
      try {
        loadName += argv[i + 1];
      } catch (...) {
        std::cout << "You need to provide a filename for the -load option." << endl;
        return 1;
      }
      
    }
  }

  // Initialize Game object with testing flag
  Game game{is_testing};

  if (is_loading) {
    // Load game state from file
    game.loadFromFile(loadName);
  } else {
    // Start a new game
    game.newGame();
  }

  return 0;
}
