#include "utils/config.h"
#include "utils/types.h"

#ifndef MEMORY_H
#define MEMORY_H

typedef enum {
  PLAYER_NULL = 0,
  PLAYER_SERVER = 1,
  PLAYER_PLAYER = 2,
  PLAYER_AI = 3
} PlayerID;

typedef struct {

  // Player, entities
  Entity allEntities[LIMIT_MAX_ENTITIES];
  Player player; // PC(Server) Arduino(Player) AI(Bot)

  // Cache
  EntityInCell viewportCache[SIZE_Y_VIEWPORT][SIZE_X_VIEWPORT];
  Chunk lazyChunks[LIMIT_MAX_CHUNKS];

  uint8_t playerTurn;

  uint16_t viewportX, viewportY;
  uint16_t cursorX, cursorY;

  uint8_t selUnit;
} __attribute__((packed)) GameData;

// Game, only for server
typedef struct {

  Player allPlayers[3];
  NukeTimer launchedNukes[LIMIT_MAX_NUKES];

  uint16_t gameTurn;

  // TODO INCOMPLETE
  // I will complete it when i will do server parts
} __attribute__((packed)) ServerData;

#endif // MEMORY_H
