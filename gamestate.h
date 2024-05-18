#ifndef GAMESTATE_H
#define GAMESTATE_H

enum class GameState : char {
  edit_move = 0,
  edit_edit = 1,

  run_init = 10,
  run_paused = 11,
  run_running = 12,
  run_ff = 13,
  run_complete = 14,
};

#endif
