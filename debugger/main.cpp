#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <sstream>
#include <tuple>

#include "lua/lua.hpp"
#include "luaaa.hpp"

#include "../callbacks.h"
#include "../data_buffer.h"
#include "../program_buffer.h"
#include "../vm.h"

#define DATA_SIZE 256
#define PROG_SIZE 512

using namespace luaaa;

class TestCallbacks : public SystemCallbacks {
  std::string m_output;
  char m_input;

public:
  virtual void print(char c) { m_output.push_back(c); }

  virtual char get() { return m_input; }

  virtual void println(char *s) { (void)s; }

  std::string getOutput() { return m_output; }

  char getInput() { return m_input; }
  void setInput(char c) { m_input = c; }

  void printOutput() { printf("%s\n", m_output.c_str()); }
};

struct VMCtx {
  TestCallbacks test_callbacks;
  ProgramBuffer program_buffer;
  DataBuffer data_buffer;
  VM vm;

  VMCtx()
      : test_callbacks(TestCallbacks()),
        program_buffer(ProgramBuffer(PROG_SIZE)),
        data_buffer(DataBuffer(DATA_SIZE)),
        vm(VM(test_callbacks, program_buffer, data_buffer)) {}

  void load(std::string prog) {
    auto copy = prog.substr(0, PROG_SIZE);

    vm.load(&copy[0]);
  }

  void crank(int count) { vm.crank(count); }

  std::tuple<std::string, int> program() {
    std::string s;
    int index = 1;
    int current_index = 1;
    for (const Instruction *i = program_buffer.begin();
         i != program_buffer.end(); i++) {
      s.push_back((char)(*i));

      if (i == program_buffer.curr()) {
        current_index = index;
      }

      index++;
    }

    return std::tuple<std::string, int>(s, current_index);
  }

  std::tuple<std::vector<int>, int> data() {
    std::vector<int> data;

    int index = 0;
    int current_index = 0;

    const char *d = data_buffer.begin();
    for (; d + 10 < data_buffer.curr(); d++) {
    }

    char buffer[6];
    for (int i = 0; i < 20 && d <= data_buffer.end(); i++, d++) {
      data.push_back((int)*d);

      if (d == data_buffer.curr()) {
        current_index = index;
      }

      index++;
    }

    return std::tuple<std::vector<int>, int>(data, current_index + 1);
  }

  void exec() { vm.crank(-1); }

  std::string currentInstruction() {
    return std::string(1, (char)vm.currentInstruction());
  }

  std::string currentCell() { return std::string(1, data_buffer.current()); }

  std::string getOutput() { return test_callbacks.getOutput(); }

  void setInput(std::string s) { test_callbacks.setInput(s[0]); }

  std::string getInput() { return std::string(1, test_callbacks.getInput()); }

  bool done() { return vm.done(); }

  void prev() { program_buffer.prev(); }

  void next() { program_buffer.next(); }
};

int main() {
  lua_State *l = luaL_newstate();

  luaL_openlibs(l);

  LuaClass<VMCtx> vmCtx(l, "VM");

  vmCtx.ctor();
  vmCtx.fun("load", &VMCtx::load);
  vmCtx.fun("crank", &VMCtx::crank);
  vmCtx.fun("program", &VMCtx::program);
  vmCtx.fun("data", &VMCtx::data);
  vmCtx.fun("exec", &VMCtx::exec);
  vmCtx.fun("currentInstruction", &VMCtx::currentInstruction);
  vmCtx.fun("currentCell", &VMCtx::currentCell);
  vmCtx.fun("getOutput", &VMCtx::getOutput);
  vmCtx.fun("done", &VMCtx::done);
  vmCtx.fun("setInput", &VMCtx::setInput);
  vmCtx.fun("getInput", &VMCtx::getInput);
  vmCtx.fun("prev", &VMCtx::prev);
  vmCtx.fun("next", &VMCtx::next);

  auto value = luaL_dofile(l, "debugger.lua");

  if (value == 1)
    printf("Do file error: %s", lua_tostring(l, -1));

  lua_close(l);
}
