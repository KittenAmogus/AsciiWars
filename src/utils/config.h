#ifndef CONFIG_H
#define CONFIG_H

// For compilation
#define ARDUINO 0
#define DEBUG 0

// For limits
#define LIMIT_MAX_ENTITIES 64

#define LIMIT_MAX_NUKES 4
#define LIMIT_MAX_CHUNKS 64

// For sizes

// Size (Cells)
#if ARDUINO
#define SIZE_X_VIEWPORT 8
#define SIZE_Y_VIEWPORT 4
#define XY_VIEWPORT 32
#else
#define SIZE_X_VIEWPORT 16
#define SIZE_Y_VIEWPORT 16

#define XY_VIEWPORT 128
#endif

#define DISABLED_CHUNK_X 0xFF
#define DISABLED_CHUNK_Y 0xFF

#endif // CONFIG_H
