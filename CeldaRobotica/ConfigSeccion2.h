#ifndef CONFIG_SECCION2_H
#define CONFIG_SECCION2_H

#include "Postura.h"

/**
 * ============================================================
 *  ARCHIVO DE CONFIGURACIÓN — SECCIÓN 2 (Brazos 3 y 4, independientes)
 *  Responsable: <nombre del compañero asignado a esta etapa>
 * ============================================================
 * Brazo3 y Brazo4 NO se mueven sincronizados entre sí, así que cada uno
 * tiene su propio arreglo de posturas. Ver README.md para la guía completa.
 */

extern const Postura POSICION_INICIAL_BRAZO3[];
extern const uint8_t POSICION_INICIAL_BRAZO3_LEN;
extern const Postura RUTINA_BRAZO3[];
extern const uint8_t RUTINA_BRAZO3_LEN;

extern const Postura POSICION_INICIAL_BRAZO4[];
extern const uint8_t POSICION_INICIAL_BRAZO4_LEN;
extern const Postura RUTINA_BRAZO4[];
extern const uint8_t RUTINA_BRAZO4_LEN;

#endif
