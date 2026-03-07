#include "utils/config.h"
#include "utils/memory.h"
#include "utils/types.h"
#include "utils/utils.h"

// #include "input/pc_input.h"
#include "input/input.h"
#include "draw/draw.h"

#if ARDUINO
#include "input/arduino_input.h"
#else
#include "input/pc_input.h"
#endif

#include <stdio.h>
#include <string.h>

ServerData serv;
GameData game;


void nextTurn(void) {
  serv.gameTurn++;

  // Next player's turn
  uint8_t pc = ((game.playerTurn + 1) & 3);
  pc += pc == 0;

  for (uint8_t p=0; p<3; p++) {
    game.player = serv.allPlayers[p];
    processIncome();
    serv.allPlayers[p] = game.player;
  }

  game.selUnit = 0xFF;
  game.playerTurn = pc;
  game.player = serv.allPlayers[pc - 1];
}


void prepareAsciiWars() {
  memset(&game, 0, sizeof (game));
  memset(&game.lazyChunks, DISABLED_CHUNK_X, sizeof (game.lazyChunks));

  for (uint8_t p=0; p<3; p++) {
    Player pl;
    pl.goldCount = 100;
    pl.id = p + 1;
    serv.allPlayers[p] = pl;
  }

  game.selUnit = 0xFF;
  game.playerTurn = 1;
  game.player = serv.allPlayers[0];
}


void handleInput(InputEvent ev) {
  game.needRedraw = 1;
  switch (ev.type) {
    case INPUT_NONE:
      game.needRedraw = 0;
      break;
    
    case INPUT_MOVE:
      moveCursor(ev.data.moveEvent.dx, ev.data.moveEvent.dy);
      break;

    // ONLY FOR DEBUG
    // TODO Add menu
    case INPUT_CANCEL:
      if (getUnitCountAt(game.cursorX, game.cursorY) >= 3) break;
      if (!chargeResources(ENTITY_COSTS[UNIT_SOLDIER])) break;

      spawnEntity(UNIT_SOLDIER, game.cursorX, game.cursorY, game.player.id);
      break;

    case INPUT_ACTION:
      if (game.selUnit == 0xFF) selectUnit();
      else {
        switch (unselectUnit()) {
          case 0:
            break;

          case 1:
            game.allEntities[game.selUnit].isBlinking = 0;
            game.selUnit = 0xFF;
            // processIncome();
            break;

          case 2:
            break;
        }
      }
      break;

    case INPUT_NEXT_TURN:
      nextTurn();
      break;

    default:
      game.needRedraw = 0;
      break;
  }
}


int main(void) {
  prepareAsciiWars();
  enableRawMode();

  spawnEntity(UNIT_SOLDIER, 5, 4, ARDUINO + 2);
  spawnEntity(BLD_BASE, 5, 7, ARDUINO + 1);
  spawnEntity(BLD_BASE, 5, 6, ARDUINO + 2);
  spawnEntity(BLD_BASE, 5, 5, ARDUINO + 3);
  
  printf("\x1b[H\x1b[2J");
  drawGrid();
  drawHUDGrid();
  redrawViewport(game.viewportX, game.viewportY);

  char c;
  int8_t x, y;

  game.playerTurn = ARDUINO + 1;

  while (1) {
    InputEvent ev = pollEvent();
    if (ev.type == INPUT_QUIT) break;
    handleInput(ev);
    
    if (game.needRedraw) {
      redrawViewport(game.viewportX, game.viewportY);

      // TODO add updateHUD();
      Entity *e = getEntityAt(game.cursorX, game.cursorY);
      if (e && e->type > ENTITY_NULL)
        printf(
          "\x1b[K TURN: %d | PTURN:\x1b[%d;%dm %d \x1b[0m || G: %d | U: %d || Unit\x1b[%d;%dm %c \x1b[0m: HP %d | AP %d ||\n\r",
          serv.gameTurn,

          BG_OWNER_COLORS[game.playerTurn],
          FG_OWNER_COLORS[game.playerTurn],
          game.playerTurn,

          game.player.goldCount,
          game.player.uraniumCount,

          BG_OWNER_COLORS[e->owner],
          FG_OWNER_COLORS[e->owner],
          ENTITY_CHARS[e->type],
          e->hp,
          e->ap
          );
      else
        printf(
            "\x1b[K TURN: %d | PTURN:\x1b[%d;%dm %d \x1b[0m || G: %d | U: %d ||",
            serv.gameTurn,

            BG_OWNER_COLORS[game.playerTurn],
            FG_OWNER_COLORS[game.playerTurn],
            game.playerTurn,

            game.player.goldCount,
            game.player.uraniumCount
            );

      fflush(stdout);
      game.needRedraw = 0;
    }
  }

  disableRawMode();
  return 0;
}

