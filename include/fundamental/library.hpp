#pragma once

#include <fundamental/book_collection.hpp>
#include <management/book_log.hpp>

class Library {
private:
  BookCollection mBookCollection;
  BookLog mBookLog;

  Library() : mBookCollection(JsonBookCollection()) {
    mBookCollection->initialize();
  }

public:
  static Library &getInstance() {
    static Library instance;
    return instance;
  }

  BookCollection &getBookCollection() {
    return mBookCollection;
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
