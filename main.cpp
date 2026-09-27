#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

void install_package(std::string package) {
  std::cout << "Installing " << package << ".." << '\n';
}

void remove_package(std::string package) {
  std::cout << "Removing " << package << ".." << '\n';
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cout << "usage: lath <command> [package]\n";
    return 1;
  }
  string package = argv[2];
  string command = argv[1];
  if (command == "get") {
    install_package(package);
  } else if (command == "rm") {
    remove_package(package);
  } else {
    std::cout << "Unknown command: " << argv[1] << '\n';
    return 1;
  }
}
