// Copyright (c) 2023, Henrik Söderlund

#pragma once

namespace std {
template<typename T>
struct identity {
  typedef T type;
};

template<typename T>
struct identity<T&> {
  // remove reference
  typedef T type;
};

template<typename T>
struct identity<T &&> {
  // remove rvalue reference
  typedef T type;
};

template<typename T>
T && forward(typename identity<T>::type && param) {
  return static_cast<typename identity<T>::type &&>(param);
}

template<class T>inline
typename identity<T>::type &&
move(T && param) {  // forward _Arg as movable
  return (typename identity<T>::type &&) param;
}

template<typename T>
void swap(T& t1, T& t2) {
  T tmp(std::move(t1));

  t1 = std::move(t2);
  t2 = std::move(tmp);
}
}  // namespace std
