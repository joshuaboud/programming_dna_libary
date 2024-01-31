#pragma once

#include <util/uuid.hpp>

#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>

class EventSubscriber {
public:
  using CallbackFunction = std::function<void(const std::string &)>;

  EventSubscriber(std::string event, CallbackFunction callback)
      : mEvent(event),
        mCallback(callback),
        mId(getUuidGenerator().getUUID()) {}

  const std::string &getEvent() const {
    return mEvent;
  }

  void notify(const std::string &message) const {
    mCallback(message);
  }

  bool operator==(const EventSubscriber &other) const {
    return mId == other.mId;
  }

private:
  std::string mEvent;
  CallbackFunction mCallback;
  UUIDv4::UUID mId;
};

class EventPublisher {
public:
  static EventPublisher &getInstance() {
    static EventPublisher instance;
    return instance;
  }

  const EventSubscriber &subscribe(
      const std::string &event,
      const EventSubscriber::CallbackFunction &callback
  ) {
    mSubscriptions[event].emplace_back(event, callback);
    return mSubscriptions[event].back();
  }

  void unsubscribe(const EventSubscriber &subscriber) {
    auto &subscribersList = mSubscriptions[subscriber.getEvent()];
    auto toErase =
        std::remove(subscribersList.begin(), subscribersList.end(), subscriber);
    subscribersList.erase(toErase, subscribersList.end());
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

  std::unordered_map<std::string, std::vector<EventSubscriber>> mSubscriptions;
};

class SubscribesToEvents {
public:
  SubscribesToEvents(const SubscribesToEvents &other) = delete;

protected:
  SubscribesToEvents() = default;
  ~SubscribesToEvents() {
    for (auto &handle : mSubscriptionHandles) {
      EventPublisher::getInstance().unsubscribe(handle);
    }
  }

  template <typename... Args> void subscribe(Args... args) {
    mSubscriptionHandles.emplace_back(
        EventPublisher::getInstance().subscribe(std::forward<Args>(args)...)
    );
  }

private:
  std::vector<EventSubscriber> mSubscriptionHandles;
};
