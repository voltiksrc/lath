#include "package.hpp"
#include "archive.hpp"
#include "download.hpp"
#include "recipe.hpp"
#include "runner.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

using std::string;

void install_staged_files(const string &install_dir, const string &root_dir) {
  for (const auto &entry : std::filesystem::recursive_directory_iterator(install_dir)) {
      if (entry.is_regular_file()) {
      auto relative_path = std::filesystem::relative(entry.path(), install_dir);
      std::filesystem::path destination = root_dir / relative_path;
      std::filesystem::create_directories(destination.parent_path());
      std::filesystem::copy_file(entry.path(), destination);
   }
  }
}

void create_manifest(const string &install_dir, const string &manifest_path) {
  std::filesystem::create_directories(".cache/manifests");
  std::ofstream file(manifest_path);
  if (!file) {
    std::cerr << "Failed to create file.\n";
    return;
  }
  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(install_dir)) {
    if (entry.is_regular_file()) {
      auto relative_path = std::filesystem::relative(entry.path(), install_dir);
      file << "/" << relative_path.string() << '\n';
    }
  }
}

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
  string install_dir = ".cache/pkg/" + pkg.name + "-" + pkg.version;
  std::filesystem::remove_all(install_dir);
  string manifest_path = ".cache/manifests/" + pkg.name + "-" + pkg.version;
  std::filesystem::create_directories(install_dir);
  std::cout << "Building " << pkg.name << " " << pkg.version << "...\n";
  run_build(recipe_path, build_dir);
  std::cout << "Build complete.\n";
  run_install(recipe_path, build_dir, install_dir);
  create_manifest(install_dir, manifest_path);
  install_staged_files(install_dir, ".cache/root");
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
