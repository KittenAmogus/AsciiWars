#ifndef CONFIG_H
#define CONFIG_H

// For compilation
#define ARDUINO 0
#define DEBUG 0

// For limits
#define LIMIT_MAX_UNITS 32
#define LIMIT_MAX_BUILDINGS 32

// For sizes

// Size (Cells)
#if ARDUINO
#define SIZE_X_VIEWPORT 8
#define SIZE_Y_VIEWPORT 4
#else
#define SIZE_X_VIEWPORT 16
#define SIZE_Y_VIEWPORT 8
#endif

// If u see it, send me in char '0xA0', i want to check if u seen my code

#endif // CONFIG_H
