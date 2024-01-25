#pragma once

#include <string>
#include <chrono>

class BookCopy {
private:
  std::string mId;
  std::string mBookId;
  std::string mUserId;
  std::chrono::system_clock::time_point mCheckInDate;
  std::chrono::system_clock::time_point mCheckOutDate;

public:
  // Default Constructor
  BookCopy()
      : mId(""),
        mBookId(""),
        mUserId(""),
        mCheckInDate(),
        mCheckOutDate() {}

  // Parameterized Constructor
  BookCopy(const std::string &id, const std::string &bookId,
           const std::string &userId, const std::chrono::system_clock::time_point &checkInDate,
           const std::chrono::system_clock::time_point &checkOutDate)
      : mId(id),
        mBookId(bookId),
        mUserId(userId),
        mCheckInDate(checkInDate),
        mCheckOutDate(checkOutDate) {}

  const std::string &getId() const {
    return mId;
  }

  void setId(const std::string &id) {
    mId = id;
  }

  const std::string &getBookId() const {
    return mBookId;
  }

  void setBookId(const std::string &bookId) {
    mBookId = bookId;
  }

  const std::string &getUserId() const {
    return mUserId;
  }

  void setUserId(const std::string &userId) {
    mUserId = userId;
  }

  const std::chrono::system_clock::time_point &getCheckInDate() const {
    return mCheckInDate;
  }

  void setCheckInDate(const std::chrono::system_clock::time_point &checkInDate) {
    mCheckInDate = checkInDate;
  }

  const std::chrono::system_clock::time_point &getCheckOutDate() const {
    return mCheckOutDate;
  }

  void setCheckOutDate(const std::chrono::system_clock::time_point &checkOutDate) {
    mCheckOutDate = checkOutDate;
  }
};
