#include "draw.h"
#include "utils/memory.h"
#include "utils/config.h"
#include "utils/utils.h"

#include <string.h>

#include <stdio.h>

typedef struct {
  uint8_t x: 4;  // 0-15
  uint8_t y: 4;  // 0-15
} __attribute__((packed)) ViewportCoords;

extern GameData game;

const char ENTITY_CHARS[16] = {
  '!',  // ERROR if NULL drawed

  // Units
  'i', '^', 'T', '=',

  // Buildings
  'W', 'F', 'F', '_', 'B', 'L', 'I',

  // Resources (Vein x2, Miner x2)
  'G', 'U', 'G', 'U'
};

const uint8_t BG_OWNER_COLORS[4] = {
  0, 101, 104, 102
};

const uint8_t FG_OWNER_COLORS[4] = {
  0, 90, 90, 90
};

void updateViewPortCache() {
  // Reset viewport buffer
  // TODO Add dirty flags and NOT clear cache

  memset(&game.viewportCache, 0, sizeof(game.viewportCache));

  Entity e;
  EntityInCell *data;

  uint8_t localX, localY;
  uint16_t globalX, globalY;

  for (uint8_t enitityId=0; enitityId<LIMIT_MAX_ENTITIES; enitityId++) {
    e = game.allEntities[enitityId];

    if (e.type == ENTITY_NULL)
      continue;

    Chunk c = game.lazyChunks[e.chunkAddr];
    globalX = (c.x << 4) + e.x;
    globalY = (c.y << 4) + e.y;

    if (globalX < game.viewportX ||
        globalX >= game.viewportX + SIZE_X_VIEWPORT) continue;
    if (globalY < game.viewportY ||
        globalY >= game.viewportY + SIZE_Y_VIEWPORT) continue;

    localX = (globalX - game.viewportX);
    localY = (globalY - game.viewportY);

    data = &game.viewportCache[localY][localX];
  
    data->soldierCount += (e.type == UNIT_SOLDIER);
    data->tankCount += (e.type == UNIT_TANK);
    data->droneCount += (e.type == UNIT_DRONE);
    data->hasLandMine = (e.type == UNIT_LANDMINE);
    data->owner = e.owner;

    data->isVeteran |= (e.isVeteran);
    data->isBlinking |= e.isBlinking;

    if (e.type >= BLD_BASE) data->type = e.type;
  }
}


void drawGrid() {
  printf("+");
  for (uint8_t i=0; i<SIZE_X_VIEWPORT; i++)
    printf("---+");
  printf("\n\r");

  for (uint8_t y=0; y<SIZE_Y_VIEWPORT; y++) {
    printf("|");

    for (uint8_t x=0; x<SIZE_X_VIEWPORT; x++) {
      printf("   |");
    }
    printf("\n\r");

  }
  printf("+");
  for (uint8_t i=0; i<SIZE_X_VIEWPORT; i++)
    printf("---+");
  printf("\n\r");
}


void redrawCell(uint16_t globalX, uint16_t globalY) {
  uint8_t x, y;
  x = globalX - game.viewportX;
  y = globalY - game.viewportY;

  EntityInCell cache = game.viewportCache[y][x];
  char buff[4] = "   ";

  if (cache.type < BLD_BASE) {

    // Add units
    uint8_t uoff=0;

    while (uoff < 3) {
      if (cache.tankCount > 0) {
        buff[uoff++] = ENTITY_CHARS[UNIT_TANK];
        cache.tankCount--;
      } else if (cache.soldierCount > 0) {
        buff[uoff++] = ENTITY_CHARS[UNIT_SOLDIER];
        cache.soldierCount--;
      } else if (cache.droneCount > 0) {
        buff[uoff++] = ENTITY_CHARS[UNIT_DRONE];
        cache.droneCount--;
      } else break;
    }

  } else {

    if (cache.type < VEIN_GOLD) {
      buff[0] = '[';
      buff[2] = ']';
    } else if (cache.type < MINER_URANIUM) {
      buff[0] = '(';
      buff[2] = ')';
    }

    buff[1] = ENTITY_CHARS[cache.type];
  }

  printf("\x1b[%d;%dH", 1 + 1 + (y), 1 + 1 + (x << 2));

  printf("\x1b[%d;%dm",
      BG_OWNER_COLORS[cache.owner],
      FG_OWNER_COLORS[cache.owner]
  );;

  if (cache.isBlinking)
    printf("\x1b[5m");
  else if (cache.isVeteran)
    printf("\x1b[93m");

  if (globalX == game.cursorX && globalY == game.cursorY) {
    printf("\x1b[7m");

    if (game.selUnit != 0xFF) {
      Entity se = game.allEntities[game.selUnit];
      buff[1] = ENTITY_CHARS[se.type];
    }
  }

  printf("%s", buff);
  printf("\x1b[0m");
}

void redrawViewport(uint16_t cx, uint16_t cy) {
  // TODO Add dirty flags
  updateViewPortCache();

  for (uint16_t y=cy; y<cy + SIZE_Y_VIEWPORT; y++) {
    for (uint16_t x=cx; x<cx + SIZE_X_VIEWPORT; x++) {
      redrawCell(x, y);
    }
  }
  printf("\x1b[%d;1H\n\r", (SIZE_Y_VIEWPORT) + 3);
}

