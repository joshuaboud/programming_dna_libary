#pragma once

#include <fundamental/book.hpp>
#include <interactive/book_filter.hpp>

#include <algorithm>
#include <array>
#include <span>
#include <tuple>
#include <vector>

class IBookCollection {
protected:
  IBookCollection() = default;

public:
  virtual std::vector<Book> fetchBooks(std::span<BookFilter> filters) = 0;
};

class ListBookCollection : public IBookCollection {
private:
  std::vector<Book> mBooks;

  inline static std::array sDefaultBooks = {
      Book("0000", "1984", "George Orwell", "Shelf 1", "9780452284234",
           "Fiction"),
      Book("0001", "Harry Potter and the Philosopher's Stone", "J.K. Rowling",
           "Shelf 2", "0747532745", "Fiction"),
      Book("0002", "Hitchhiker's Guide to the Galaxy, The", "Douglas Adams",
           "Shelf 42", "0345391802", "Fiction"),
  };

public:
  ListBookCollection(std::span<Book> books)
      : mBooks(books.begin(), books.end()) {}
  ListBookCollection() : ListBookCollection(sDefaultBooks) {}

  std::vector<Book> fetchBooks(std::span<BookFilter> filters) override {
    if (filters.empty())
      return mBooks;
    std::vector<Book> matchingBooks;

    auto predicate = [&filters](const Book &book) {
      for (BookFilter &filter : filters) {
        std::string filterValue = filter.getValue();
        std::string bookValue;
        switch (filter.getKey()) {
        case BookFilter::Key::AUTHOR:
          bookValue = book.getAuthor();
          break;
        case BookFilter::Key::GENRE:
          bookValue = book.getGenre();
          break;
        case BookFilter::Key::ISBN:
          bookValue = book.getIsbn();
          break;
        case BookFilter::Key::TITLE:
          bookValue = book.getTitle();
          break;
        default:
          throw std::logic_error(
              "Unhandled BookFilter::Key in switch statement");
        }
        // to uppercase before testing
        std::transform(filterValue.begin(), filterValue.end(),
                       filterValue.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        std::transform(bookValue.begin(), bookValue.end(), bookValue.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        if (bookValue.find(filterValue) == std::string::npos) {
          return false;
        } // else continue
      }
      return true;
    };

    std::copy_if(mBooks.begin(), mBooks.end(),
                 std::back_inserter(matchingBooks), predicate);

    return matchingBooks;
  }
};

using BookCollection = ListBookCollection;
