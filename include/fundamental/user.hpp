#pragma once

#include <string>
#include <util/password.hpp>

class User {
private:
  std::string mId;
  std::string mFirstName;
  std::string mLastName;
  std::string mEmail;
  Password mPassword;

public:
  // Default Constructor
  User()
      : mId(""),
        mFirstName(""),
        mLastName(""),
        mEmail(""),
        mPassword(Password::fromHash("")) {}

  // Parameterized Constructor
  User(
      const std::string &id, const std::string &firstName,
      const std::string &lastName, const std::string &email,
      const Password &password
  )
      : mId(id),
        mFirstName(firstName),
        mLastName(lastName),
        mEmail(email),
        mPassword(password) {}

  static User fromJson(const nlohmann::json &j) {
    User user;
    user.setId(j["id"]);
    user.setFirstName(j["firstName"]);
    user.setLastName(j["lastName"]);
    user.setEmail(j["email"]);
    if (j.contains("passwordHash"))
      user.setPassword(Password::fromHash(j["passwordHash"]));
    else
      user.setPassword(Password::fromPlainText(j["password"]));
    return user;
  }

  const std::string &getId() const {
    return mId;
  }

  void setId(const std::string &id) {
    mId = id;
  }

  const std::string &getFirstName() const {
    return mFirstName;
  }

  void setFirstName(const std::string &firstName) {
    mFirstName = firstName;
  }

  const std::string &getLastName() const {
    return mLastName;
  }

  void setLastName(const std::string &lastName) {
    mLastName = lastName;
  }

  const std::string &getEmail() const {
    return mEmail;
  }

  void setEmail(const std::string &email) {
    mEmail = email;
  }

  const Password &getPassword() const {
    // lol pwn'd
    return mPassword;
  }

  void setPassword(const Password &password) {
    mPassword = password;
  }

  bool operator==(const User &other) const {
    return getId() == other.getId();
  }
};
