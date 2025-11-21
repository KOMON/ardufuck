#include "vm.h"

VM::VM(SystemCallbacks &callbacks, ProgramBuffer &program_buffer,
       DataBuffer &data_buffer)
    : m_program_buffer(program_buffer), m_data_buffer(data_buffer),
      m_callbacks(callbacks), m_done(false) {}

void VM::loopStart() {
  if (m_data_buffer.current() != 0) {
    return;
  }

  int start_nest_count = m_program_buffer.nestCount();

  do {
    m_program_buffer.next();
  } while (!(m_program_buffer.current() == Instruction::endLoop &&
             start_nest_count == m_program_buffer.nestCount()) &&
           !m_program_buffer.atEnd());
}

void VM::loopEnd() {
  if (m_data_buffer.current() == 0) {
    return;
  }

  int start_nest_count = m_program_buffer.nestCount();

  do {
    m_program_buffer.prev();
  } while (!(m_program_buffer.current() == Instruction::startLoop &&
             start_nest_count == m_program_buffer.nestCount()) &&
           !m_program_buffer.atStart());
}

void VM::print() { m_callbacks.print(m_data_buffer.current()); }

void VM::get() { m_data_buffer.set(m_callbacks.get()); }

void VM::crank(int count) {
  bool crank_to_end = count == -1;
  while ((count > 0 || crank_to_end) && !m_done) {
    count--;

    switch (m_program_buffer.current()) {
    case Instruction::incr:
      m_data_buffer.incr();
      break;
    case Instruction::decr:
      m_data_buffer.decr();
      break;
    case Instruction::left:
      m_data_buffer.left();
      break;
    case Instruction::right:
      m_data_buffer.right();
      break;
    case Instruction::startLoop:
      loopStart();
      break;
    case Instruction::endLoop:
      loopEnd();
      break;
    case Instruction::print:
      print();
      break;
    case Instruction::get:
      get();
      break;
    case Instruction::exit:
      m_done = true;
      break;
    }

    m_program_buffer.next();
    if (m_program_buffer.atEnd()) {
      m_done = true;
    }
  }
}

void VM::load(char *program) { m_program_buffer.load(program); }

bool VM::done() { return m_done; }

Instruction VM::currentInstruction() { return m_program_buffer.current(); }

char VM::currentCell() { return m_data_buffer.current(); }

void VM::reset() {
  m_data_buffer.reset();
  m_program_buffer.rewind();
  m_done = false;
  m_callbacks.reset();
}
