#pragma once

#include "book.hpp"
#include "user.hpp"

#include <algorithm>
#include <span>
#include <vector>

class BookCollection {
private:
  std::vector<Book> mBooks;

public:
  BookCollection() : mBooks() {}
  BookCollection(std::span<Book> books) : mBooks(books.begin(), books.end()) {}

  bool checkOutBooks(std::span<Book> books) {
    // TODO
  }

  bool checkInBooks(std::span<Book> books) {

  }
};
