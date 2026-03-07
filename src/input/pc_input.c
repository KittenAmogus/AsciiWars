#include "input/pc_input.h"
#include "input/input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct termios orig_termios;


InputEvent pollEvent(void) {
  char c;
  InputEvent event;
  memset(&event, 0, sizeof (event));

  read(STDIN_FILENO, &c, 1);

  if (c == '\x1b') {
    if (read(STDIN_FILENO, &c, 1) == 0) event.type = INPUT_NONE;
    if (read(STDIN_FILENO, &c, 1) == 0) event.type = INPUT_NONE;
  }

  event.raw.pcChar = c;
  
  // Quit
  if (c == 3 || c == 4) {
    event.type = INPUT_QUIT;
    return event;
  }

  // Cursor moving
  if (c == 'w' || c == 'k' || c == 'A') {
    event.type = INPUT_MOVE;
    event.data.moveEvent.dy = -1;
    return event;
  } else if (c == 's' || c == 'j' || c == 'B') {
    event.type = INPUT_MOVE;
    event.data.moveEvent.dy = 1;
    return event;
  } else if (c == 'd' || c == 'l' || c == 'C') {
    event.type = INPUT_MOVE;
    event.data.moveEvent.dx = 1;
    return event;
  } else if (c == 'a' || c == 'h' || c == 'D') {
    event.type = INPUT_MOVE;
    event.data.moveEvent.dx = -1;
    return event;
  }

  // Action
  if (c == ' ' || c == '\n' || c == 'o') {
    event.type = INPUT_ACTION;
    return event;
  }

  // Cancel
  if (c == '\b' || c == 'p' || c == 26) {
    event.type = INPUT_CANCEL;
    return event;
  }

  // Next turn
  if (c == 'N') {
    event.type = INPUT_NEXT_TURN;
    return event;
  }

  return event;
}


void disableRawMode(void) {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  printf("\x1b[?25h");
}

void enableRawMode(void) {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    exit(1);

  atexit(disableRawMode);
  struct termios raw = orig_termios;

  raw.c_iflag &= ~(IXON | ICRNL);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  // Swap to block input
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;

  printf("\x1b[?25l");

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    exit(1);
}
