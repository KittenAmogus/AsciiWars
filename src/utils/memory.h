#include "utils/config.h"
#include "utils/types.h"

#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
  uint8_t droneCount : 2;
  uint8_t tankCount : 2;
  uint8_t soldierCount : 2;
  uint8_t hasLandMine : 1;
  uint8_t owner : 1;
  // Landmine still can be placed only once
} __attribute__((packed)) UnitCount;
// 1 Byte

typedef struct {
  uint8_t type : 3;
  uint8_t hp : 4;
  uint8_t owner : 1;
} __attribute__((packed)) BuildingDrawData;
// 1 Byte

typedef struct {
  // Can be shrinked for other values, min size: 2 bits
  uint8_t timer;

  uint16_t x : 4;
  uint16_t y : 4;
  uint16_t chunkAddr : 8;
} __attribute__((packed)) NukeTimer;
// 3 Byte

typedef struct {
  // Global arrays of objects
  Vein allVeins[LIMIT_MAX_VEINS];
  Unit allUnits[LIMIT_MAX_UNITS];
  Building allBuildings[LIMIT_MAX_BUILDINGS];

  Chunk loadedChunks[LIMIT_MAX_CHUNKS];

  // Positions
  uint16_t cursorX, cursorY;
  uint16_t viewPortX, viewPortY;

  // For new generations
  uint16_t allGoldSum;
  uint16_t allUraniumSum;

  // Players
  Player players[2];
  uint8_t playerTurn;

  // Game

  uint16_t turnCount;
  uint32_t worldSeed;

  // Cache
  UnitCount unitInCells[SIZE_Y_VIEWPORT][SIZE_X_VIEWPORT];
  BuildingDrawData bldInCells[SIZE_Y_VIEWPORT][SIZE_X_VIEWPORT];

  NukeTimer allNukes[LIMIT_MAX_NUKES];

} __attribute__((packed)) GameData;
// LIMIT_MAX_VEIN(16) * 3
// LIMIT_MAX_UNITS(32) * 3
// LIMIT_MAX_BUILDINGS(32) * 3
// LIMIT_MAX_CHUNKS(128) * 1
//
// 4 + 4
//
// 2 + 2
//
// 2 * 4
// 1
//
// 2 + 4
//
// SIZE_X_VIEWPORT * SIZE_Y_VIEWPORT * 1
// Copy this: ^^^
//
// LIMIT_MAX_NUKES(4) * 3
//
// Total:
// 48 + 96 + 96 + 128 + 8 + 4 + 8 + 1 + 6 + (arduino: 8 * 4 * 2) + 4 * 3

#endif // MEMORY_H
