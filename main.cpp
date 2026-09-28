#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <lua5.3/lua.hpp>
#include <string>
#include <vector>

using std::string;
using std::vector;

struct Package {
  string name;
  string version;
  string source;
};
Package load_recipe(const string &path) {
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
  }
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

// libcurl data writing function
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  size_t total = size * nmemb;

  auto *file = static_cast<std::ofstream *>(userdata);

  file->write(ptr, total);

  return total;
}

void download_source(const string &source_url) {
  CURL *curl = curl_easy_init();
  auto pos = source_url.find_last_of('/');
  string filename = source_url.substr(pos + 1);

  string output_path = ".cache/" + filename;
  auto query_pos = filename.find('?');
  if (query_pos != std::string::npos) {
    filename = filename.substr(0, query_pos);
  }

  if (curl == nullptr) {
    std::cerr << "Failed to initalize curl\n";
    return;
  }

  std::ofstream file(output_path, std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open output file\n";
    curl_easy_cleanup(curl);
    return;
  }

  curl_easy_setopt(curl, CURLOPT_URL, source_url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  CURLcode result = curl_easy_perform(curl);
  std::cout << '\n';
  if (result != CURLE_OK) {
    std::cerr << curl_easy_strerror(result) << '\n';
  }

  curl_easy_cleanup(curl);
}

void lath_version() {
  double version{0.1};
  std::cout << "lath " << version << '\n';
}
// for noobies(when will printf become normalized^^)
void show_help() {
  std::cout << "lath features/help\n";
  std::cout << '\n';
  std::cout << "lath get <package>\n";
  std::cout << "lath rm <package>\n";
  std::cout << "lath search <package>\n";
  std::cout << "lath --help\n";
}
// FUCK vectors
void search_package(std::vector<string> packages, std::string package) {
  bool found = false;
  for (string pkg : packages) {
    if (pkg == package) {
      found = true;
      break;
    }
  }
  if (found == true) {
    std::cout << "Package found: " << package << '\n';
  } else {
    std::cout << "Package not found.\n";
  }
}
// main installation function
void install_package(const string &recipe_path) {
  Package pkg = load_recipe(recipe_path);

  std::cout << "Installing " << pkg.name << '\n';
  std::cout << "Version: " << pkg.version << '\n';
  download_source(pkg.source);
}
void remove_package(std::string package) {
  std::cout << "Removing " << package << "..." << '\n';
}
int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Please enter a command and 'optionally' a package.\n";
    return 1;
  }

  string command = argv[1];
  // command line interaction
  if (command == "--help") {
    show_help();
    return 0;
  } else if (command == "--version") {
    lath_version();
    return 0;
  }
  if (argc < 3) {
    std::cout << "usage: lath <command> [package]\n";
    return 1;
  }
  string package = argv[2];
  string recipe_path = "recipes/" + package + "/package.lua";
  // command checks for validation
  if (command == "get") {
    install_package(recipe_path);
  } else if (command == "search") {
    // search_package(package);
  } else if (command == "rm") {
    remove_package(package);
  } else {
    std::cout << "Unknown command: " << argv[1] << '\n';
    return 1;
  }
}
