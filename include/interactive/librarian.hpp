#pragma once

#include <fundamental/book.hpp>
#include <fundamental/library.hpp>
#include <interactive/filter.hpp>

#include <algorithm>
#include <array>
#include <span>

class Librarian {
public:
  static std::vector<Book> fetchBooks(std::span<const Filter<Book>> filters) {
    return Library::getInstance().getBookCollection()->fetchMatching(filters);
  }

  static std::vector<Book> fetchBooks(const Filter<Book> &filter) {
    return fetchBooks(std::array{filter});
  }

  static std::vector<User> fetchUsers(std::span<const Filter<User>> filters) {
    return Library::getInstance().getUserCollection()->fetchMatching(filters);
  }

  static std::vector<User> fetchUsers(const Filter<User> &filter) {
    return fetchUsers(std::array{filter});
  }

private:
    // static bool
};
