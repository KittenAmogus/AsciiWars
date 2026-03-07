#include "utils/utils.h"
#include "utils/config.h"
#include "utils/memory.h"

#include <string.h>

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

UnitCount getUnitsAt(uint16_t x, uint16_t y) {
  Entity *entity = 0;
  Chunk *chunk = 0;

  uint8_t localX, localY, chunkX, chunkY;

  localX = x & 0x0F;
  localY = y & 0x0F;
  chunkX = x >> 4;
  chunkY = y >> 4;

  UnitCount uc = {0, 0, 0, 0, 0};

  for (uint8_t entityId=0; entityId<LIMIT_MAX_ENTITIES; entityId++) {
    entity = &game.allEntities[entityId];

    if (entity->x == localX && entity->y == localY) {
      chunk = &game.lazyChunks[entity->chunkAddr];
      
      if (chunk->x == chunkX && chunk->y == chunkY) {
        switch (entity->type) {
          case UNIT_SOLDIER:
            uc.soldiers++;
            break;

          case UNIT_DRONE:
            uc.drones++;
            break;

          case UNIT_TANK:
            uc.tanks++;
            break;

          case UNIT_LANDMINE:
            uc.landmine = 1;
            break;
        }

        uc.owner = entity->owner;

      }
    }
  }

  return uc;
}

uint8_t getUnitCountAt(uint16_t x, uint16_t y) {
  UnitCount uc = getUnitsAt(x, y);
  //                                          Do not step on it
  return uc.soldiers + uc.drones + uc.tanks + (uc.landmine << 2);
}

uint8_t applyDamage(Entity *e, uint8_t damage) {
  if (e && e->type != ENTITY_NULL) {
    e->hp -= (e->hp >= damage ? damage : e->hp);
  }
  return e->hp == 0;
}

uint8_t chargeResources(ResourceCount res) {
  if (game.player.goldCount < res.gold ||
      game.player.uraniumCount < res.uranium) return 0;
  game.player.goldCount -= res.gold;
  game.player.uraniumCount -= res.uranium;
  return 1;
}

void selectUnit() {
  uint8_t localX = game.cursorX & 0x0F;
  uint8_t localY = game.cursorY & 0x0F;

  uint8_t chunkX = game.cursorX >> 4;
  uint8_t chunkY = game.cursorY >> 4;

  for (uint8_t i=0; i<LIMIT_MAX_ENTITIES; i++) {
    Entity *e = &game.allEntities[i];
    
    if (e->type == ENTITY_NULL || e->owner != game.playerTurn)
      continue;

    if (e->x == localX && e->y == localY) {
      Chunk c = game.lazyChunks[e->chunkAddr];
      if (c.x != chunkX || c.y != chunkY) {
        continue;
      }
    } else {
      continue;
    }
 
    e->isBlinking = 1;
    game.selUnit = i;
    break;
  }

  game.needRedraw = 1;
}

uint8_t unselectUnit() {
  uint8_t localX, localY; localX = game.cursorX & 0x0F;
  localY = game.cursorY & 0x0F;

  uint8_t chunk = findAvailableChunk_INDEX(game.cursorX >> 4, game.cursorY >> 4);
  if (chunk == 0xFF)
    return 2;

  Entity *e = &game.allEntities[game.selUnit];
  
  e->isBlinking = 0;
  game.selUnit = 0xFF;
  
  if (e->type >= BLD_BASE) return 0;

  Entity *e2;

  // If target exists and it is enemy's unit
  while (1) {
    
    e2 = getEntityAt(game.cursorX, game.cursorY);
    if (!e2 || e2 == e || e2->type == ENTITY_NULL) break;
    if (e2->owner == e->owner) break;

    uint8_t damage = UNIT_DAMAGE[e->type] + (e->isVeteran << 1);
    if (!applyDamage(e2, damage)) return 1;

    if (e2->type < BLD_BASE)
      memset(e2, 0, sizeof (*e2));

    else {
      e2->hp = damage;
      e2->owner = e->owner;
      return 1;
    }

  }

  uint8_t cnt = getUnitCountAt(game.cursorX, game.cursorY);

  if (cnt < 3) {

    e->x = localX;
    e->y = localY;
    e->chunkAddr = chunk;

    game.selUnit = -1;
    game.needRedraw = 1;

    return 0;
  }

  return 1;
}

void moveCursor(int dx, int dy) {
  if (dx > 0 && game.cursorX < (LIMIT_MAX_CHUNKS << 4) - 1) game.cursorX++;
  if (dx < 0 && game.cursorX > 0) game.cursorX--;
  if (dy > 0 && game.cursorY < (LIMIT_MAX_CHUNKS << 4) - 1) game.cursorY++;
  if (dy < 0 && game.cursorY > 0) game.cursorY--;

  if (game.cursorX >= game.viewportX + SIZE_X_VIEWPORT) 
      game.viewportX = game.cursorX - (SIZE_X_VIEWPORT - 1);
  
  if (game.cursorY >= game.viewportY + SIZE_Y_VIEWPORT) 
      game.viewportY = game.cursorY - (SIZE_Y_VIEWPORT - 1);

  if (game.cursorX < game.viewportX) 
      game.viewportX = game.cursorX;
  
  if (game.cursorY < game.viewportY) 
      game.viewportY = game.cursorY;
}



void spawnEntity(EntityTypes type, uint16_t x, uint16_t y, uint8_t owner) {
  
  Entity *e = findAvailableEntity(x, y);
  uint8_t c = findAvailableChunk_INDEX(x >> 4, y >> 4);

  if (!e || e->type != ENTITY_NULL)
    return;

  if (c == 0xFF)
    return;

  memset(e, 0, sizeof (Entity));

  // I have not HP array, will add later
  e->hp = ENTITY_HPS[type];
  e->type = type;
  e->chunkAddr = c;
  e->x = x & 0x0F;
  e->y = y & 0x0F;
  e->owner = owner;

  // // DEBUG
  // e->isVeteran = 1;
  
  game.needRedraw = 1;

  return;
}

void processIncome(void) {
  game.player.goldCount += 2;  // TODO Change to BASIC_INCOME

  for (uint8_t i=0; i<LIMIT_MAX_ENTITIES; i++) {
    Entity *e = &game.allEntities[i];
    
    if (e->type == ENTITY_NULL || e->hp == 0 || e->owner != game.player.id)
      continue;

    // isVeteran = isRich for miners
    if (e->type == MINER_GOLD)
      game.player.goldCount += 1 + (e->isVeteran);
    if (e->type == MINER_URANIUM)
      game.player.uraniumCount += 1 + (e->isVeteran);

    // Army supply
    if (e->type < BLD_BASE && !e->isVeteran) {
      ResourceCount cnt = (ResourceCount){
        (e->type == UNIT_TANK) + (e->type != UNIT_LANDMINE)
      };
      if (!chargeResources(cnt))
        e->hp -= 1 + (e->hp > 1);

      // No need to check again BLD_BASE
      if (e->hp == 0)
        e->type = ENTITY_NULL;
    }

    // TODO Add 'repair' -> heal only if soldier inside
    if (e->hp < ENTITY_HPS[e->type] &&
        e->hp && game.player.goldCount > 0) e->hp++;
  }
}
