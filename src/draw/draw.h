#include "utils/types.h"

#ifndef DRAW_H
#define DRAW_H

void drawGrid();

void redrawCell(uint16_t x, uint16_t y);

void redrawViewport(uint16_t cx, uint16_t cy);

#endif // DRAW_H
