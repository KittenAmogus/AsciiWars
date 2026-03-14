#ifndef MEMORY_H
#define MEMORY_H

#include "config.h"
#include "types/types.h"

#ifdef SERVER

typedef struct {
  PlayerData allPlayers[LIMIT_PLAYERS];
  Entity allEntities[LIMIT_ENTITIES];
  NukeTimer allNukes[LIMIT_NUKES];

  uint16_t gameTurn;
} ServerData;

#endif

typedef struct {
  CacheDraw viewportCache[SIZE_Y_VIEWPORT][SIZE_X_VIEWPORT];
  CacheCell selectedCell;
  uint16_t gameTurn;

  // Cursor moving
  uint16_t cursorX : 5;
  uint16_t cursorY : 5;
  uint16_t moveSpeedPower : 2; // Delay: -, 1000, 500, 200
} GameData;

#endif // MEMORY_H
