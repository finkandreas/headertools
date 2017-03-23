// -std=c++11 -lssl -lcrypto
#pragma once
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>
#include <vector>

template<typename T>
std::string get_md5sum(std::vector<T> data)
{
  unsigned char md[MD5_DIGEST_LENGTH];
  MD5((unsigned char*)data.data(), data.size()*sizeof(T), md);

  std::ostringstream ss;
  ss << std::setw(2) << std::hex << std::setfill('0');
  for(int n=0; n<MD5_DIGEST_LENGTH; n++) ss << std::setw(2) << (int)md[n];
  return ss.str();
}

