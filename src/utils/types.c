#include "utils/types.h"

const ResourceCount ENTITY_COSTS[16] = {
    // DO NOT TOUCH MY ARRAYS, CLANGD!
    [ENTITY_NULL] = {0, 0},

    // Units
    [UNIT_SOLDIER] = {10, 0},
    [UNIT_DRONE] = {15, 5},
    [UNIT_TANK] = {40, 15},
    [UNIT_LANDMINE] = {5, 2},

    // Buildings
    [BLD_BASE] = {100, 50},
    [BLD_FARM] = {20, 0},
    [BLD_FACTORY] = {50, 10},
    [BLD_MINER] = {20, 0},
    [BLD_BARRACKS] = {30, 0},
    [BLD_LAB] = {60, 20},
    [BLD_SILO] = {80, 40},

    // Resources
    [VEIN_GOLD] = {0, 0},
    [VEIN_URANIUM] = {0, 0},

    [MINER_GOLD] = {20, 0},
    [MINER_URANIUM] = {20, 0}
    // DO NOT TOUCH IT CLANGD
};

const uint8_t ENTITY_HPS[16] = {
  // CLANG DO NOT TOUCH THIS
  0,

  7,  1, 15, 1,

  15, 7, 15, 15, 15, 15, 15,

  0,  0, 15, 15
};


const uint8_t UNIT_DAMAGE[5] = {
  0,
  2,
  15,
  7,
  15
};



