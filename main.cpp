#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// just doing print functions for now
void show_help() {
  std::cout << "Lath features/help\n";
  std::cout << '\n';
  std::cout << "lath get <package>\n";
  std::cout << "lath rm <package>\n";
  std::cout << "lath search <package>\n";
  std::cout << "lath --help\n";
}
// FUCK vectors
void search_package(std::vector<string> packages, std::string package) {
  bool found = false;
  for (string pkg : packages) {
    if (pkg == package) {
      found = true;
      break;
    }
  }
  if (found == true) {
    std::cout << "Package found!\n";
  } else {
    std::cout << "Package not found.\n";
  }
}
void install_package(std::string package) {
  std::cout << "Installing " << package << ".." << '\n';
}
void remove_package(std::string package) {
  std::cout << "Removing " << package << ".." << '\n';
}
int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Please enter a command and 'optionally' a package.\n";
    return 1;
  }
  vector<string> packages{"tree", "github", "discord", "prismlauncher"};
  string command = argv[1];
  // command line interaction
  if (command == "--help") {
    show_help();
    return 0;
  }
  if (argc < 3) {
    std::cout << "usage: lath <command> [package]\n";
    return 1;
  }
  string package = argv[2];
  // fuck cout; wish std::printf was normalized
  if (command == "get") {
    install_package(package);
  } else if (command == "search") {
    search_package(packages, package);
  } else if (command == "rm") {
    remove_package(package);
  } else {
    std::cout << "Unknown command: " << argv[1] << '\n';
    return 1;
  }
}
