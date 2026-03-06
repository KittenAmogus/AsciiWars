#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef struct {
  // Coordinates 2 byte
  uint16_t x : 4;
  uint16_t y : 4;
  uint16_t chunkAddr : 8;

  // Data 1 byte
  uint8_t timer : 3; // Timer to explosion
  uint8_t power : 3; // Different powers
  uint8_t owner : 2; // For statistics
} __attribute__((packed)) NukeTimer;

typedef struct {
  uint16_t type : 4;
  uint16_t owner : 2;
  uint16_t isBlinking : 1;
  uint16_t isVeteran : 1;

  uint16_t soldierCount : 2;
  uint16_t tankCount : 2;
  uint16_t droneCount : 2;
  uint16_t hasLandMine : 1;
  uint16_t unused : 1;
} __attribute__((packed)) EntityInCell;
// 1 Byte

typedef struct {

  uint16_t x : 8;
  uint16_t y : 8;

} __attribute__((packed)) Chunk;
// 2 Byte

typedef struct {

  // Coordinates 2 byte
  uint32_t x : 4;
  uint32_t y : 4;
  uint32_t chunkAddr : 8;

  // Data 2 byte
  uint32_t type : 4;       // 0 - 15
  uint32_t hp : 4;         // 0 - 15
  uint32_t ap : 3;         // Bonus for LABs
  uint32_t owner : 2;      // NULL, 3 Players
  uint32_t isVisible : 1;  // Can enemy see it
  uint32_t isVeteran : 1;  // Killed tank
  uint32_t isBlinking : 1; // Targeted by NUKE or enemy inside

} __attribute__((packed)) Entity;
// 4 Byte

typedef struct {

  // 0 - 1023
  uint32_t goldCount : 10;
  uint32_t uraniumCount : 10;

  uint32_t wasFarmPlaced : 1;
  uint32_t bonusApCount : 3;

  uint32_t unused : 8;

} __attribute__((packed)) Player;
// 4 Byte

// Maximum 15 types + NULL
typedef enum {

  // NULL Type
  ENTITY_NULL = 0,

  // Units
  UNIT_SOLDIER,
  UNIT_DRONE,
  UNIT_TANK,
  UNIT_LANDMINE,

  // Buildings
  BLD_BASE,
  BLD_FARM,
  BLD_FACTORY,
  BLD_MINER,
  BLD_BARRACKS,
  BLD_LAB,
  BLD_SILO,

  // Resources
  VEIN_GOLD,
  VEIN_URANIUM,

  // No recalculate mining resource every frane
  MINER_GOLD,
  MINER_URANIUM

} EntityTypes;

#endif // TYPES_H
