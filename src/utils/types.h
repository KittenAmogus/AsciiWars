#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef struct {
  // Just position from (0, 0)
  uint16_t x : 8;
  uint16_t y : 8;
} __attribute__((packed)) Chunk;

typedef struct {
  // Offset from chunk X, Y
  uint16_t x : 4;
  uint16_t y : 4;

  uint16_t direction : 2; // just 4 directions
  uint16_t type : 2;      // soldat, drone, tank, <maybe i will add new type>
  uint16_t owner : 1;     // isPC's
  uint16_t hp : 3;        // 0 - 4

  uint8_t chunkAddr;
} __attribute__((packed)) Unit;

typedef struct {
  // Offset from chunk X, Y
  uint16_t x : 4;
  uint16_t y : 4;

  uint16_t type : 3;  // 0 - 7
  uint16_t hp : 4;    // 0-15
  uint16_t owner : 1; // isPC's

  uint8_t chunkAddr;
} __attribute__((packed)) Building;

typedef struct {
  // Offset from chunk X, Y
  uint16_t x : 4;
  uint16_t y : 4;
  uint16_t resources : 5; // 0-63
  uint16_t isRich : 1;
  uint16_t isMined : 1;
  uint16_t type : 1; // Gold/Uranium
  uint8_t chunkAddr;
} __attribute__((packed)) Vein;

typedef struct {
  // 0 - 1023
  uint32_t goldCount : 10;
  uint32_t uraniumCount : 10;

  uint32_t wasFarmPlaced : 1;
  uint32_t bonusApCount : 3;

  uint32_t unused : 8;
} __attribute__((packed)) Player;

typedef enum {
  DIR_UP = 0b00,
  DIR_DOWN = 0b01,
  DIR_LEFT = 0b10,
  DIR_RIGHT = 0b11
} Directions;

typedef enum {
  UNIT_SOLDIER = 0b00,
  UNIT_DRONE = 0b01,
  UNIT_TANK = 0b10,
  UNIT_LANDMINE = 0b11
  // Will add it like static drone, not visible by enemy
} UnitTypes;

typedef enum {
  BLD_NULL,

  BLD_BASE,
  BLD_FARM,
  BLD_FACTORY,
  BLD_MINER,
  BLD_BARRACKS,
  BLD_LAB,
  BLD_SILO
} BuildingTypes;

#endif // TYPES_H
