#pragma once

#include <uuid_v4.h>

inline UUIDv4::UUIDGenerator<std::mt19937_64> &getUuidGenerator() {
  static UUIDv4::UUIDGenerator<std::mt19937_64> instance;
  return instance;
}
