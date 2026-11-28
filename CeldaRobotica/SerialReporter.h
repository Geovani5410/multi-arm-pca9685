#ifndef SERIAL_REPORTER_H
#define SERIAL_REPORTER_H

#include <Arduino.h>
#include "Postura.h"

/**
 * SerialReporter
 * ---------------
 * Núcleo compartido (Capa 3): le avisa al HMI (vía el servidor en la
 * Raspberry Pi / laptop) qué ángulos lógicos acaba de alcanzar un brazo,
 * para que la simulación 3D se mueva en espejo con el hardware real.
 *
 * Formato de línea (una por brazo, una por postura alcanzada):
 *   "N1,45,90,120,0"
 *    id, n1, n2, n3, pausaMs
 *
 * Este módulo NO decide cuándo reportar -- eso lo hace RutinaRunner,
 * que es quien conoce el grupo de brazos y la postura ya aplicada.
 * No se modifica para ajustar el comportamiento de una sección.
 */
void reportarMovimiento(const char* idBrazo, const Postura &p);

#endif
