#ifndef SECCION2_H
#define SECCION2_H

#include "BrazoRobotico.h"

/**
 * Sección 2 — Brazos 3 y 4 (rutinas independientes)
 * ----------------------------------------------------
 * "Pegamento" entre RutinaRunner y los datos de ConfigSeccion2. Los ángulos
 * de cada brazo se editan en ConfigSeccion2.cpp, nunca aquí.
 */
namespace Seccion2 {

  void posicionInicial(BrazoRobotico &brazo3, BrazoRobotico &brazo4);
  void procesarEtapaDos(BrazoRobotico &brazo3, BrazoRobotico &brazo4);

}

#endif
