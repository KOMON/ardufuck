#include "editor.h"

Editor::Editor(ProgramBuffer &program_buffer, int screen_width)
    : m_program_buffer(program_buffer), m_screen_width(screen_width) {}

void Editor::next() { m_program_buffer.next(); }

void Editor::prev() { m_program_buffer.prev(); }

void Editor::rewind() { m_program_buffer.rewind(); }

void Editor::up() {
  if (m_program_buffer.pos() >= m_screen_width) {
    m_program_buffer.jump(m_program_buffer.pos() - m_screen_width);
  }
}

void Editor::down() {
  if (m_program_buffer.pos() <=
      (ptrdiff_t)(m_program_buffer.size() - m_screen_width)) {
    m_program_buffer.jump(m_program_buffer.pos() + m_screen_width);
  }
}

/**
 * _ -> + -> - -> > -> < -> [ -> ] -> . -> , -> _
 */
void Editor::incr() {
  switch (m_program_buffer.current()) {
  case Instruction::exit:
    m_program_buffer.set(Instruction::incr);
    break;
  case Instruction::incr:
    m_program_buffer.set(Instruction::decr);
    break;
  case Instruction::decr:
    m_program_buffer.set(Instruction::right);
    break;
  case Instruction::right:
    m_program_buffer.set(Instruction::left);
    break;
  case Instruction::left:
    m_program_buffer.set(Instruction::startLoop);
    break;
  case Instruction::startLoop:
    m_program_buffer.set(Instruction::endLoop);
    break;
  case Instruction::endLoop:
    m_program_buffer.set(Instruction::print);
    break;
  case Instruction::print:
    m_program_buffer.set(Instruction::get);
    break;
  case Instruction::get:
    m_program_buffer.set(Instruction::exit);
    break;
  }
}

/**
 * _ -> , -> . -> ] -> [ -> < -> > -> - -> + -> _
 */
void Editor::decr() {
  switch (m_program_buffer.current()) {
  case Instruction::exit:
    m_program_buffer.set(Instruction::get);
    break;
  case Instruction::get:
    m_program_buffer.set(Instruction::print);
    break;
  case Instruction::print:
    m_program_buffer.set(Instruction::endLoop);
    break;
  case Instruction::endLoop:
    m_program_buffer.set(Instruction::startLoop);
    break;
  case Instruction::startLoop:
    m_program_buffer.set(Instruction::left);
    break;
  case Instruction::left:
    m_program_buffer.set(Instruction::right);
    break;
  case Instruction::right:
    m_program_buffer.set(Instruction::decr);
    break;
  case Instruction::decr:
    m_program_buffer.set(Instruction::incr);
    break;
  case Instruction::incr:
    m_program_buffer.set(Instruction::exit);
    break;
  }
}

const Instruction *Editor::begin() { return m_program_buffer.begin(); }
const Instruction *Editor::end() { return m_program_buffer.end(); }
const Instruction *Editor::curr() { return m_program_buffer.curr(); }
