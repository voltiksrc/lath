#pragma once

#include <string>

using std::string;

bool install_staged_files(const string &install_dir, const string &root_dir);
void install_package(const std::string &recipe_path);
void remove_package(std::string package);
void search_package(const std::string &recipe_path, const std::string &package);
