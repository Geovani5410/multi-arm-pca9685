#include "ConfigSeccion3.h"

// --- Brazo 5: toma y sostiene la puerta ---
const Postura POSICION_INICIAL_BRAZO5[] = {
  { 90, 90, 90, 0 },
};
const uint8_t POSICION_INICIAL_BRAZO5_LEN = sizeof(POSICION_INICIAL_BRAZO5) / sizeof(Postura);

const Postura RUTINA_BRAZO5[] = {
  { 70, 60, 100, 0   },
  { 90, 90, 120, 300 },  // sostiene la puerta mientras Brazo6 la fija
  { 90, 90, 90,  0   },
};
const uint8_t RUTINA_BRAZO5_LEN = sizeof(RUTINA_BRAZO5) / sizeof(Postura);

// --- Brazo 6: fija la puerta (atornilla/ajusta) ---
const Postura POSICION_INICIAL_BRAZO6[] = {
  { 90, 90, 90, 0 },
};
const uint8_t POSICION_INICIAL_BRAZO6_LEN = sizeof(POSICION_INICIAL_BRAZO6) / sizeof(Postura);

const Postura RUTINA_BRAZO6[] = {
  { 110, 70,  90, 0   },
  { 90,  100, 70, 300 },
  { 90,  90,  90, 0   },
};
const uint8_t RUTINA_BRAZO6_LEN = sizeof(RUTINA_BRAZO6) / sizeof(Postura);
