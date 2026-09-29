#include "package.hpp"
#include "archive.hpp"
#include "download.hpp"
#include "recipe.hpp"
#include "runner.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <system_error>

using std::string;

bool install_staged_files(const string &install_dir, const string &root_dir) {
  for (const auto &entry : std::filesystem::recursive_directory_iterator(install_dir)) {
      auto relative_path = entry.path().lexically_relative(install_dir);
      std::filesystem::path destination = root_dir / relative_path;
      std::filesystem::create_directories(destination.parent_path());
      if (entry.is_symlink()) {
          auto target = std::filesystem::read_symlink(entry.path());
          /*  We recreate the symlink inside the fake root dir to resolve
           *   the broken symlinks. */
          std::error_code ec;
          std::filesystem::remove(destination, ec);
          ec.clear();
          std::filesystem::create_symlink(target, destination, ec);
          if (ec) {
              std::cerr<<"Symlink failed: "<<ec.message()<<'\n';
              return false;
          }
      }
      else if (entry.is_regular_file()) {
      // If it isnt a symlink then we copy it normally.
      // error code function for better error troubleshooting on copy
      std::error_code ec;
      std::filesystem::copy_file(
          entry.path(),
          destination,
          std::filesystem::copy_options::overwrite_existing,
          ec
      );
      if (ec) {
          std::cerr<<"Copy failed: "<<ec.message()<<'\n';
          return false;
      }
    }
  }
  return true;
}

void create_manifest(const string &install_dir, const string &manifest_path) {
  std::filesystem::create_directories(".cache/manifests");
  std::ofstream file(manifest_path);
  // check to see if the file failed to create so we can prevent it from continuing.
  if (!file) {
    std::cerr << "Failed to create file.\n";
    return;
  }
  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(install_dir)) {
    if (entry.is_regular_file() || entry.is_symlink()) {
      auto relative_path = entry.path().lexically_relative(install_dir);
      file << "/" << relative_path.string() << '\n';
    }
  }
}

void install_package(const string &recipe_path) {
  Package pkg = load_recipe(recipe_path);

  std::cout << "Installing " << pkg.name << ".."<<'\n';
  std::cout << "Version: " << pkg.version << '\n';

  string archive_path = download_source(pkg.source, pkg.name, pkg.version);

  if (archive_path.empty()) {
    std::cerr << "Download failed.\n";
    return;
  }

  extract_source(archive_path);
  /* Main area where we do all of the installation steps such as building, installing,
   * copying to fake root.
   */
  string build_dir = ".cache/build/" + pkg.name + "-" + pkg.version;
  string install_dir = ".cache/pkg/" + pkg.name + "-" + pkg.version;
  std::filesystem::remove_all(install_dir);
  string manifest_path = ".cache/manifests/" + pkg.name + "-" + pkg.version;
  std::filesystem::create_directories(install_dir);
  std::cout << "Building " << pkg.name << " " << pkg.version << "...\n";
  run_build(recipe_path, build_dir);
  std::cout << "Build complete.\n";
  if (!run_install(recipe_path, build_dir, install_dir)) {
      std::cerr<<"Installed failed.\n";
      return;
  }
  create_manifest(install_dir, manifest_path);
  if (!install_staged_files(install_dir, ".cache/root")) {
      std::cerr<<"Failed to install package files.\n";
      return;
  }
  std::filesystem::create_directories(".cache/db/packages/");
  string package_db_dir = ".cache/db/packages/" + pkg.name;
  std::filesystem::create_directories(package_db_dir);

  std::ofstream file(package_db_dir + "/version");

  if (!file) {
      std::cerr<<"Failed to create package database entry.\n";
      return;
  }
  file << pkg.version<<'\n';
  std::error_code ec;
  std::filesystem::copy_file(
      manifest_path,
      package_db_dir + "/files",
      std::filesystem::copy_options::overwrite_existing,
      ec
  );
  if (ec) {
      std::cerr<<"Copy failed: "<<ec.message()<<'\n';
      return;
  }
}
// lowk hate ifstream, honestly i js hate c++
void remove_package(std::string package) {
  std::cout << "Removing " << package << "..." << '\n';
  string line;
  string package_db_dir = ".cache/db/packages/" + package;
  std::ifstream file(package_db_dir + "/files");
  // check to see if package is installed
  if (!file) {
      std::cerr<<"Package isn't installed!"<<'\n';
      return;
  }
  while (std::getline(file, line)) {
      std::filesystem::path target = ".cache/root" / std::filesystem::path(line.substr(1));
      std::filesystem::remove(target);
  }
  // fully remove pkg from db
  std::filesystem::remove_all(package_db_dir);
}

void search_package(const string &recipe_path, const string &package) {
  std::ifstream file(recipe_path);
  if (!file) {
    std::cerr << "No packages found for: " << package << ".\n";
    return;
  }
  std::cout << "Package found: " << package << '\n';
}
