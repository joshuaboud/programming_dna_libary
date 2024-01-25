#pragma once

#include <fundamental/user.hpp>

#include <memory>

class UserSession {
private:
  std::shared_ptr<User> mUser;

  UserSession() : mUser{nullptr} {}

public:
  static UserSession &getInstance() {
    static UserSession instance; // guaranteed to only be initialized once
    return instance;
  }

  void setUser(const User &user) {
    mUser = std::make_shared<User>(user);
  }

  User getUser() const {
    return *mUser;
  }

  bool isLoggedIn() const {
    return mUser.get() != nullptr;
  }

  bool logIn(User &user, const std::string &password) {
    if (password != user.getPassword()) {
      return false;
    }
    setUser(user);
    return true;
  }

  void logOut() {
    mUser.reset();
  }
};
