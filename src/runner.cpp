#include "runner.hpp"
#include <filesystem>
#include <iostream>
#include <lua5.3/lua.hpp>
#include <string>

using std::string;

bool run_install(const std::string &recipe_path, const std::string &build_dir,
                 const std::string &install_dir) {
  auto original = std::filesystem::current_path();
  auto absolute_recipe = std::filesystem::absolute(recipe_path);
  auto absolute_install_dir = std::filesystem::absolute(install_dir);
  std::filesystem::current_path(build_dir);
  lua_State *L = luaL_newstate();
  if (!L) {
    std::cerr << "Failed to create lua state\n";
    std::filesystem::current_path(original);
    return false;
  }
  luaL_openlibs(L);
  int result = luaL_dofile(L, absolute_recipe.c_str());
  if (result != LUA_OK) {
    std::cerr << "Lua error: " << lua_tostring(L, -1) << '\n';
    std::filesystem::current_path(original);
    lua_close(L);
    return false;
  }
  lua_getglobal(L, "pkg");
  lua_getfield(L, -1, "install");
  lua_pushstring(L, absolute_install_dir.c_str());
  int cresult = lua_pcall(L, 1, 0, 0);
  if (cresult != LUA_OK) {
    std::cerr << "Lua error: " << lua_tostring(L, -1) << '\n';
    std::filesystem::current_path(original);
    lua_close(L);
    return false;
  }
  lua_close(L);
  std::filesystem::current_path(original);
  return true;
}

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
  int lresult = lua_pcall(L, 0, 0, 0);
  if (lresult != LUA_OK) {
    std::cerr << "Lua error: " << lua_tostring(L, -1) << '\n';
    lua_close(L);
    std::filesystem::current_path(original);
    return;
  }

  lua_close(L);
  std::filesystem::current_path(original);
}
