#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
typedef struct { // Just position from (0, 0)
  uint16_t x : 8;
  uint16_t y : 8;
  // uint16_t revealed : 1;  // Use (x==0 && y==0)
} __attribute__((packed)) Chunk;
// 2 Byte

typedef struct {
  // Offset from chunk X, Y
  uint16_t x : 4;
  uint16_t y : 4;

  uint16_t type : 2;  // soldat, drone, tank, <maybe i will add new type>
  uint16_t owner : 1; // isPC's
  uint16_t hp : 5;    // 0 - 15
  // uint16_t unused; <- Idk, why I did this lol, i just realised this is so
  // stupid

  uint8_t chunkAddr;
} __attribute__((packed)) Unit;
// 3 Byte

typedef struct {
  // Offset from chunk X, Y
  uint16_t x : 4;
  uint16_t y : 4;

  uint16_t type : 3;  // 0 - 7
  uint16_t hp : 4;    // 0-15
  uint16_t owner : 1; // isPC's

  uint8_t chunkAddr;
} __attribute__((packed)) Building;
// 3 Byte

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
// 3 Byte

typedef struct {
  // 0 - 1023
  uint32_t goldCount : 10;
  uint32_t uraniumCount : 10;

  uint32_t wasFarmPlaced : 1;
  uint32_t bonusApCount : 3;

  uint32_t unused : 8;
} __attribute__((packed)) Player;
// 4 Byte

/* typedef enum {
  DIR_UP = 0b00,
  DIR_DOWN = 0b01,
  DIR_LEFT = 0b10,
  DIR_RIGHT = 0b11
} Directions; */

typedef enum {
  UNIT_SOLDIER = 0b00,
  UNIT_DRONE = 0b01,
  UNIT_TANK = 0b10,
  UNIT_LANDMINE = 0b11
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
