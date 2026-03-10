#ifndef CONFIG_H
#define CONFIG_H

// Limits
#define MAX_ENTITIES 128
#define MAX_CHUNKS 128
#define MAX_PLAYERS 3 // 0 - empty
#define MAX_NUKES 4

// Player's initial resources
#define INITIAL_GOLD 100
#define INITIAL_URANIUM 0

// Map size (256x240)
#define SIZE_X_CHUNKS 16
#define SIZE_Y_CHUNKS 15

// Chunk address
#define START_P0_X 7 // ((SIZE_X_CHUNKS - 2) >> 1)
#define START_P0_Y 0

#define START_P1_X 0
#define START_P1_Y 14 // (SIZE_Y_CHUNKS - 1)

#define START_P2_X 15 // (SIZE_X_CHUNKS - 1)
#define START_P2_Y 14 // (SIZE_Y_CHUNKS - 1)

#endif // CONFIG_H
