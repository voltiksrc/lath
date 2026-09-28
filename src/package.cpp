#include "package.hpp"
#include "archive.hpp"
#include "download.hpp"
#include "recipe.hpp"
#include <fstream>
#include <iostream>

using std::string;

void install_package(const string &recipe_path) {
  Package pkg = load_recipe(recipe_path);

  std::cout << "Installing " << pkg.name << '\n';
  std::cout << "Version: " << pkg.version << '\n';
  string archive_path = download_source(pkg.source, pkg.name, pkg.version);
  if (!archive_path.empty()) {
    extract_source(archive_path);
  }
}
void remove_package(std::string package) {
  std::cout << "Removing " << package << "..." << '\n';
}

void search_package(const string &recipe_path, const string &package) {
  std::ifstream file(recipe_path);
  if (!file) {
    std::cerr << "No packages found for: " << package << ".\n";
    return;
  }
  std::cout << "Package found: " << package << '\n';
}
