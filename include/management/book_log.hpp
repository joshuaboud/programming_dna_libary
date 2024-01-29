#pragma once

#include <interactive/book_copy.hpp>
#include <fundamental/book.hpp>
#include <fundamental/user.hpp>

#include <vector>
#include <optional>

class BookLog {
public:
  BookLog() = default;

  bool addEntry(const Book &book, const User &user) {
    return false;
  }

private:
  std::vector<BookCopy> mBookCopies;
};
