#pragma once

#include <util/string_utils.hpp>

#include <functional>
#include <string>
#include <vector>

class BookFilter {
public:
  using PropertyProvider = const std::string &(Book::*)() const;

  BookFilter() = default;
  BookFilter(const std::string &searchText, PropertyProvider propertyProvider)
      : mSearchText(searchText),
        mPropertyProvider(propertyProvider) {}

  bool passes(const Book &book) const {
    if (mPropertyProvider == nullptr)
      return false;
    return StringUtils::containsSubstring(
        normalizeString((book.*mPropertyProvider)()),
        normalizeString(mSearchText)
    );
  }

private:
  std::string mSearchText;
  PropertyProvider mPropertyProvider;

  static std::string normalizeString(std::string str) {
    return StringUtils::lowercase(StringUtils::stripNonAlphaNumeric(str));
  }
};

class BookFiltersBuilder {
public:
  BookFiltersBuilder() : mFilters() {}

  template <typename... Args> BookFiltersBuilder &addFilter(Args... args) {
    mFilters.emplace_back(std::forward<Args>(args)...);
    return *this;
  }

  BookFiltersBuilder &addFilterIfStringNotEmpty(
      const std::string &searchText,
      BookFilter::PropertyProvider propertyProvider
  ) {
    if (!searchText.empty())
      return addFilter(searchText, propertyProvider);
    return *this;
  }

  std::vector<BookFilter> get() {
    return std::move(mFilters);
  }

private:
  std::vector<BookFilter> mFilters;
};
