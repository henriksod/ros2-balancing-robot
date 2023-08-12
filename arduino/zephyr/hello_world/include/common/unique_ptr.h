// Copyright (c) 2023, Henrik Söderlund

#pragma once

#include "common/utility.h"

namespace std {
template<typename T>
struct default_delete {
  default_delete()                      = default;
  default_delete(const default_delete&) = default;

  template<typename U>
  default_delete(const default_delete<U>&) {}

  void operator()(T *ptr) const {
    delete ptr;
  }
};

template<typename T, typename Deleter = default_delete<T> >
class unique_ptr {
 public:
  unique_ptr() = default;
  explicit unique_ptr(T *ptr) : ptr_(ptr) {}

  unique_ptr(T *ptr, const Deleter& deleter) : ptr_(ptr), deleter_(deleter) {}

  ~unique_ptr() {
    deleter_(ptr_);
  }

  unique_ptr(const unique_ptr& other) = delete;

  unique_ptr(unique_ptr&& other) noexcept
    : ptr_(other.release()), deleter_(other.deleter_) {}

  // generalized move ctor
  template<typename U, typename E>
  unique_ptr(unique_ptr<U, E>&& other) noexcept
    : ptr_(other.release()), deleter_(std::forward<E>(other.get_deleter())) {}

  unique_ptr& operator=(const unique_ptr& other) = delete;

  unique_ptr& operator=(unique_ptr&& other) noexcept {
    static_cast<void>(unique_ptr(std::move(other)).swap(*this));
    return *this;
  }

  void reset(T *ptr) noexcept {
    deleter_(ptr_);
    ptr_ = ptr;
  }

  T* release() noexcept {
    auto old_ptr = ptr_;

    ptr_ = nullptr;
    return old_ptr;
  }

  void swap(unique_ptr& other) noexcept {
    using std::swap;
    swap(ptr_, other.ptr_);
  }

  T& operator*() const noexcept {
    return *ptr_;
  }

  T * operator->() const noexcept {
    return ptr_;
  }

  T* get() const noexcept {
    return ptr_;
  }

  Deleter get_deleter() const noexcept {
    return deleter_;
  }

  explicit operator bool() {
    return ptr_ != nullptr;
  }

 private:
  T *ptr_          = nullptr;
  Deleter deleter_ = Deleter();
};
}  // namespace std
