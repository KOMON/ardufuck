#ifndef CALLBACKS_H
#define CALLBACKS_H

class SystemCallbacks {
public:
  virtual void print(char c) = 0;
  virtual char get() = 0;
};

class NoopCallbacks : public SystemCallbacks {
public:
  virtual void print(char c) { (void)c; }
  virtual char get() { return 0; }
  virtual void println(char *s) { (void)s; }
};

class ArduboyCallbacks : public SystemCallbacks {
  int m_print_cursor;
  char *m_output;

public:
  ArduboyCallbacks(int outputWidth)
      : m_print_cursor(0), m_output(new char[outputWidth + 1]) {}

  ~ArduboyCallbacks() { delete[] m_output; }

  virtual void print(char c) {
    m_output[m_print_cursor] = c;
    m_print_cursor++;
  }

  virtual char get() { return 'a'; }

  char *getOutput() { return m_output; }
};

#endif
