#ifndef VM_H
#define VM_H

#include "callbacks.h"
#include "data_buffer.h"
#include "instruction.h"
#include "program_buffer.h"

class VM {
public:
  VM(SystemCallbacks &callbacks, ProgramBuffer &program_buffer,
     DataBuffer &data_buffer);

  Instruction currentInstruction();
  char currentCell();

  void crank(int count);

  void reset();

  void load(char *program);

  bool done();

  // .
  void print();

  // ,
  void get();

private:
  ProgramBuffer &m_program_buffer;
  DataBuffer &m_data_buffer;
  SystemCallbacks &m_callbacks;

  bool m_done;

  // [
  void loopStart();

  // ]
  void loopEnd();
};

#endif
