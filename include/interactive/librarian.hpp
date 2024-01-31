#pragma once

#include <fundamental/book.hpp>
#include <fundamental/library.hpp>
#include <interactive/book_filter.hpp>

#include <algorithm>
#include <array>
#include <span>

class Librarian {
public:
  static std::vector<Book> fetchBooks(std::span<const BookFilter> filters) {
    auto allBooks = Library::getInstance().getBookCollection()->getBooks();

    if (filters.empty())
      return allBooks;

    std::vector<Book> matching;

    auto matchesAllFilters = [&filters, &allBooks](const Book &book) -> bool {
      for (const auto &filter : filters) {
        if (!filter.passes(book))
          return false;
      }
      return true;
    };

    std::copy_if(
        allBooks.begin(), allBooks.end(), std::back_inserter(matching),
        matchesAllFilters
    );

    return matching;
  }

  static std::vector<Book> fetchBooks(BookFilter filter) {
    return fetchBooks(std::array{filter});
  }
};
