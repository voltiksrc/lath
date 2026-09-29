#include "download.hpp"
#include <curl/curl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using std::string;

// libcurl data writing function
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  size_t total = size * nmemb;

  auto *file = static_cast<std::ofstream *>(userdata);

  file->write(ptr, total);

  return total;
}

std::string download_source(const string &source_url, const string &name,
                            const string &version) {
  // initalize libcurl for downloads
  CURL *curl = curl_easy_init();

  auto pos = source_url.find_last_of('/');
  string filename = source_url.substr(pos + 1);

  auto query_pos = filename.find('?');
  if (query_pos != std::string::npos) {
    filename = filename.substr(0, query_pos);
  }

  string output_path = ".cache/" + name + "-" + version + ".tar.gz";

  if (curl == nullptr) {
    std::cerr << "Failed to initalize curl\n";
    return "";
  }
  // create .cache before opening it
  std::filesystem::create_directories(".cache");
  std::ofstream file(output_path, std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open output file\n";
    curl_easy_cleanup(curl);
    return "";
  }
  // set source_url for downloads
  curl_easy_setopt(curl, CURLOPT_URL, source_url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  CURLcode result = curl_easy_perform(curl);
  std::cout << '\n';
  if (result != CURLE_OK) {
    std::cerr << curl_easy_strerror(result) << '\n';
    curl_easy_cleanup(curl);
    return "";
  }
  file.close();
  curl_easy_cleanup(curl);
  return output_path;
}
