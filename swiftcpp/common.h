#ifndef SWIFTCPP_COMMON_H_
#define SWIFTCPP_COMMON_H_

#include <iostream>
#include <sstream>
#include <vector>

// copied from mxnet
#ifndef DISALLOW_COPY_AND_ASSIGN
#if DMLC_USE_CXX11
#define DISALLOW_COPY_AND_ASSIGN(T)                                            \
  T(T const &) = delete;                                                       \
  T(T &&) = delete;                                                            \
  T &operator=(T const &) = delete;                                            \
  T &operator=(T &&) = delete
#else
#define DISALLOW_COPY_AND_ASSIGN(T)                                            \
  T(T const &);                                                                \
  T &operator=(T const &)
#endif
#endif

namespace swiftcpp {

/*
 * split a string by delimiter
 */
inline std::vector<std::string> Split(const std::string &s, char delim) {
  std::string item;
  std::istringstream is(s);
  std::vector<std::string> ret;
  while (std::getline(is, item, delim)) {
    ret.push_back(item);
  }
  return ret;
}

} // namespace swiftcpp
#endif
