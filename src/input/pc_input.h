#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#ifndef PC_INPUT_H
#define PC_INPUT_H

static struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  printf("\x1b[?25h");
}

void enableRawMode() {
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

#endif
