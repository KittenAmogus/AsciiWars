#include "utils/utils.h"


extern GameData game;


Chunk *findAvailableChunk(uint8_t x, uint8_t y) {
  Chunk *chunk = 0;
  Chunk *tempChunk;

  for (uint8_t chunkId=0; chunkId<LIMIT_MAX_CHUNKS; chunkId++) {
    tempChunk = &game.lazyChunks[chunkId];

    if (tempChunk->x == x && tempChunk->y == y) {
      chunk = tempChunk;
      break;
    }

    if (tempChunk->x == DISABLED_CHUNK_X && tempChunk->y == DISABLED_CHUNK_Y)
      chunk = tempChunk;
  }

  return chunk;
}

uint8_t findAvailableChunk_INDEX(uint8_t x, uint8_t y) {
  uint8_t chunk = 0xFF;
  Chunk *tempChunk;

  for (uint8_t chunkId=0; chunkId<LIMIT_MAX_CHUNKS; chunkId++) {
    tempChunk = &game.lazyChunks[chunkId];

    if (tempChunk->x == x && tempChunk->y == y) {
      chunk = chunkId;
      break;
    }

    if (tempChunk->x == DISABLED_CHUNK_X && tempChunk->y == DISABLED_CHUNK_Y) {
      tempChunk->x = x;
      tempChunk->y = y;
      chunk = chunkId;
      break;
    }
  }

  return chunk;
}

Entity *findAvailableEntity(uint16_t x, uint16_t y) {
  Entity *entity = 0;
  Entity *tempEntity;

  Chunk *chunk = findAvailableChunk((x >> 4), (y >> 4));
  if (!chunk)
    return entity;

  for (uint8_t entityId=0; entityId<LIMIT_MAX_ENTITIES; entityId++) {
    tempEntity = &game.allEntities[entityId];

    if (tempEntity->type == ENTITY_NULL)
      entity = tempEntity;
  }

  return entity;
}

Entity *getEntityAt(uint16_t x, uint16_t y) {
  Entity *entity = 0;
  Chunk *chunk = 0;

  uint8_t localX, localY, chunkX, chunkY;

  localX = x & 0x0F;
  localY = y & 0x0F;
  chunkX = x >> 4;
  chunkY = y >> 4;

  for (uint8_t entityId=0; entityId<LIMIT_MAX_ENTITIES; entityId++) {
    entity = &game.allEntities[entityId];

    if (entity->x == localX && entity->y == localY) {
      chunk = &game.lazyChunks[entity->chunkAddr];
      
      if (chunk->x == chunkX && chunk->y == chunkY)
        return entity;
    }
  }

  return (void*)0;
}

