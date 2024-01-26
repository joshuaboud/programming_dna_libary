#pragma once

#include <util/book_collection.hpp>

class Library {
private:
  BookCollection mBookCollection;

public:
  auto fetchBooks(std::span<BookFilter> filters) {
    return mBookCollection.fetchBooks(filters);
  }

  bool checkOutBooks(std::span<Book> books) {
    // TODO
    return true;
  }

  bool checkInBooks(std::span<Book> books) {
    // TODO
    return true;
  }

};
