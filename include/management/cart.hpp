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
    if (std::find_if(mBooks.begin(), mBooks.end(), [&book](const Book &b) {
          return b.getId() == book.getId();
        }) != mBooks.end()) {
      return; // alreadt in cart
    }
    mBooks.emplace_back(book);
  }

  void removeBook(const Book &toRemove) {
    mBooks.erase(
        std::remove_if(
            mBooks.begin(), mBooks.end(),
            [&](const Book &book) { return book.getId() == toRemove.getId(); }
        ),
        mBooks.end()
    );
  }

private:
  std::vector<Book> mBooks;

  Cart() = default;
};
