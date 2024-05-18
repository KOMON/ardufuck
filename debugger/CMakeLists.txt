cmake_minimum_required(VERSION 3.25)

project(test
  LANGUAGES C CXX
  VERSION 1.0
)

## lua
add_library(lua
  lua/lapi.c
  lua/lauxlib.c
  lua/lbaselib.c
  lua/lcode.c
  lua/lcorolib.c
  lua/lctype.c
  lua/ldblib.c
  lua/ldebug.c
  lua/ldo.c
  lua/ldump.c
  lua/lfunc.c
  lua/lgc.c
  lua/linit.c
  lua/liolib.c
  lua/llex.c
  lua/lmathlib.c
  lua/lmem.c
  lua/loadlib.c
  lua/lobject.c
  lua/lopcodes.c
  lua/loslib.c
  lua/lparser.c
  lua/lstate.c
  lua/lstring.c
  lua/lstrlib.c
  lua/ltable.c
  lua/ltablib.c
  lua/ltm.c
  lua/lua.c
  lua/lundump.c
  lua/lutf8lib.c
  lua/lvm.c
  lua/lzio.c
)

target_include_directories(lua PUBLIC lua)
target_compile_definitions(lua PRIVATE LUA_USE_LINUX)
target_link_libraries(lua PRIVATE dl)

add_executable(debugger
  main.cpp
  "../data_buffer.cpp"
  "../program_buffer.cpp"
  "../vm.cpp"
)

target_link_libraries(debugger PRIVATE lua)

set_property(TARGET debugger PROPERTY CXX_STANDARD 11)

add_custom_command(
  TARGET debugger POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/debugger.lua
  ${CMAKE_CURRENT_BINARY_DIR}/debugger.lua
)
