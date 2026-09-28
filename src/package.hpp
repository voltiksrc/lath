#pragma once

#include <string>

void install_package(const std::string &recipe_path);
void remove_package(std::string package);
void search_package(const std::string &recipe_path, const std::string &package);
