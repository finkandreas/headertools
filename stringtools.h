#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace af {
namespace s {
template <typename T>
T to(const std::string& s) {
  T ret;
  std::istringstream ss(s);
  ss >> ret;
  return ret;
}


inline std::string str(const std::string& s) { return s; }
inline std::string str(const char* s) { return s; }

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
    estring(std::string&& s) : m_s(s) {}
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
    std::vector<estring> split(const estring& delim=",", bool discardEmpty=true) const {
      std::vector<estring> ret;
      size_t start = 0, pos;
      do {
        pos = m_s.find(delim.m_s, start);
        if (false == discardEmpty || pos-start > delim.m_s.length()) {
          ret.push_back(m_s.substr(start, pos-start));
        }
        start = pos+delim.m_s.length();
      } while(pos != std::string::npos);
      return ret;
    }
    estring& replace(const estring& searchString, const estring& replaceString) {
      size_t pos=0;
      while (pos != std::string::npos) {
        pos = m_s.find(searchString, pos);
        if (pos != std::string::npos) {
          m_s.replace(pos, searchString.size(), replaceString);
        }
      }
      return *this;
    }
    template<typename T> T to() const {
      std::istringstream is(m_s);
      T ret = T();
      is >> ret;
      return ret;
    }
    size_t size() const { return m_s.size(); }
    bool empty() const { return m_s.empty(); }
    bool operator==(const estring& o) const { return o.m_s == m_s; }
    bool operator!=(const estring& o) const { return o.m_s != m_s; }
    estring& operator+(const char* o) { m_s+=o; return *this; }
    operator const std::string&() const { return m_s; }
    operator std::string&() { return m_s; }
    const char* c_str() const { return m_s.c_str(); }
    const std::string& str() const { return m_s; }
};
} // s namespace
inline s::estring& operator+(const char* sz, s::estring& es) {
  std::string& internalString = es;
  internalString = sz + internalString;
  return es;
}
} // af namespace

namespace std {
inline ostream& operator<<(ostream& dest, const af::s::estring& s) {
  return (dest << s.c_str());
}
}

