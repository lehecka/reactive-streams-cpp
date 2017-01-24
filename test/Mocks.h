// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include <cassert>
#include <exception>

#include <gmock/gmock.h>

#include "ReactiveStreams.h"

namespace reactivestreams {

/// GoogleMock-compatible Publisher implementation for fast prototyping.
/// UnmanagedMockPublisher's lifetime MUST be managed externally.
template <typename T, typename E = std::exception_ptr>
class MockPublisher : public Publisher<T, E> {
 public:
  MOCK_METHOD1_T(subscribe_, void(std::shared_ptr<Subscriber<T, E>> subscriber));

  void subscribe(std::shared_ptr<Subscriber<T, E>> subscriber) noexcept override {
    subscribe_(std::move(subscriber));
  }
};

/// GoogleMock-compatible Subscriber implementation for fast prototyping.
/// MockSubscriber MUST be heap-allocated, as it manages its own lifetime.
/// For the same reason putting mock instance in a smart pointer is a poor idea.
/// Can only be instanciated for CopyAssignable E type.
template <typename T, typename E>
class MockSubscriber : public Subscriber<T, E> {
 public:
  MOCK_METHOD1(onSubscribe_, void(std::shared_ptr<Subscription> subscription));
  MOCK_METHOD1_T(onNext_, void(T& value));
  MOCK_METHOD0(onComplete_, void());
  MOCK_METHOD1_T(onError_, void(E ex));

  void onSubscribe(std::shared_ptr<Subscription> subscription) noexcept override {
    subscription_ = subscription;
    // We allow registering the same subscriber with multiple Publishers.
    EXPECT_CALL(checkpoint_, Call());
    onSubscribe_(subscription);
  }

  void onNext(T element) noexcept override {
    onNext_(element);
  }

  void onComplete() noexcept override {
    onComplete_();
    checkpoint_.Call();
    subscription_ = nullptr;
  }

  void onError(E ex) noexcept override {
    onError_(ex);
    checkpoint_.Call();
    subscription_ = nullptr;
  }

  Subscription* subscription() const {
    return subscription_.get();
  }

 private:
  std::shared_ptr<Subscription> subscription_;
  testing::MockFunction<void()> checkpoint_;
};

/// GoogleMock-compatible Subscriber implementation for fast prototyping.
/// MockSubscriber MUST be heap-allocated, as it manages its own lifetime.
/// For the same reason putting mock instance in a smart pointer is a poor idea.
class MockSubscription : public Subscription {
 public:
  MOCK_METHOD1(request_, void(size_t n));
  MOCK_METHOD0(cancel_, void());

  void request(size_t n) noexcept override {
    if (!requested_) {
      requested_ = true;
      EXPECT_CALL(checkpoint_, Call()).Times(1);
    }

    request_(n);
  }

  void cancel() noexcept override {
    cancel_();
    checkpoint_.Call();
  }

 private:
  bool requested_{false};
  testing::MockFunction<void()> checkpoint_;
};

}
