#include "utility/utility.h"

#include <iostream>
#include <sstream>

bool Utility::String::split(const std::string& str, std::vector<std::string>& split_str, const char& delim) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(str);
  while (std::getline(tokenStream, token, delim)) {
    split_str.push_back(token);
  }
  return true;
}