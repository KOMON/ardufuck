#include "data_buffer.h"

DataBuffer::DataBuffer(size_t data_size)
    : m_data_size(data_size), m_data(new char[m_data_size]()), m_dp(m_data) {}

DataBuffer::~DataBuffer() { delete[] m_data; }

bool DataBuffer::atStart() { return m_dp == m_data; }

bool DataBuffer::atEnd() { return m_dp == &m_data[m_data_size - 1]; }

char DataBuffer::current() const { return *m_dp; }

void DataBuffer::rewind() { m_dp = m_data; }

void DataBuffer::left() {
  if (atStart()) {
    m_dp = &m_data[m_data_size - 1];
  }

  m_dp--;
}

void DataBuffer::right() {
  if (atEnd()) {
    m_dp = m_data;
  }

  m_dp++;
}

void DataBuffer::incr() {
  if (*m_dp == CHAR_MAX) {
    *m_dp = 0;
  }

  (*m_dp)++;
}

void DataBuffer::decr() {
  if (*m_dp == 0) {
    *m_dp = CHAR_MAX;
  }

  (*m_dp)--;
}

void DataBuffer::set(char c) { *m_dp = c; }

char *DataBuffer::begin() { return m_data; }
char *DataBuffer::end() { return &m_data[m_data_size]; }
char *DataBuffer::curr() { return m_dp; }
