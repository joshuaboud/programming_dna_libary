#pragma once

#include <util/string_utils.hpp>

#include <functional>
#include <string>


class BookFilter {
public:
  using PropertyProvider = const std::string &(Book::*)() const;

  BookFilter() = default;
  BookFilter(
      const std::string &searchText, PropertyProvider propertyProvider
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
  PropertyProvider mPropertyProvider;
};
