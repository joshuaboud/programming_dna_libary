#pragma once

#include <algorithm>
#include <string>

namespace StringUtils {

constexpr std::string lowercase(std::string str) {
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
  return str;
}

constexpr bool containsSubstring(std::string str, std::string substr) {
  return str.find(substr) != std::string::npos;
}

} // namespace StringUtils
