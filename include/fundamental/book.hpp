#pragma once

#include <string>

#include <nlohmann/json.hpp>

class Book {
private:
  std::string mId;
  std::string mTitle;
  std::string mAuthor;
  std::string mLocation;
  std::string mIsbn;
  std::string mGenre;

public:
  // Default Constructor
  constexpr Book()
      : mId(""),
        mTitle(""),
        mAuthor(""),
        mLocation(""),
        mIsbn(""),
        mGenre("") {}

  // Parameterized Constructor
  Book(
      const std::string &id, const std::string &title,
      const std::string &author, const std::string &location,
      const std::string &isbn, const std::string &genre
  )
      : mId(id),
        mTitle(title),
        mAuthor(author),
        mLocation(location),
        mIsbn(isbn),
        mGenre(genre) {}

  static Book fromJson(const nlohmann::json &j) {
    Book book;
    book.setId(j.at("id").get<std::string>());
    book.setTitle(j.at("title").get<std::string>());
    book.setAuthor(j.at("author").get<std::string>());
    book.setLocation(j.at("location").get<std::string>());
    book.setIsbn(j.at("isbn").get<std::string>());
    book.setGenre(j.at("genre").get<std::string>());
    return book;
  }

  const std::string &getId() const {
    return mId;
  }

  void setId(const std::string &id) {
    mId = id;
  }

  const std::string &getTitle() const {
    return mTitle;
  }

  void setTitle(const std::string &title) {
    mTitle = title;
  }

  const std::string &getAuthor() const {
    return mAuthor;
  }

  void setAuthor(const std::string &authors) {
    mAuthor = authors;
  }

  const std::string &getLocation() const {
    return mLocation;
  }

  void setLocation(const std::string &location) {
    mLocation = location;
  }

  const std::string &getIsbn() const {
    return mIsbn;
  }

  void setIsbn(const std::string &isbn) {
    mIsbn = isbn;
  }

  const std::string &getGenre() const {
    return mGenre;
  }

  void setGenre(const std::string &genre) {
    mGenre = genre;
  }

  bool operator==(const Book &other) const {
    return getId() == other.getId();
  }
};
