#include "ConfigSeccion1.h"

// Postura de reposo compartida por Brazo1 y Brazo2
const Postura POSICION_INICIAL_SECCION1[] = {
  { 90, 90, 90, 0 },
};
const uint8_t POSICION_INICIAL_SECCION1_LEN = sizeof(POSICION_INICIAL_SECCION1) / sizeof(Postura);

// Rutina de soldadura, ejecutada en espejo por Brazo1 y Brazo2.
// Campos de cada Postura: { n1, n2, n3, pausaMs }
const Postura RUTINA_SOLDAR[] = {
  { 45, 60, 30,  0   },  // 1. Acercarse al punto de soldadura
  { 45, 90, 90,  0   },  // 2. Bajar el electrodo
  { 45, 90, 120, 200 },  // 3. Aplicar soldadura (pausa mientras suelda)
  { 45, 90, 90,  0   },  // 4. Levantar el electrodo
  { 90, 90, 90,  0   },  // 5. Volver a home
};
const uint8_t RUTINA_SOLDAR_LEN = sizeof(RUTINA_SOLDAR) / sizeof(Postura);
