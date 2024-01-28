#pragma once

#include <util/book_collection.hpp>

class Library {
private:
  BookCollection mBookCollection;

  Library() = default;

public:
  static Library &getInstance() {
    static Library instance;
    return instance;
  }

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
