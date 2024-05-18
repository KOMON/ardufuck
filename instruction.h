// -*-c++-*-

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

enum class Instruction : char {
  exit = 0,
  left = '<',
  right = '>',
  incr = '+',
  decr = '-',
  startLoop = '[',
  endLoop = ']',
  print = '.',
  get = ',',
};

#endif
