#pragma once

#include <util/book_collection.hpp>
#include <management/book_log.hpp>

class Library {
private:
  BookCollection mBookCollection;
  BookLog mBookLog;

  Library() = default;

public:
  static Library &getInstance() {
    static Library instance;
    return instance;
  }

  auto fetchBooks(std::span<BookFilter> filters) {
    return mBookCollection.fetchBooks(filters);
  }

  auto addNewBook(const Book &book) {
    return mBookCollection.addNewBook(book);
  }

  auto removeBook(const Book &book) {
    return mBookCollection.removeBook(book);
  }

  auto editBook(const Book &book) {
    return mBookCollection.editBook(book);
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
