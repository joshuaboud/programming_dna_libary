#pragma once

#include <fundamental/book.hpp>
#include <fundamental/collection.hpp>
#include <fundamental/user.hpp>
#include <management/book_log.hpp>

#include <util/get_executable_path.hpp>

class Library {
private:
  Collection<Book> mBookCollection;
  Collection<User> mUserCollection;
  BookLog mBookLog;

  Library()
      : mBookCollection(JsonCollection<Book>(
            getExecuatblePath().parent_path() / "resources/book_collection.json"
        )),
        mUserCollection(JsonCollection<User>(
            getExecuatblePath().parent_path() / "resources/user_collection.json"
        )) {
    mBookCollection->initialize();
  }

public:
  static Library &getInstance() {
    static Library instance;
    return instance;
  }

  Collection<Book> &getBookCollection() {
    return mBookCollection;
  }

  Collection<User> &getUserCollection() {
    return mUserCollection;
  }

  BookLog &getBookLog() {
    return mBookLog;
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
