#ifndef CALLBACKS_H
#define CALLBACKS_H

class SystemCallbacks {
public:
  virtual void print(char c) = 0;
  virtual char get() = 0;
  virtual void reset() = 0;
};

class NoopCallbacks : public SystemCallbacks {
public:
  virtual void print(char c) { (void)c; }
  virtual char get() { return 0; }
  virtual void println(char *s) { (void)s; }
  virtual void reset() { }
};

class ArduboyCallbacks : public SystemCallbacks {
  int m_print_cursor;
  char *m_output;
  int m_width;

public:
  ArduboyCallbacks(int outputWidth)
    : m_width(outputWidth), m_print_cursor(0), m_output(new char[outputWidth + 1]) {
    for (int i = 0; i < m_width + 1; i++) {
      m_output[i] = ' ';
    }
  }


    ~ArduboyCallbacks() { delete[] m_output; }

    virtual void print(char c) {
      m_output[m_print_cursor] = c;
      m_print_cursor++;
    }

    virtual char get() { return 'a'; }

    char *getOutput() { return m_output; }

    void reset() {
      for (int i = 0; i < m_width + 1; i++) {
        m_output[i] = ' ';
      }

      m_print_cursor = 0;
    }
  };

#endif
