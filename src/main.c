#include "utils/config.h"
#include "utils/memory.h"
#include "utils/types.h"

#include "input/pc_input.h"
#include "draw/draw.h"
#include <stdio.h>

#include <string.h>

#define GET_INPUT(x) read(STDIN_FILENO, x, 1)

GameData game;

uint8_t needRedraw;


void selectUnit() {
  uint8_t localX = game.cursorX & 0x0F;
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
  Entity *e = &game.allEntities[game.selUnit];
  e->x = (game.cursorX & 0xF);
  e->y = (game.cursorY & 0xF);
  e->isBlinking = 0;

  game.selUnit = -1;
  needRedraw = 1;
}


void moveCursor(int x, int y) {
  game.cursorX += x;
  game.cursorY += y;
}


void spawnEntity(EntityTypes type, uint16_t x, uint16_t y, uint8_t owner) {
  uint8_t chunkX, chunkY;
  uint8_t localX, localY;

  // Convert from global coordinates
  chunkX = (x >> 4);
  chunkY = (y >> 4);
  localX = x & 0x0F;
  localY = y & 0x0F;
  
  uint8_t chunkAddr;
  uint8_t freeChunkAddr;
for (chunkAddr = 0; chunkAddr < LIMIT_MAX_CHUNKS; chunkAddr++) { Chunk *c =&game.lazyChunks[chunkAddr];

    // If chunk has not been loaded, save it
    if (c->x == DISABLED_CHUNK_X && c->y == DISABLED_CHUNK_Y) freeChunkAddr = chunkAddr;

    // If already loaded
    else if (c->x == chunkX && c->y == chunkY) break;

    else if (chunkAddr == LIMIT_MAX_CHUNKS - 1)
      return;
  }

  Chunk *c = &game.lazyChunks[chunkAddr];

  if (c->x != chunkX || c->y != chunkY) {
    chunkAddr = freeChunkAddr; // Явно переключаемся на свободный индекс
    game.lazyChunks[chunkAddr].x = chunkX;
    game.lazyChunks[chunkAddr].y = chunkY;
  } // Теперь e->chunkAddr = chunkAddr; запишет верный индекс.


  Entity *e = NULL;

  for (uint8_t entityAddr=0; entityAddr<LIMIT_MAX_ENTITIES; entityAddr++) {
    e = &game.allEntities[entityAddr];
    if (e->type == ENTITY_NULL) break;
  }
  if (e->type != ENTITY_NULL || e == NULL) return;

  memset(e, 0, sizeof (Entity));

  // I have not HP array, will add later
  e->hp = 15;
  e->type = type;
  e->chunkAddr = chunkAddr;
  e->x = localX;
  e->y = localY;
  e->owner = owner;
  
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
      if (read(STDIN_FILENO, &c, 1) == 0) continue; // Пропуск [
      if (read(STDIN_FILENO, &c, 1) == 0) continue; // Читаем A/B/C/D
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
     spawnEntity(UNIT_SOLDIER, game.cursorX, game.cursorY, ARDUINO + 1);

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

