#include <Arduboy2.h>

#include "gamestate.h"
#include "vm.h"
#include "editor.h"
#include "callbacks.h"

#define RED 16, 0, 0
#define BLUE 0, 0, 16
#define GREEN 0, 16, 0
#define BLUEGREEN 0, 16, 16
#define PURPLE 16, 0, 16


GameState checkMajorStateChange(Arduboy2& arduboy, GameState current_state) {
  if (!arduboy.pressed(B_BUTTON)) {
    return current_state;
  }

  if (arduboy.justPressed(UP_BUTTON)) {
    return GameState::edit_move;
  } else if (arduboy.justPressed(DOWN_BUTTON)) {
    return GameState::run_init;
  }
}

GameState editMove(Arduboy2& arduboy, Editor& editor) {
  arduboy.setRGBled(16, 16, 16);

  if (arduboy.justPressed(A_BUTTON)) {
    return GameState::edit_edit;
  }
  
  if (arduboy.justPressed(LEFT_BUTTON)) {
    editor.prev();
  } else if (arduboy.justPressed(RIGHT_BUTTON)) {
    editor.next();
  } else if (arduboy.justPressed(UP_BUTTON)) {
    editor.up();
  } else if (arduboy.justPressed(DOWN_BUTTON)) {
    editor.down();
  }

  return GameState::edit_move;
}

GameState editEdit(Arduboy2& arduboy, Editor& editor) {
  arduboy.setRGBled(RED);

  if (arduboy.justPressed(A_BUTTON)) {
    return GameState::edit_move;
  }
  
  if (arduboy.justPressed(LEFT_BUTTON)) {
    editor.prev();
  } else if (arduboy.justPressed(RIGHT_BUTTON)) {
    editor.next();
  } else if (arduboy.justPressed(UP_BUTTON)) {
    editor.incr();
  } else if (arduboy.justPressed(DOWN_BUTTON)) {
    editor.decr();
  }

  return GameState::edit_edit;
}

GameState runInit(Arduboy2& arduboy, VM& vm){
  vm.reset();

  return GameState::run_paused;
}

GameState runPaused(Arduboy2& arduboy, VM& vm) {
  arduboy.setRGBled(BLUE);

  if (arduboy.justPressed(A_BUTTON)) {
    return GameState::run_running;
  }

  return GameState::run_paused;
}

GameState runRunning(Arduboy2& arduboy, VM& vm) {
  arduboy.setRGBled(BLUEGREEN);
  arduboy.setFrameRate(20);
  if (arduboy.justPressed(A_BUTTON)) {
    return GameState::run_paused;
  } else if (arduboy.pressed(RIGHT_BUTTON)) {
    return GameState::run_ff;
  }
  


  if (vm.done()) {
    return GameState::run_complete;
  }

  vm.crank(1);

  return GameState::run_running;
}

GameState runFf(Arduboy2& arduboy, VM& vm) {
  arduboy.setRGBled(GREEN);
  if (arduboy.justPressed(A_BUTTON)) {
    return GameState::run_paused;
  } if (arduboy.justReleased(RIGHT_BUTTON)) {
    return GameState::run_running;
  }

  if (vm.done()) {
    return GameState::run_complete;
  }

  vm.crank(8);
  
  return GameState::run_ff;
}


GameState runComplete(Arduboy2& arduboy, VM& vm) {
  arduboy.setRGBled(PURPLE);

  if (arduboy.justPressed(A_BUTTON)) {
    return GameState::run_init;
  }

  return GameState::run_complete;
}

void draw(Arduboy2& arduboy, GameState game_state, Editor& editor, VM& vm, ArduboyCallbacks &callbacks) {
  arduboy.clear();

  arduboy.setTextColor(BLACK);
  arduboy.setTextBackground(WHITE);
  if (game_state < GameState::run_init) {
    arduboy.println("^v chg inst <> mv cur");
  } else {
    arduboy.println("   A start, > fast   ");
  }

  arduboy.setTextColor(WHITE);
  arduboy.setTextBackground(BLACK);
  
  arduboy.setTextWrap(true);
  for (Instruction *i = editor.begin(); i != editor.end(); i++) {
    if (i == editor.curr()) {
      arduboy.setTextColor(BLACK);
      arduboy.setTextBackground(WHITE);
      arduboy.print((char)*i);
      arduboy.setTextColor(WHITE);
      arduboy.setTextBackground(BLACK);
      continue;
    }

    arduboy.print((char)*i);
  }

  if (game_state > GameState::run_init) {
    int x = arduboy.getCursorX();
    int y = arduboy.getCursorY();
    arduboy.setCursor(0, 52);
    arduboy.println(callbacks.getOutput());
    arduboy.setCursor(x, y);
  }
}

