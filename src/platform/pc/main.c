#include "config.h"
#include "memory/memory.h"

#include <string.h>

#ifdef SERVER
extern ServerData sdata;

void serverInit() {
  memset(&sdata, 0, sizeof(sdata));
  for (uint16_t i = 0; i < LIMIT_ENTITIES; i++) {
    Entity *e = &sdata.allEntities[i];
    e->hp = 1;
  }
}

#endif

int main(void) {

#ifdef SERVER
  serverInit();
#endif

  return 0;
}
