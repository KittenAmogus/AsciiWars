#ifndef CONFIG_H
#define CONFIG_H

// #define ARDUINO
#define SERVER

// Viewport
#ifdef ARDUINO
#define SIZE_X_VIEWPORT 8
#define SIZE_Y_VIEWPORT 4
#else
#define SIZE_X_VIEWPORT 16
#define SIZE_Y_VIEWPORT 16
#endif

// World
#define SIZE_X_WORLD 32
#define SIZE_Y_WORLD 32

// Limits
#define LIMIT_ENTITIES 256
#define LIMIT_PLAYERS 4
#define LIMIT_NUKES 4

// Resource limit per player
#define LIMIT_GOLD 1024
#define LIMIT_URANIUM 256

// Initial values
#define INITIAL_AP 15
#define INITIAL_GOLD 100
#define INITIAL_URANIUM 0

#endif // CONFIG_H
