#pragma once

#include <fundamental/book.hpp>
#include <fundamental/collection.hpp>
#include <fundamental/user.hpp>
#include <management/book_log.hpp>
#include <interactive/user_session.hpp>

#include <util/get_executable_path.hpp>
#include <util/uuid.hpp>

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
    if (auto user = UserSession::getInstance().getUser()) {
      for (const auto &book : books) {
        BookCopy logEntry;
        logEntry.setId(getUuidGenerator().getUUID().str());
        logEntry.setBookId(book.getId());
        logEntry.setUserId(user->getId());
        logEntry.setCheckOutDate(std::chrono::system_clock::now());
        mBookLog.addEntry(logEntry);
      }
      return true;
    }
    return false;
  }

  bool checkInBooks(std::span<Book> books) {
    if (auto user = UserSession::getInstance().getUser()) {
      for (const auto &book : books) {
        auto entries = mBookLog.getEntriesFor(book, *user);
        for (auto &entry : entries) {
          entry.setCheckInDate(std::chrono::system_clock::now());
          mBookLog.removeEntry(entry);
          mBookLog.addEntry(entry);
        }
      }
      return true;
    }
    return false;
  }
};
