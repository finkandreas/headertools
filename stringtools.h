#include <sstream>

namespace s {
template <typename T>
to(const std::string& s) {
  T ret;
  std::istringstream ss(s);
  s >> ret;
  return ret;
}


template <typename T>
str(const T& in) {
  std::ostringstream ss;
  ss << in;
  return in.str();
}
