#pragma once

#include <functional>
#include <string>
#include <unordered_map>

class Subscriber {
public:
  using CallbackFunction = std::function<void(const std::string &)>;

  Subscriber(CallbackFunction callback) : callback(callback) {}

  void notify(const std::string &message) const {
    callback(message);
  }

private:
  CallbackFunction callback;
};

class EventPublisher {
public:
  static EventPublisher &getInstance() {
    static EventPublisher instance;
    return instance;
  }

  void subscribe(
      const std::string &event, const Subscriber::CallbackFunction &callback
  ) {
    mSubscriptions[event].emplace_back(callback);
  }

  void publish(const std::string &event, const std::string &message) {
    auto it = mSubscriptions.find(event);
    if (it != mSubscriptions.end()) {
      for (const auto &subscriber : it->second) {
        subscriber.notify(message);
      }
    }
  }

private:
  EventPublisher() = default;

  std::unordered_map<std::string, std::vector<Subscriber>> mSubscriptions;
};
