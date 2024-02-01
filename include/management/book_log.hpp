#pragma once

#include <fundamental/book.hpp>
#include <fundamental/user.hpp>
#include <interactive/book_copy.hpp>

#include <algorithm>
#include <optional>
#include <vector>

class BookLog {
public:
  BookLog() = default;

  const std::vector<BookCopy> &getEntries() const {
    return mBookCopies;
  }

  void addEntry(const BookCopy &bookCopy) {
    mBookCopies.emplace_back(bookCopy);
  }

  void removeEntry(const BookCopy &bookCopy) {
    mBookCopies.erase(
        std::remove(mBookCopies.begin(), mBookCopies.end(), bookCopy)
    );
  }

  std::vector<BookCopy> getEntriesFor(const User &user) const {
    auto predicate = [&user](const BookCopy &entry) {
      return user.getId() == entry.getUserId();
    };
    std::vector<BookCopy> matching;
    std::copy_if(
        mBookCopies.begin(), mBookCopies.end(), std::back_inserter(matching),
        predicate
    );
    return matching;
  }

  std::vector<BookCopy> getEntriesFor(const Book &book) const {
    auto predicate = [&book](const BookCopy &entry) {
      return book.getId() == entry.getBookId();
    };
    std::vector<BookCopy> matching;
    std::copy_if(
        mBookCopies.begin(), mBookCopies.end(), std::back_inserter(matching),
        predicate
    );
    return matching;
  }

  std::vector<BookCopy> getEntriesFor(const Book &book, const User &user) const {
    auto predicate = [&book, &user](const BookCopy &entry) {
      return book.getId() == entry.getBookId() && user.getId() == entry.getUserId();
    };
    std::vector<BookCopy> matching;
    std::copy_if(
        mBookCopies.begin(), mBookCopies.end(), std::back_inserter(matching),
        predicate
    );
    return matching;
  }

private:
  std::vector<BookCopy> mBookCopies;
};
