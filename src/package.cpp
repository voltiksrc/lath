#include "package.hpp"
#include "archive.hpp"
#include "build.hpp"
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

  if (archive_path.empty()) {
    std::cerr << "Download failed.\n";
    return;
  }

  extract_source(archive_path);

  string build_dir = ".cache/build/" + pkg.name + "-" + pkg.version;
  std::cout << "Building " << pkg.name << " " << pkg.version << "...\n";
  run_build(recipe_path, build_dir);
  std::cout << "Build complete.\n";
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
