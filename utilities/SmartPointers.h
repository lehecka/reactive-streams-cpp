// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>

namespace reactivestreams {

class Subscription;

/// A "smart pointer" to an arbitrary Subscriber.
///
/// Accessing a Subscriber via this class only ensures that a terminal signal is
/// delivered to the pointee exactly once. Note that Subscriber::onSubscriber
/// must be delivered to the pointee before it is wrapped in SubscriberPtr.
///
/// This class is not thread-safe. User must provide external synchronisation.
template <typename S>
class SubscriberPtr {
 public:
  using SharedPtrT = std::shared_ptr<S>;

  SubscriberPtr() = default;
  explicit SubscriberPtr(std::shared_ptr<S> subscriber) : subscriber_(std::move(subscriber)) {
    assert(subscriber_);
  }

  SubscriberPtr(const SubscriberPtr& rhs) = delete;
  SubscriberPtr& operator=(const SubscriberPtr& rhs) = delete;

  SubscriberPtr(SubscriberPtr&& rhs) noexcept : subscriber_(rhs.release()) {}
  SubscriberPtr& operator=(SubscriberPtr&& rhs) noexcept {
    subscriber_ = rhs.release();
  }

  ~SubscriberPtr() noexcept {
    // Tail-call
    reset();
  }

  void reset(std::shared_ptr<S> subscriber = std::shared_ptr<S>()) {
    subscriber_.swap(subscriber);
    if (subscriber) {
      // Tail-call
      subscriber->onComplete();
    }
  }

  std::shared_ptr<S> release() {
    return std::move(subscriber_);
  }

//  S* get() const {
//    return subscriber_.get();
//  }

  explicit operator bool() const {
    return (bool)subscriber_;
  }

  operator std::shared_ptr<S>() const {
    return subscriber_;
  }

  void onSubscribe(std::shared_ptr<Subscription> subscription) const {
    assert(subscriber_);
    // calling onSubscribe can result in calling terminating signals (onComplete/onError/cancel)
    // and releasing shared_ptrs which may destroy object instances while onSubscribe method is still on the stack
    // we will protect against such bugs by keeping a strong reference to the object while in onSubscribe method
    auto subscriberCopy = subscriber_;
    subscriberCopy->onSubscribe(std::move(subscription));
  }

  void onNext(typename S::ElementType element) const {
    // Tail-call
    assert(subscriber_);
    // calling onNext can result in calling terminating signals (onComplete/onError/cancel)
    // and releasing shared_ptrs which may destroy object instances while onNext method is still on the stack
    // we will protect against such bugs by keeping a strong reference to the object while in onNext method
    auto subscriberCopy = subscriber_;
    subscriberCopy->onNext(std::move(element));
  }

  void onComplete() {
    if (auto subscriber = release()) {
      // Tail-call
      subscriber->onComplete();
    }
  }

  void onError(typename S::ErrorType ex) {
    if (auto subscriber = release()) {
      // Tail-call
      subscriber->onError(std::move(ex));
    }
  }

 private:
  std::shared_ptr<S> subscriber_;
};

template <typename S>
SubscriberPtr<S> makeSubscriberPtr(std::shared_ptr<S> subscriber) {
  return SubscriberPtr<S>(std::move(subscriber));
}

/// A "smart pointer" to an arbitrary Subscription.
///
/// Accessing a Subscription via this class only ensures that a terminal signal
/// is delivered to the pointee exactly once.
///
/// This class is not thread-safe. User must provide external synchronisation.
template <typename S>
class SubscriptionPtr {
 public:
  using SharedPtrT = std::shared_ptr<S>;

  SubscriptionPtr() = default;
  explicit SubscriptionPtr(std::shared_ptr<S> subscription) : subscription_(std::move(subscription)) {
    assert(subscription_);
  }

  SubscriptionPtr(const SubscriptionPtr& rhs) = delete;
  SubscriptionPtr& operator=(const SubscriptionPtr& rhs) = delete;

  SubscriptionPtr(SubscriptionPtr&& rhs) noexcept
      : subscription_(rhs.release()) {}
  SubscriptionPtr& operator=(SubscriptionPtr&& rhs) noexcept {
    subscription_ = rhs.release();
  }

  ~SubscriptionPtr() noexcept {
    // Tail-call
    reset();
  }

  void reset(std::shared_ptr<S> subscription = std::shared_ptr<S>()) {
    subscription_.swap(subscription);
    if (subscription) {
      // Tail-call
      subscription->cancel();
    }
  }

  std::shared_ptr<S> release() {
    return std::move(subscription_);
  }

//  S* get() const {
//    return subscription_.get();
//  }

  explicit operator bool() const {
    return (bool)subscription_;
  }

  operator std::shared_ptr<S>() const {
    return subscription_;
  }

  void request(size_t n) const {
    // Tail-call
    assert(subscription_);
    // calling request can result in calling terminating signals (onComplete/onError/cancel)
    // and releasing shared_ptrs which may destroy object instances while request method is still on the stack
    // we will protect against such bugs by keeping a strong reference to the object while in request method
    auto subscriptionCopy = subscription_;
    subscriptionCopy->request(n);
  }

  void cancel() {
    if (auto subscription = release()) {
      // Tail-call
      subscription->cancel();
    }
  }

 private:
  std::shared_ptr<S> subscription_;
};

template <typename S>
SubscriptionPtr<S> makeSubscriptionPtr(std::shared_ptr<S> subscription) {
  return SubscriptionPtr<S>(std::move(subscription));
}
}
