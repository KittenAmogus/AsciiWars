
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

UnitCount getUnitsAt(uint16_t x, uint16_t y);
uint8_t getUnitCountAt(uint16_t x, uint16_t y);

uint8_t applyDamage(Entity *e, uint8_t damage);
uint8_t chargeResources(ResourceCount res);

void selectUnit();
uint8_t unselectUnit();

void moveCursor(int dx, int dy);
void spawnEntity(EntityTypes type, uint16_t x, uint16_t y, uint8_t owner);

void processIncome(void);

#endif // UTILS_H
