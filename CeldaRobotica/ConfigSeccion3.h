#ifndef CONFIG_SECCION3_H
#define CONFIG_SECCION3_H

#include "Postura.h"

/**
 * ============================================================
 *  ARCHIVO DE CONFIGURACIÓN — SECCIÓN 3 (Brazos 5 y 6, relevo)
 *  Responsable: <nombre del compañero asignado a esta etapa>
 * ============================================================
 * Brazo5 primero posiciona y sostiene la puerta; Brazo6 la fija después.
 * El ORDEN de ejecución (quién va primero) vive en Seccion3.cpp; aquí solo
 * se configuran los ángulos propios de cada brazo. Ver README.md.
 */

extern const Postura POSICION_INICIAL_BRAZO5[];
extern const uint8_t POSICION_INICIAL_BRAZO5_LEN;
extern const Postura RUTINA_BRAZO5[];
extern const uint8_t RUTINA_BRAZO5_LEN;

extern const Postura POSICION_INICIAL_BRAZO6[];
extern const uint8_t POSICION_INICIAL_BRAZO6_LEN;
extern const Postura RUTINA_BRAZO6[];
extern const uint8_t RUTINA_BRAZO6_LEN;

#endif
