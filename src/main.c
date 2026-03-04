#include "utils/config.h"
#include "utils/memory.h"
#include "utils/types.h"

#include "input/pc_input.h"
#include <stdio.h>


int main(void) {
  printf("Set raw mode...\n\r");
  enableRawMode();

  while (1) {
    char c; read(STDIN_FILENO, &c, 1);
    if (c == '\x1b')
      printf("I cannot now read 'x1b'!\n\r");

    // Break on (ENTER, ^C, ^D, BACKSPACE)
    else if (c == 13 || c == 3 || c == 4 || c == 127)
      break;

    else
      printf("You entered: '%c'!\n\r", c);
  }

  printf("Disabling raw mode...\n\r");
  disableRawMode();

  return 0;
}

