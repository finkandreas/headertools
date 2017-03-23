#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace {
namespace s {
template <typename T>
T to(const std::string& s) {
  T ret;
  std::istringstream ss(s);
  ss >> ret;
  return ret;
}


std::string str(const std::string& s) { return s; }
std::string str(const char* s) { return s; }

template <typename T>
std::string str(const T& in) {
  std::ostringstream ss;
  ss << in;
  return ss.str();
}

class estring {
  private:
    std::string m_s;

  public:
    estring(const char* sz) { if (sz) m_s=sz; }
    estring(const std::string& s="") : m_s(s)  {}
    bool startsWith(const estring& other) const {
      return (m_s.substr(0, other.m_s.length()) == other.m_s);
    }
    bool endsWith(const estring& other) const {
      return (m_s.length() >= other.m_s.length() && (m_s.substr(m_s.length()-other.m_s.length(), other.m_s.length()) == other.m_s));
    }
    std::istream& getline(std::istream& in) {
      m_s = "";
      return std::getline(in, m_s);
    }
    std::vector<estring> split(const estring& delim=",") const {
      std::vector<estring> ret;
      size_t start = 0, pos;
      do {
        pos = m_s.find(delim.m_s, start);
        ret.push_back(m_s.substr(start, pos-start));
        start = pos+delim.m_s.length();
      } while(pos != std::string::npos);
      return ret;
    }
    template<typename T> T to() const {
      std::istringstream is(m_s);
      T ret = T();
      is >> ret;
      return ret;
    }
    bool empty() const { return m_s.empty(); }
    bool operator==(const estring& o) const { return o.m_s == m_s; }
    bool operator!=(const estring& o) const { return o.m_s != m_s; }
    operator std::string() const { return m_s; }
    const char* c_str() const { return m_s.c_str(); }
    const std::string& str() const { return m_s; }
};
} // s namespace
} // anonymous namespace

namespace std {
ostream& operator<<(ostream& dest, const ::s::estring& s) {
  dest << s.c_str();
}
}

