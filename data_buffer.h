#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include <limits.h>
#include <stddef.h>

class DataBuffer {
public:
  DataBuffer(size_t data_size);
  ~DataBuffer();

  char current() const;
  void reset();

  void left();
  void right();
  void incr();
  void decr();

  void set(char c);

  void print();

  char *begin();
  char *end();
  char *curr();

private:
  const size_t m_data_size;
  char *const m_data;

  char *m_dp;

  bool atStart();
  bool atEnd();
};

#endif
