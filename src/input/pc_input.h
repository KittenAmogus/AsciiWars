#include <termios.h>
#include <unistd.h>

#include "input/input.h"

#ifndef PC_INPUT_H
#define PC_INPUT_H

InputEvent pollEvent(void);

void disableRawMode(void);
void enableRawMode(void);

#endif
