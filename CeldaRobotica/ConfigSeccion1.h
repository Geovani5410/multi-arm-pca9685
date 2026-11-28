#ifndef CONFIG_SECCION1_H
#define CONFIG_SECCION1_H

#include "Postura.h"

/**
 * ================================================================
 *  ARCHIVO DE CONFIGURACIÓN — SECCIÓN 1 (Brazos 1 y 2, modo espejo)
 *  Responsable: <nombre del compañero asignado a esta etapa>
 * ================================================================
 * Este archivo (.h + .cpp) es el ÚNICO lugar donde deberías editar
 * ángulos, posturas o rutinas de la Sección 1. No requiere tocar
 * BrazoRobotico, RutinaRunner, Postura.h, Main.ino ni las demás
 * secciones. Ver README.md para la guía completa de edición.
 *
 * Para AGREGAR una postura nueva a una rutina existente: añade una línea
 * al arreglo correspondiente en ConfigSeccion1.cpp (el LEN se calcula solo).
 * Para AGREGAR una rutina nueva (p. ej. una variante de soldadura): declara
 * aquí un nuevo par extern (arreglo + LEN), defínelo en el .cpp, y agrega
 * una función correspondiente en Seccion1.h/.cpp que la invoque con
 * RutinaRunner::ejecutar(...).
 */

extern const Postura POSICION_INICIAL_SECCION1[];
extern const uint8_t POSICION_INICIAL_SECCION1_LEN;

extern const Postura RUTINA_SOLDAR[];
extern const uint8_t RUTINA_SOLDAR_LEN;

#endif
