#include "program_buffer.h"

ProgramBuffer::ProgramBuffer(size_t prog_size)
    : m_prog_size(prog_size), m_prog(new Instruction[m_prog_size]()),
      m_pp(m_prog), m_nest_count(0) {}

ProgramBuffer::~ProgramBuffer() { delete[] m_prog; }

Instruction ProgramBuffer::current() const { return *m_pp; }

bool ProgramBuffer::atStart() const { return m_pp == m_prog; }

bool ProgramBuffer::atEnd() const { return m_pp == &m_prog[m_prog_size - 1]; }

void ProgramBuffer::next() {
  if (atEnd()) {
    return;
  }

  if (*m_pp == Instruction::startLoop) {
    m_nest_count++;
  }

  m_pp++;

  if (*m_pp == Instruction::endLoop && m_nest_count > 0) {
    m_nest_count--;
  }
}

void ProgramBuffer::prev() {
  if (atStart()) {
    return;
  }

  if (*m_pp == Instruction::startLoop && m_nest_count > 0) {
    m_nest_count--;
  }

  m_pp--;

  if (*m_pp == Instruction::endLoop) {
    m_nest_count++;
  }
}

void ProgramBuffer::rewind() {
  m_pp = m_prog;
  m_nest_count = 0;
}

void ProgramBuffer::load(char *program) {
  rewind();

  while (*program != '\0' && !atEnd()) {
    *m_pp = static_cast<Instruction>(*program);
    next();
    program++;
  }

  while (!atEnd()) {
    *m_pp = Instruction::exit;
    next();
  }

  rewind();
}

const Instruction *ProgramBuffer::buffer() { return m_prog; }

const Instruction *ProgramBuffer::begin() { return m_prog; }

const Instruction *ProgramBuffer::end() { return &m_prog[m_prog_size]; }

const Instruction *ProgramBuffer::curr() { return m_pp; }

int ProgramBuffer::nestCount() const { return m_nest_count; }

void ProgramBuffer::set(Instruction i) { *m_pp = i; }

ptrdiff_t ProgramBuffer::pos() const { return m_pp - m_prog; }

size_t ProgramBuffer::size() const { return m_prog_size; }

void ProgramBuffer::jump(size_t index) {
  if (index <= 0) {
    m_pp = m_prog;
  } else if (index >= m_prog_size) {
    m_pp = &m_prog[m_prog_size];
  } else {
    m_pp = &m_prog[index];
  }
}
