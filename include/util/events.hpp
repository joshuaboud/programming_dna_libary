#pragma once

#include <util/uuid.hpp>

#include <algorithm>
#include <forward_list>
#include <functional>
#include <string>
#include <unordered_map>

class EventSubscription {
public:
  using CallbackFunction = std::function<void(const std::string &)>;

  EventSubscription(std::string event, CallbackFunction callback)
      : mEvent(event),
        mCallback(callback),
        mId(getUuidGenerator().getUUID()) {}

  const std::string &getEvent() const {
    return mEvent;
  }

  void notify(const std::string &message) const {
    mCallback(message);
  }

  bool operator==(const EventSubscription &other) const {
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

  const EventSubscription &subscribe(
      const std::string &event,
      const EventSubscription::CallbackFunction &callback
  ) {
    mSubscriptions[event].emplace_front(event, callback);
    return mSubscriptions[event].front();
  }

  void unsubscribe(const EventSubscription &subscriber) {
    auto &subscribersList = mSubscriptions[subscriber.getEvent()];
    subscribersList.remove(subscriber);
  }

  void publish(const std::string &event, const std::string &message) {
    auto it = mSubscriptions.find(event);
    if (it != mSubscriptions.end()) {
      auto &subscribers = it->second;
      for (const auto &subscriber : subscribers) {
        subscriber.notify(message);
      }
    }
  }

private:
  EventPublisher() = default;

  std::unordered_map<std::string, std::forward_list<EventSubscription>>
      mSubscriptions;
};

/**
 * @brief Inherit from this class to allow for automatic unsubscription when
 * destructor is called
 * 
 * e.g.
 * class MyClass : public EventSubscriber {
 *   MyClass() {
 *     subscribe("eventName", [] (const std::string &) {
 *       handleEvent();
 *     });
 *   }
 * };
 *
 */
class EventSubscriber {
public:
  EventSubscriber(const EventSubscriber &other) = delete;

protected:
  EventSubscriber() = default;
  ~EventSubscriber() {
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
  std::vector<EventSubscription> mSubscriptionHandles;
};
