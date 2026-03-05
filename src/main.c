#include "utils/config.h"
#include "utils/memory.h"
#include "utils/types.h"

#include "input/pc_input.h"
#include "draw/draw.h"
#include <stdio.h>

#include <string.h>

#define GET_INPUT(x) read(STDIN_FILENO, x, 1)

GameData game;

const uint8_t UNIT_HP[4] = { 7, 1, 15, 1 };


void prepareAsciiWars() {
  memset(&game, 0, sizeof (game));
  memset(&game.loadedChunks, 0xFF, sizeof (game.loadedChunks));

  // Change to center later
  game.cursorX = game.viewPortX = 0;
  game.cursorY = game.viewPortY = 0;

  // TODO Add to INITIAL_GOLD_VALUE
  game.players[0].goldCount = 100;
  game.players[1].goldCount = 100;
}

void moveCursor(int8_t x, int8_t y) {
    if (y == -1 && game.cursorY > 0) game.cursorY--;
    if (y == 1 && game.cursorY < (LIMIT_MAX_CHUNKS << 4) - 1) game.cursorY++;

    if (x == -1 && game.cursorX > 0) game.cursorX--;
    if (x == 1 && game.cursorX < (LIMIT_MAX_CHUNKS << 4) - 1) game.cursorX++;

    if (game.cursorX < game.viewPortX) 
        game.viewPortX = game.cursorX;
    if (game.cursorX >= game.viewPortX + SIZE_X_VIEWPORT) 
        game.viewPortX = game.cursorX - (SIZE_X_VIEWPORT - 1);
    
    if (game.cursorY < game.viewPortY) 
        game.viewPortY = game.cursorY;
    if (game.cursorY >= game.viewPortY + SIZE_Y_VIEWPORT) 
        game.viewPortY = game.cursorY - (SIZE_Y_VIEWPORT - 1);
}


uint8_t spawnUnit(uint16_t x, uint16_t y, UnitTypes type, uint8_t owner) {
  uint8_t unitX, unitY;
  uint8_t chunkX, chunkY;

  // Code 1: Invalid coordinates
  if (x >= (LIMIT_MAX_CHUNKS << 4) || y >= (LIMIT_MAX_CHUNKS << 4))
    return 1;

  // Convert from global to chunk + unit coordinates
  unitX = x & 0x0F;
  unitY = y & 0x0F;
  chunkX = x >> 4;
  chunkY = y >> 4;

  Unit *unit = NULL;

  for (uint8_t i=0; i<LIMIT_MAX_UNITS; i++) {
    Unit *u = &game.allUnits[i];
    if (u->hp == 0) {
      unit = u;
      unit->hp = UNIT_HP[type];
      break;
    }
  }

  // Code 3: Unit limit reached
  if (unit == NULL)
    return 3;

  for (uint8_t chunkId=0; chunkId<LIMIT_MAX_CHUNKS; chunkId++) {
    Chunk *c = &game.loadedChunks[chunkId];

    unit->chunkAddr = chunkId;

    // If chunk already loaded
    if (c->x == chunkX && c->y == chunkY) break;

    // If there is unloaded chunk
    else if (c->x == DISABLED_CHUNK_X && c->y == DISABLED_CHUNK_Y) {
      c->x = chunkX;
      c->y = chunkY;
      break;
    }

    // Code 2: No possible chunk
    else if (chunkId == LIMIT_MAX_CHUNKS - 1)
      return 2;
  }

  unit->x = unitX;
  unit->y = unitY;
  unit->type = type;
  unit->owner = owner;

  // Code 0: success
  return 0;
}

void spawnBuilding(uint16_t x, uint16_t y, BuildingTypes type, uint8_t owner) {}


void handleInput(char c) {
  if (c == '\x1b') {
    GET_INPUT(&c);
    GET_INPUT(&c);
  }

  // -- Moving
  int8_t x, y;
  x = 0;
  y = 0;

  // Horisontal
  if (c == 'a' || c == 'D' || c == 'h') {
    x = -1;
  } else if (c == 'd' || c == 'C' || c == 'l') {
    x = 1;
  }
  
  // Vertical
  else if (c == 'w' || c == 'A' || c == 'k') {
    y = -1;
  } else if (c == 's' || c == 'B' || c == 'j') {
    y = 1;
  }

  // Debug(maybe i will add it to release, but improved)
  else if (c == '0') {
    game.viewPortX = game.cursorX = 0;
  } else if (c == '$') {
    game.viewPortX = game.cursorX = (LIMIT_MAX_CHUNKS << 4) - 1;
    game.viewPortX -= SIZE_X_VIEWPORT + 1;
  }

  else if (c == 'g') {
    game.viewPortY = game.cursorY = 0;
  } else if (c == 'G') {
    game.viewPortY = game.cursorY = (LIMIT_MAX_CHUNKS << 4) - 1;
    game.viewPortY -= SIZE_Y_VIEWPORT + 1;
  }

  else if (c == 'O') {
    game.viewPortX = game.cursorX = (LIMIT_MAX_CHUNKS << 3) - 1;
    game.viewPortY = game.cursorY = (LIMIT_MAX_CHUNKS << 3) - 1;
  }

  // Spawn
  else if (c == '\t') {

  } else if (c == ' ') {
    spawnUnit(
        game.cursorX, game.cursorY,
        UNIT_SOLDIER, 0
    );
  } else if (c == '\b') {

  }

  moveCursor(x, y);
}


int main() {
  enableRawMode();

  prepareAsciiWars();

  printf("\x1b[H\x1b[2J");
  drawGrid();

  redrawViewport(game.viewPortX, game.viewPortY);
  printf("\x1b[1A\x1b[K 0 | 0 0 | 0 0\n\r");

  char c;
  while (1) {
    read(STDIN_FILENO, &c, 1);

    if (c == 3 || c == 4)
      break;

    handleInput(c);

    redrawViewport(game.viewPortX, game.viewPortY);
    printf("\x1b[1A\x1b[K %d | %d %d | %d %d\n\r", game.turnCount, game.cursorX, game.cursorY, game.viewPortX, game.viewPortY);
    game.turnCount++;
  }


  disableRawMode();
  return 0;
}

