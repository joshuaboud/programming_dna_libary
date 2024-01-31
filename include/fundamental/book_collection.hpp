#pragma once

#include <fundamental/book.hpp>
#include <interactive/book_filter.hpp>

#include <util/get_executable_path.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <cassert>
#include <filesystem>

#include <nlohmann/json.hpp>

class BookCollectionBase {
protected:
  BookCollectionBase() = default;

public:
  virtual void initialize() = 0;
  virtual std::vector<Book> getBooks() = 0;
  virtual bool addNewBook(const Book &book) = 0;
  virtual bool removeBook(const Book &book) = 0;
  virtual bool editBook(const Book &book) = 0;
  virtual bool hasBook(const Book &book) = 0;
};

using BookCollection = std::shared_ptr<BookCollectionBase>;

class JsonBookCollectionBase : public BookCollectionBase {

public:
  JsonBookCollectionBase(const std::filesystem::path &jsonFilePath)
      : mBooks(),
        mJsonFilePath(jsonFilePath) {}

  void initialize() override {
    std::ifstream file(mJsonFilePath);

    assert(file.is_open());

    nlohmann::json jsonBooks;

    file >> jsonBooks;

    file.close();

    mBooks.clear();

    for (const auto &jsonBook : jsonBooks) {
      mBooks.emplace_back(bookFromJson(jsonBook));
    }
  }

  std::vector<Book> getBooks() override {
    return mBooks;
    // if (filters.empty())
    //   return mBooks;
    // std::vector<Book> matchingBooks;

    // auto predicate = [&filters](const Book &book) {
    //   for (BookFilter &filter : filters) {
    //     std::string filterValue = filter.getValue();
    //     std::string bookValue;
    //     switch (filter.getKey()) {
    //     case BookFilter::Key::AUTHOR:
    //       bookValue = book.getAuthor();
    //       break;
    //     case BookFilter::Key::GENRE:
    //       bookValue = book.getGenre();
    //       break;
    //     case BookFilter::Key::ISBN:
    //       bookValue = book.getIsbn();
    //       break;
    //     case BookFilter::Key::TITLE:
    //       bookValue = book.getTitle();
    //       break;
    //     default:
    //       throw std::logic_error("Unhandled BookFilter::Key in switch
    //       statement"
    //       );
    //     }
    //     // to uppercase before testing
    //     std::transform(
    //         filterValue.begin(), filterValue.end(), filterValue.begin(),
    //         [](unsigned char c) { return std::toupper(c); }
    //     );
    //     std::transform(
    //         bookValue.begin(), bookValue.end(), bookValue.begin(),
    //         [](unsigned char c) { return std::toupper(c); }
    //     );
    //     // check for substring match
    //     if (bookValue.find(filterValue) == std::string::npos) {
    //       return false;
    //     } // else continue
    //   }
    //   return true;
    // };

    // std::copy_if(
    //     mBooks.begin(), mBooks.end(), std::back_inserter(matchingBooks),
    //     predicate
    // );

    // return matchingBooks;
  }

  bool addNewBook(const Book &book) {
    if (hasBook(book))
      return false;
    mBooks.emplace_back(book);
    return true;
  }

  bool removeBook(const Book &book) {
    if (!hasBook(book))
      return false;
    mBooks.erase(
        std::remove_if(
            mBooks.begin(), mBooks.end(),
            [&](const Book &b) { return b.getId() == book.getId(); }
        ),
        mBooks.end()
    );
    return true;
  }

  bool editBook(const Book &book) {
    if (!removeBook(book))
      return false;
    return addNewBook(book);
  }

  bool hasBook(const Book &book) {
    return std::find_if(mBooks.begin(), mBooks.end(), [&, this](const Book &b) {
             return b.getId() == book.getId();
           }) != mBooks.end();
  }

private:
  std::vector<Book> mBooks;

  std::filesystem::path mJsonFilePath;

  static Book bookFromJson(const nlohmann::json &j) {
    Book book;
    book.setId(j.at("id").get<std::string>());
    book.setTitle(j.at("title").get<std::string>());
    book.setAuthor(j.at("author").get<std::string>());
    book.setLocation(j.at("location").get<std::string>());
    book.setIsbn(j.at("isbn").get<std::string>());
    book.setGenre(j.at("genre").get<std::string>());
    return book;
  }
};

BookCollection JsonBookCollection(const std::filesystem::path &jsonFilePath) {
  return std::make_shared<JsonBookCollectionBase>(jsonFilePath);
}

BookCollection JsonBookCollection() {
  return JsonBookCollection(getExecuatblePath().parent_path() / "resources/book_collection.json");
}
