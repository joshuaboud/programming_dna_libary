#pragma once

#include <fundamental/book.hpp>

#include <algorithm>
#include <span>
#include <vector>

class Cart {
public:
  static Cart &getInstance() {
    static Cart instance;
    return instance;
  }

  std::span<Book> getBooks() {
    return mBooks;
  }

  void setBooks(std::span<Book> books) {
    mBooks.assign(books.begin(), books.end());
  }

  void addBook(const Book &book) {
    if (hasBook(book)) {
      return; // alreadt in cart
    }
    mBooks.emplace_back(book);
  }

  void removeBook(const Book &toRemove) {
    auto toErase = std::remove(mBooks.begin(), mBooks.end(), toRemove);
    mBooks.erase(toErase, mBooks.end());
  }

  bool hasBook(const Book &book) const {
    return std::find(mBooks.begin(), mBooks.end(), book) != mBooks.end();
  }

private:
  std::vector<Book> mBooks;

  Cart() = default;
};
