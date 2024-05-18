#ifndef PROGRAM_BUFFER_H
#define PROGRAM_BUFFER_H

#include "callbacks.h"
#include "instruction.h"
#include <stddef.h>

class ProgramBuffer {
public:
  ProgramBuffer(size_t prog_size);
  ~ProgramBuffer();

  Instruction current() const;

  const Instruction *buffer();

  const Instruction *begin();
  const Instruction *end();
  const Instruction *curr();

  bool atStart() const;
  bool atEnd() const;

  int nestCount() const;

  ptrdiff_t pos() const;
  size_t size() const;

  void next();
  void prev();
  void jump(size_t index);
  void rewind();
  void load(char *program);

  void set(Instruction i);

private:
  const size_t m_prog_size;
  Instruction *const m_prog;
  Instruction *m_pp;
  int m_nest_count;
};
#endif
