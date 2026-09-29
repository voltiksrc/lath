#pragma once

#include <string>

void run_install(const std::string &recipe_path, const std::string &install_dir,
                 const std::string &build_dir);
void run_build(const std::string &recipe_path, const std::string &build_dir);
