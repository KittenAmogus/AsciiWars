#ifndef PC_INPUT_H
#define PC_INPUT_H

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios orig_termios;

void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

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

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    exit(1);
}

#endif
