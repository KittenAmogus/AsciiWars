#include "utils/config.h"
#include "utils/memory.h"
#include "utils/types.h"

#ifndef UTILS_H
#define UTILS_H

// Find free slot/same values
// DRY failed maybe?..
Chunk *findAvailableChunk(uint8_t x, uint8_t y);
Entity *findAvailableEntity(uint16_t x, uint16_t y);

uint8_t findAvailableChunk_INDEX(uint8_t x, uint8_t y);

// Get entity from global cords
Entity *getEntityAt(uint16_t x, uint16_t y);

#endif // UTILS_H
