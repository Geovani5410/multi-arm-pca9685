#ifndef POSTURA_H
#define POSTURA_H

#include <Arduino.h>

/**
 * Postura
 * --------
 * Un punto lógico de una rutina: el ángulo objetivo de cada articulación
 * (Base N1, Hombro N2, Codo N3) y, opcionalmente, cuánto tiempo (ms) debe
 * quedarse el brazo quieto en esa postura antes de avanzar a la siguiente.
 *
 * Esta es la ÚNICA estructura que un compañero necesita conocer para
 * configurar su etapa: no requiere entender BrazoRobotico ni RutinaRunner,
 * solo llenar arreglos de 'Postura' en su propio archivo
 * ConfigSeccionN.h/.cpp (ver README.md).
 *
 * Nota de portabilidad: se evita el uso de valores por defecto en los
 * miembros (ej. "pausaMs = 0") para que la inicialización agregada
 * { n1, n2, n3, pausaMs } funcione igual en cualquier placa/toolchain
 * (AVR clásico, ESP32, etc.). Por eso cada Postura en los archivos de
 * configuración siempre debe llevar sus 4 valores explícitos.
 */
struct Postura {
  float n1;            // Ángulo lógico de la Base   (0-180°, 90° = neutro)
  float n2;            // Ángulo lógico del Hombro   (0-180°, 90° = neutro)
  float n3;            // Ángulo lógico del Codo     (0-180°, 90° = neutro)
  uint16_t pausaMs;    // Pausa (ms) al llegar a esta postura (0 = sin pausa)
};

#endif
