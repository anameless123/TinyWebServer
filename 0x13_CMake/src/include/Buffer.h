#pragma once
#include <string>

class Buffer {
 private:
  std::string buf;

 public:
  Buffer();
  explicit Buffer(ssize_t _size);
  ~Buffer();

  void append(const char *_str, int _size);
  ssize_t size();
  const char *c_str();
  void clear();
  void getline();
  void setBuf(const char *_buf);
};
