#include "package.hpp"
#include <iostream>
#include <string>

using std::string;

void lath_version() {
  double version{0.1};
  std::cout << "lath " << version << '\n';
}
// for noobies(when will printf become normalized^^)
void show_help() {
  std::cout << "lath features/help\n";
  std::cout << '\n';
  std::cout << "lath get <package>\n";
  std::cout << "lath rm <package>\n";
  std::cout << "lath search <package>\n";
  std::cout << "lath --help\n";
}
int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Please enter a command and 'optionally' a package.\n";
    return 1;
  }

  string command = argv[1];
  // command line interaction
  if (command == "--help") {
    show_help();
    return 0;
  } else if (command == "--version") {
    lath_version();
    return 0;
  }
  if (argc < 3) {
    std::cout << "usage: lath <command> [package]\n";
    return 1;
  }
  string package = argv[2];
  string recipe_path = "recipes/" + package + "/package.lua";
  // command checks for validation
  if (command == "get") {
    install_package(recipe_path);
  } else if (command == "search") {
    // search_package(package);
  } else if (command == "rm") {
    remove_package(package);
  } else {
    std::cout << "Unknown command: " << argv[1] << '\n';
    return 1;
  }
}
