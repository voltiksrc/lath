#include "recipe.hpp"
#include <iostream>
#include <lua5.3/lua.hpp>
#include <string>

Package load_recipe(const std::string &path) {
  lua_State *L = luaL_newstate();

  if (!L) {
    std::cerr << "Failed to create Lua state\n";
    return {};
  }

  luaL_openlibs(L);

  int result = luaL_dofile(L, path.c_str());

  if (result != LUA_OK) {
    std::cerr << "Lua error: " << lua_tostring(L, -1) << '\n';
    lua_close(L);
    return {};
  } // embed lua recipes
  lua_getglobal(L, "pkg");
  lua_getfield(L, -1, "name");
  Package package;
  package.name = lua_tostring(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, -1, "version");
  package.version = lua_tostring(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, -1, "source");
  package.source = lua_tostring(L, -1);
  lua_pop(L, 1);
  lua_close(L);
  return package;
}
