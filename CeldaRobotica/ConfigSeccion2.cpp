#include "ConfigSeccion2.h"

// --- Brazo 3 (p. ej. remachado) ---
const Postura POSICION_INICIAL_BRAZO3[] = {
  { 90, 90, 90, 0 },
};
const uint8_t POSICION_INICIAL_BRAZO3_LEN = sizeof(POSICION_INICIAL_BRAZO3) / sizeof(Postura);

const Postura RUTINA_BRAZO3[] = {
  { 60, 70,  100, 0   },
  { 60, 110, 80,  150 },
  { 90, 90,  90,  0   },
};
const uint8_t RUTINA_BRAZO3_LEN = sizeof(RUTINA_BRAZO3) / sizeof(Postura);

// --- Brazo 4 (p. ej. colocación de pieza) ---
const Postura POSICION_INICIAL_BRAZO4[] = {
  { 90, 90, 90, 0 },
};
const uint8_t POSICION_INICIAL_BRAZO4_LEN = sizeof(POSICION_INICIAL_BRAZO4) / sizeof(Postura);

const Postura RUTINA_BRAZO4[] = {
  { 120, 80,  60,  0   },
  { 120, 100, 110, 150 },
  { 90,  90,  90,  0   },
};
const uint8_t RUTINA_BRAZO4_LEN = sizeof(RUTINA_BRAZO4) / sizeof(Postura);
