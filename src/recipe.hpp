#pragma once

#include <string>

struct Package {
  std::string name;
  std::string version;
  std::string source;
};

Package load_recipe(const std::string &path);
