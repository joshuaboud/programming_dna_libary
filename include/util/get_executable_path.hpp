#pragma once

#include <filesystem>

std::filesystem::path getExecuatblePath() {
  return std::filesystem::canonical("/proc/self/exe");
}
