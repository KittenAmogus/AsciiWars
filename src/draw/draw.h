#include "utils/types.h"

#ifndef DRAW_H
#define DRAW_H

extern const char ENTITY_CHARS[16];

extern const uint8_t BG_OWNER_COLORS[4];
extern const uint8_t FG_OWNER_COLORS[4];

void drawGrid();

void drawHUDGrid();
void drawHUD();

void redrawCell(uint16_t x, uint16_t y);

void redrawViewport(uint16_t cx, uint16_t cy);

#endif // DRAW_H
