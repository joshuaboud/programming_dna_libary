#pragma once

#include <string>


// TODO
class BookFilter {
public:
  enum class Key {TITLE, AUTHOR, ISBN, GENRE};

private:
  Key mKey;
  std::string mValue;

public:
  BookFilter() {}
  BookFilter(Key key, std::string value) : mKey{key}, mValue{value} {}

  Key getKey() const {
    return mKey;
  }

  void setKey(Key key) {
    mKey = key;
  }

  const std::string &getValue() const {
    return mValue;
  }

  void setValue(const std::string &value) {
    mValue = value;
  }
};
