#ifndef EDITOR_H
#define EDITOR_H

#include "callbacks.h"
#include "instruction.h"
#include "program_buffer.h"

class Editor {
public:
  Editor(ProgramBuffer &program_buffer, int screen_width);

  void next();
  void prev();
  void rewind();

  void up();
  void down();

  void incr();
  void decr();

  const Instruction *begin();
  const Instruction *end();
  const Instruction *curr();

private:
  ProgramBuffer &m_program_buffer;
  int m_screen_width;
};
#endif
