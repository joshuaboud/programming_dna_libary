#pragma once

#include <fundamental/user.hpp>
#include <util/password.hpp>

#include <memory>
#include <optional>

class UserSession {
private:
  std::optional<User> mUser;

  UserSession() : mUser(std::nullopt) {}

public:
  static UserSession &getInstance() {
    static UserSession instance; // guaranteed to only be initialized once
    return instance;
  }

  void setUser(const User &user) {
    mUser = user;
  }

  std::optional<User> getUser() const {
    return mUser;
  }

  bool isLoggedIn() const {
    return mUser.has_value();
  }

  bool logIn(User &user, const std::string &password) {
    if (Password::fromPlainText(password) != user.getPassword()) {
      return false;
    }
    setUser(user);
    return true;
  }

  void logOut() {
    mUser.reset();
  }
};
