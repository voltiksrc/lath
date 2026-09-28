#include "build.hpp"
#include <filesystem>
#include <iostream>
#include <lua5.3/lua.hpp>
#include <string>

using std::string;

void run_build(const std::string &recipe_path, const std::string &build_dir) {
  auto original = std::filesystem::current_path();
  auto absolute_recipe = std::filesystem::absolute(recipe_path);
  std::filesystem::current_path(build_dir);
  lua_State *L = luaL_newstate();
  if (!L) {
    std::cerr << "Failed to create lua state\n";
    std::filesystem::current_path(original);
    return;
  }

  luaL_openlibs(L);
  int result = luaL_dofile(L, absolute_recipe.c_str());
  if (result != LUA_OK) {
    std::cerr << "Lua error: " << lua_tostring(L, -1) << '\n';
    lua_close(L);
    std::filesystem::current_path(original);
    return;
  }
  lua_getglobal(L, "pkg");
  lua_getfield(L, -1, "build");
  lua_pcall(L, 0, 0, 0);
  if (result != LUA_OK) {
    std::cerr << "Lua error: " << lua_tostring(L, -1) << '\n';
    lua_close(L);
    std::filesystem::current_path(original);
    return;
  }

  lua_close(L);
  std::filesystem::current_path(original);
}
