#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// just doing print functions for now
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
    std::cout << "Package found: " << package << '\n';
  } else {
    std::cout << "Package not found.\n";
  }
}
void install_package(vector<string> &installed_packages, std::string package) {
  std::cout << "Installing " << package << "..." << '\n';
  bool installed = false;
  for (string installed_pkgs : installed_packages) {
    if (installed_pkgs == package) {
      installed = true;
      break;
    }
  }
  if (installed == true) {
    std::cout << "Package already installed!\n";
  } else if (installed == false) {
    installed_packages.push_back(package);
    std::cout << package << " installed!\n";
  }
}
void remove_package(std::string package) {
  std::cout << "Removing " << package << "..." << '\n';
}
int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Please enter a command and 'optionally' a package.\n";
    return 1;
  }
  // vectors make me wanna die >:(
  vector<string> packages{"tree", "github", "discord", "prismlauncher"};
  vector<string> installed_packages;
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
  // command checks for validation
  if (command == "get") {
    install_package(installed_packages, package);
  } else if (command == "search") {
    search_package(packages, package);
  } else if (command == "rm") {
    remove_package(package);
  } else {
    std::cout << "Unknown command: " << argv[1] << '\n';
    return 1;
  }
}
