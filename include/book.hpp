#pragma once

#include <string>

class Book {
private:
  std::string mId;
  std::string mTitle;
  std::string mAuthors;
  std::string mLocation;
  std::string mIsbn;
  std::string mGenre;

public:
  // Default Constructor
  Book()
      : mId(""),
        mTitle(""),
        mAuthors(""),
        mLocation(""),
        mIsbn(""),
        mGenre("") {}

  // Parameterized Constructor
  Book(const std::string &id, const std::string &title,
       const std::string &authors, const std::string &location,
       const std::string &isbn, const std::string &genre)
      : mId(id),
        mTitle(title),
        mAuthors(authors),
        mLocation(location),
        mIsbn(isbn),
        mGenre(genre) {}

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

  const std::string &getAuthors() const {
    return mAuthors;
  }

  void setAuthors(const std::string &authors) {
    mAuthors = authors;
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
};
