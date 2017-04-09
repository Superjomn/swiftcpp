#ifndef SWIFTCPP_COMMON_H_
#define SWIFTCPP_COMMON_H_

#include <vector>
#include <iostream>
#include <sstream>

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
