#pragma once

#include <util/string_utils.hpp>

#include <functional>
#include <string>
#include <vector>

template <typename T> class Filter {
public:
  using PropertyProvider = const std::string &(T::*)() const;

  Filter() = default;
  Filter(const std::string &searchText, PropertyProvider propertyProvider)
      : mSearchText(searchText),
        mPropertyProvider(propertyProvider) {}

  bool passes(const T &obj) const {
    if (mPropertyProvider == nullptr)
      return false;
    return StringUtils::containsSubstring(
        normalizeString((obj.*mPropertyProvider)()),
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


template<typename T>
class FiltersBuilder {
public:
  FiltersBuilder() : mFilters() {}

  template <typename... Args> FiltersBuilder<T> &addFilter(Args... args) {
    mFilters.emplace_back(std::forward<Args>(args)...);
    return *this;
  }

  FiltersBuilder<T> &addFilterIfStringNotEmpty(
      const std::string &searchText,
      Filter<T>::PropertyProvider propertyProvider
  ) {
    if (!searchText.empty())
      return addFilter(searchText, propertyProvider);
    return *this;
  }

  std::vector<Filter<T>> get() {
    return std::move(mFilters);
  }

private:
  std::vector<Filter<T>> mFilters;
};
