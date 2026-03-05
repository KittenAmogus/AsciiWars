#include "draw.h"
#include "utils/memory.h"
#include "utils/config.h"

#include <string.h>

#include <stdio.h>


extern GameData game;


/*
BuildingTypes getBType(uint16_t x, uint16_t y) {
  for (uint8_t i=0; i<LIMIT_MAX_BUILDINGS; i++) {
    Building bld = game.allBuildings[i];

    if (bld.hp == 0 || bld.type == BLD_NULL)
      continue;

    Chunk chunk = game.loadedChunks[bld.chunkAddr];

    uint8_t worldX, worldY;
    worldX = (chunk.x << 4) + bld.x;
    worldY = (chunk.y << 4) + bld.y;

    if (worldX == x && worldY == y)
      return bld.type;
  }
  return BLD_NULL;
}
*/

const char BLD_CHARS[8] = {
  '.', 'W', 'f', 'A', '\0', 'R', 'L', 'I'
};

const char UNIT_CHARS[4] = {
  'i', '^', 't', '='
};


void updateViewPortCache() {
  memset(&game.unitInCells, 0, sizeof (game.unitInCells));

  for (uint8_t i=0; i<LIMIT_MAX_UNITS; i++) {
    Unit u = game.allUnits[i];

    if (u.hp == 0)
      continue;

    Chunk uc = game.loadedChunks[u.chunkAddr];

    uint16_t globalX, globalY;
    globalX = (uc.x << 4) + u.x;
    globalY = (uc.y << 4) + u.y;

    // If is not in viewport
    if ((globalX >= game.viewPortX + SIZE_X_VIEWPORT || globalX < game.viewPortX ) ||
        (globalY >= game.viewPortY + SIZE_Y_VIEWPORT || globalY < game.viewPortY))
      continue;

    // Position in viewport
    uint8_t localX, localY;
    localX = globalX - game.viewPortX;
    localY = globalY - game.viewPortY;

    UnitCount *ucnt = &game.unitInCells[localY][localX];

    // Increase values
    ucnt->soldierCount += (u.type == UNIT_SOLDIER);
    ucnt->hasLandMine = (u.type == UNIT_LANDMINE);
    ucnt->droneCount += (u.type == UNIT_DRONE);
    ucnt->tankCount += (u.type == UNIT_TANK);
    ucnt->owner = u.owner;
  }
}


void drawGrid() {
  printf("+");
  for (uint8_t i=0; i<SIZE_X_VIEWPORT; i++)
    printf("---+");
  printf("\n\r");

  for (uint8_t y=0; y<SIZE_Y_VIEWPORT; y++) {
    printf("|");

    for (uint8_t x=0; x<SIZE_X_VIEWPORT; x++) {
      printf("   |");
    }
    printf("\n\r");

  }
  printf("+");
  for (uint8_t i=0; i<SIZE_X_VIEWPORT; i++)
    printf("---+");
  printf("\n\r");
}


void redrawCell(uint16_t x, uint16_t y) {
  uint8_t localX, localY, drawX, drawY;
  localX = (uint8_t)(x - game.viewPortX);
  localY = (uint8_t)(y - game.viewPortY);

  drawX = 1 + 1 + (localX << 2);
  drawY = 1 + 1 + (localY);

  UnitCount unitCount = game.unitInCells[localY][localX];
  BuildingDrawData bld = game.bldInCells[localY][localX];

  // Drawing
  printf("\x1b[%d;%dH", drawY, drawX);

  char buff[4] = "   ";

  // Units
  if (unitCount.hasLandMine) {
    buff[0] = ' ';
    buff[2] = ' ';
    buff[1] = UNIT_CHARS[UNIT_LANDMINE];

  }
  else {

    uint8_t uoff = 0;

    for (uint8_t i=uoff; i<unitCount.droneCount; i++) {
      buff[i] = UNIT_CHARS[UNIT_DRONE];
      uoff+= (uoff < 3);
    }
    for (uint8_t i=uoff; i<unitCount.tankCount; i++) {
      buff[i] = UNIT_CHARS[UNIT_TANK];
      uoff+= (uoff < 3);
    }
    for (uint8_t i=uoff; i<unitCount.soldierCount; i++) {
      buff[i] = UNIT_CHARS[UNIT_SOLDIER];
      uoff+= (uoff < 3);
    }

  }

  // Building
  if (bld.type != BLD_NULL) {
    buff[1] = BLD_CHARS[bld.type];
    if (bld.type != BLD_MINER) {
      buff[0] = '[';
      buff[2] = ']';
    }
    else {
      buff[0] = '(';
      buff[1] = 'G';  // TODO Change to vein type
      buff[2] = ')';
    }
  }

  if (x == game.cursorX && y == game.cursorY)
    printf("\x1b[7m");

  printf("%s", buff);

  printf("\x1b[0m");
}

void redrawViewport(uint16_t cx, uint16_t cy) {
  updateViewPortCache();

  for (uint16_t y=cy; y<cy + SIZE_Y_VIEWPORT; y++) {
    for (uint16_t x=cx; x<cx + SIZE_X_VIEWPORT; x++) {
      redrawCell(x, y);
    }
  }
  printf("\x1b[%d;1H\n\r", (SIZE_Y_VIEWPORT) + 3);
}

