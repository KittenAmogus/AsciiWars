#include "utils/config.h"
#include "utils/memory.h"

#ifndef INPUT_H
#define INPUT_H

typedef enum {
  INPUT_NONE = 0,  // Error
  INPUT_MOVE,      // Move cursor
  INPUT_ACTION,    // Select, Attack
  INPUT_CANCEL,    // Cancel action choice
  INPUT_NEXT_TURN, // End turn
  INPUT_QUIT       // ^C / ^D
} InputEventTypes;

/*
typedef enum {
  ACTION_CANCEL = 0,
  ACTION_ATTACK, // Attack unit
  ACTION_SELL,   // Sell unit
  ACTION_NUKE    // Nuke cell
} ActionType;
*/  // Let main.c decide what to do with action

typedef struct {
  int8_t dx;
  int8_t dy;
} EventMove;

typedef union {
  EventMove moveEvent;
  uint8_t unstructed_event;
} InputUnion;

typedef union {
  char pcChar;
  struct {
    uint16_t deltaX;
    uint16_t deltaY;
    uint8_t buttonPressed;
  } arduino;
} RawInput;

typedef struct {
  InputEventTypes type;
  InputUnion data;
  RawInput raw;
} __attribute__((packed)) InputEvent;

#endif
