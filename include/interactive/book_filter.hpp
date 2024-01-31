#pragma once

#include <util/string_utils.hpp>

#include <functional>
#include <string>

using BookPropertyProvider = const std::string &(Book::*)() const;

// TODO
class BookFilter {
public:
  BookFilter() = default;
  BookFilter(
      const std::string &searchText, BookPropertyProvider propertyProvider
  )
      : mSearchText(searchText),
        mPropertyProvider(propertyProvider) {}

  bool passes(const Book &book) const {
    if (mPropertyProvider == nullptr)
      return false;
    return StringUtils::containsSubstring(
        StringUtils::lowercase((book.*mPropertyProvider)()),
        StringUtils::lowercase(mSearchText)
    );
  }

private:
  std::string mSearchText;
  BookPropertyProvider mPropertyProvider;
};
