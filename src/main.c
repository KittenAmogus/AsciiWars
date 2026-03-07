#include "utils/config.h"
#include "utils/memory.h"
#include "utils/types.h"
#include "utils/utils.h"

#include "input/pc_input.h"
#include "draw/draw.h"
#include <stdio.h>

#include <string.h>

#define GET_INPUT(x) read(STDIN_FILENO, x, 1)

GameData game;

uint8_t needRedraw;


void selectUnit() { uint8_t localX = game.cursorX & 0x0F;
  uint8_t localY = game.cursorY & 0x0F;

  uint8_t chunkX = game.cursorX >> 4;
  uint8_t chunkY = game.cursorY >> 4;

  for (uint8_t i=0; i<LIMIT_MAX_ENTITIES; i++) {
    Entity *e = &game.allEntities[i];
    
    if (e->type == ENTITY_NULL)
      continue;

    if (e->x == localX && e->y == localY) {
      Chunk c = game.lazyChunks[e->chunkAddr];
      if (c.x != chunkX || c.y != chunkY) {
        continue;
      }
    } else {
      continue;
    }
 
    e->isBlinking = 1;
    game.selUnit = i;
    break;
  }

  needRedraw = 1;
}

void unselectUnit() {
  uint8_t localX, localY;
  localX = game.cursorX & 0x0F;
  localY = game.cursorY & 0x0F;

  uint8_t chunk = findAvailableChunk_INDEX(game.cursorX >> 4, game.cursorY >> 4);
  if (chunk == 0xFF)
    return;

  Entity *e = &game.allEntities[game.selUnit];
  e->x = localX;
  e->y = localY;
  e->chunkAddr = chunk;
  e->isBlinking = 0;

  game.selUnit = -1;
  needRedraw = 1;
}


void moveCursor(int dx, int dy) {
  if (dx > 0 && game.cursorX < (LIMIT_MAX_CHUNKS << 4) - 1) game.cursorX++;
  if (dx < 0 && game.cursorX > 0) game.cursorX--;
  if (dy > 0 && game.cursorY < (LIMIT_MAX_CHUNKS << 4) - 1) game.cursorY++;
  if (dy < 0 && game.cursorY > 0) game.cursorY--;

  if (game.cursorX >= game.viewportX + SIZE_X_VIEWPORT) 
      game.viewportX = game.cursorX - (SIZE_X_VIEWPORT - 1);
  
  if (game.cursorY >= game.viewportY + SIZE_Y_VIEWPORT) 
      game.viewportY = game.cursorY - (SIZE_Y_VIEWPORT - 1);

  if (game.cursorX < game.viewportX) 
      game.viewportX = game.cursorX;
  
  if (game.cursorY < game.viewportY) 
      game.viewportY = game.cursorY;
}



void spawnEntity(EntityTypes type, uint16_t x, uint16_t y, uint8_t owner) {
  
  Entity *e = findAvailableEntity(x, y);
  uint8_t c = findAvailableChunk_INDEX(x >> 4, y >> 4);

  if (!e || e->type != ENTITY_NULL)
    return;

  if (c == 0xFF)
    return;

  memset(e, 0, sizeof (Entity));

  // I have not HP array, will add later
  e->hp = 15;
  e->type = type;
  e->chunkAddr = c;
  e->x = x & 0x0F;
  e->y = y & 0x0F;
  e->owner = owner;

  // // DEBUG
  // e->isVeteran = 1;
  
  needRedraw = 1;

  return;
}


void prepareAsciiWars() {
  memset(&game, 0, sizeof (game));
  memset(&game.lazyChunks, DISABLED_CHUNK_X, sizeof (game.lazyChunks));

  game.player.goldCount = 100;
  game.selUnit = 0xFF;
}



int main(void) {
  prepareAsciiWars();
  enableRawMode();

  spawnEntity(BLD_BASE, 5, 7, ARDUINO + 1);
  spawnEntity(BLD_BASE, 5, 6, ARDUINO + 2);
  spawnEntity(BLD_BASE, 5, 5, ARDUINO + 3);
  
  printf("\x1b[H\x1b[2J");
  drawGrid();

  redrawViewport(game.viewportX, game.viewportY);

  char c;
  int8_t x, y;

  while (1) {
    x = 0;
    y = 0;

    GET_INPUT(&c);

    if (c == 3 || c == 4)
      break;

    if (c == '\x1b') {
      if (read(STDIN_FILENO, &c, 1) == 0) continue;
      if (read(STDIN_FILENO, &c, 1) == 0) continue;
    }

    // Moving
    if (c == 'w' || c == 'k' || c == 'A')
      y = -1;
    else if (c == 's' || c == 'j' || c == 'B')
      y = 1;
    else if (c == 'a' || c == 'h' || c == 'D')
      x = -1;
    else if (c == 'd' || c == 'l' || c == 'C')
      x = 1;
    
    else if (c == ' ')
     spawnEntity(UNIT_TANK, game.cursorX, game.cursorY, ARDUINO + 1);

    else if (c == '\t')
      spawnEntity(MINER_GOLD, game.cursorX, game.cursorY, ARDUINO + 1);

    else if (c == 'o') {
      if (game.selUnit == 0xFF) {
        selectUnit();
      } else {
        unselectUnit();
      }
    }

    moveCursor(x, y);

    needRedraw |= (x != 0) || (y != 0);
    
    if (needRedraw) {
      redrawViewport(game.viewportX, game.viewportY);
      fflush(stdout);
      needRedraw = 0;
    }
  }

  disableRawMode();
  return 0;
}

