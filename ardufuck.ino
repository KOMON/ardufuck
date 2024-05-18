#include <Arduboy2.h>

#include "callbacks.h"
#include "data_buffer.h"
#include "editor.h"
#include "program_buffer.h"
#include "vm.h"
#include "gamestate.h"

Arduboy2 arduboy;

#define DATA_SIZE 256
#define PROG_SIZE 512
#define OUTPUT_WIDTH 40
#define INPUT_WIDTH 40

char *hello_world = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+"
                    "++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";

ProgramBuffer *program_buffer;
DataBuffer *data_buffer;
ArduboyCallbacks *callbacks;
VM *vm;
Editor *editor;

GameState game_state;

void setup() {
  arduboy.begin();
  arduboy.clear();
  arduboy.setCursor(0, 0);
  arduboy.println("Hello, World!");
  arduboy.display();
  while (!arduboy.pressed(A_BUTTON)) {}
  arduboy.waitNoButtons();
  game_state = GameState::edit_move;
  
  callbacks = new ArduboyCallbacks(OUTPUT_WIDTH);
  program_buffer = new ProgramBuffer(PROG_SIZE);
  data_buffer = new DataBuffer(DATA_SIZE);
  vm = new VM(*callbacks, *program_buffer, *data_buffer);
  editor = new Editor(*program_buffer, 21);
  program_buffer->load(hello_world);
}

void loop() {
  arduboy.pollButtons();

  GameState new_game_state = game_state;

  new_game_state = checkMajorStateChange(arduboy, new_game_state);

  if (new_game_state != game_state) {
    game_state = new_game_state;
    return;
  }

   draw(arduboy, game_state, *editor, *vm, *callbacks);

  switch (game_state) {
  case GameState::edit_move:
    game_state = editMove(arduboy, *editor);
    break;
  case GameState::edit_edit:
    game_state = editEdit(arduboy, *editor);
    break;
  case GameState::run_init:
    game_state = runInit(arduboy, *vm);
    break;
  case GameState::run_paused:
    game_state = runPaused(arduboy, *vm);
    break;
  case GameState::run_running:
    game_state = runRunning(arduboy, *vm);
    break;
  case GameState::run_ff:
    game_state = runFf(arduboy, *vm);
    break;
  case GameState::run_complete:
    game_state = runComplete(arduboy, *vm);
    break;
  }
  
  arduboy.display();
}
