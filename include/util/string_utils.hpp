#pragma once

#include <algorithm>
#include <string>
#include <functional>

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

static inline std::string
stripMatching(std::string str, std::function<bool(unsigned char)> predicate) {
  str.erase(std::remove_if(str.begin(), str.end(), predicate), str.end());
  return str;
}

static inline std::string stripNonAlphaNumeric(std::string str) {
  return stripMatching(str, [](unsigned char c) { return !std::isalnum(c); });
}

} // namespace StringUtils
