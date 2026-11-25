#ifndef SECCION3_H
#define SECCION3_H

#include "BrazoRobotico.h"

/**
 * Sección 3 — Brazos 5 y 6 (relevo: ensamble de puerta)
 * ----------------------------------------------------------
 * "Pegamento" entre RutinaRunner y los datos de ConfigSeccion3. Los ángulos
 * de cada brazo se editan en ConfigSeccion3.cpp, nunca aquí.
 */
namespace Seccion3 {

  void posicionInicial(BrazoRobotico &brazo5, BrazoRobotico &brazo6);
  void ensamblarPuerta(BrazoRobotico &brazo5, BrazoRobotico &brazo6);

}

#endif
