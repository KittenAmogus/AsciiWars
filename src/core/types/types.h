#ifndef TYPES_H
#define TYPES_H

#ifndef ARDUINO
#include <stdint.h>
#endif

#define PACKED __attribute__((packed))

// Structs
typedef struct {
  uint16_t x : 5;
  uint16_t y : 5;
  uint16_t type : 4;
  uint16_t isVet : 1; // bonus to UNIT: damage,healing MINER/VEIN: resource/turn
  uint16_t isVis : 1; // UNIT/BUILDING: is visible on viewport, VEIN: isMining

  union {
    uint8_t resource;

    struct {
      uint8_t hp : 4;
      uint8_t owner : 2;
      uint8_t target : 1;
      uint8_t select : 1;
    };
  };
} PACKED Entity;

typedef struct {
  uint32_t id : 2;
  uint32_t ap : 4;
  uint32_t gold : 10;
  uint32_t uranium : 7;
  uint32_t eliteBonuces : 3; // Set isVet to random unit, = uraniumOverflow << 1
  uint32_t unitCap : 6;
} PACKED PlayerData;

typedef struct {
  uint16_t x : 5;
  uint16_t y : 5;
  uint16_t size : 2;
  uint16_t timer : 2;
  uint16_t owner : 2; // For statistick
} PACKED NukeTimer;

// Cache
typedef struct {
  union {
    struct {
      uint16_t isBuilding : 1;

      uint16_t type : 4;
      uint16_t owner : 2;
      uint16_t isVet : 1;
      uint16_t isBlinking : 1;
      uint16_t unused : 3;
    } bld;

    struct {
      uint16_t isBuilding : 1;

      uint16_t engineers : 2;
      uint16_t soldiers : 2;
      uint16_t drones : 2;
      uint16_t tanks : 2;
      uint16_t owner : 2;

      uint16_t blinkMask : 2; // -/0/1/2
      uint16_t vetMask : 3;
    } unit;
  };
} PACKED CacheDraw;

typedef struct {
  uint16_t soldiers : 2;
  uint16_t engineers : 2;
  uint16_t tanks : 2;
  uint16_t drones : 2;
  uint16_t owner : 2;
  uint16_t bldType : 4;
} PACKED CacheCell;

// Enums
typedef enum {
  ENTITY_NULL = 0,

  // Units
  UNIT_ENGINEER,
  UNIT_SOLDIER,
  UNIT_TANK,
  UNIT_DRONE,
  UNIT_LANDMINE,

  // Buildings
  BLD_BASE,
  BLD_FARM,
  BLD_FACTORY,
  BLD_BARRACKS,
  BLD_SILO,

  // Miners
  MINER_EMTPY,
  MINER_GOLD,
  MINER_URANIUM,

  // Resources
  VEIN_GOLD,
  VEIN_URANIUM
} EntityType;

typedef enum {
  PLAYER_SERVER = 0,
  PLAYER_1 = 1,
  PLAYER_2 = 2,
  PLAYER_AI = 3
} PlayerId;

#endif // TYPES_H
