#ifndef SECCION1_H
#define SECCION1_H

#include "BrazoRobotico.h"

/**
 * Sección 1 — Brazos 1 y 2 (modo espejo)
 * ----------------------------------------
 * Este archivo es "pegamento": solo nombra las rutinas disponibles para esta
 * estación y las conecta con RutinaRunner + los datos de ConfigSeccion1.
 * NO contiene ángulos (esos viven en ConfigSeccion1.cpp) ni lógica de bajo
 * nivel (esa vive en BrazoRobotico/RutinaRunner). Por eso rara vez necesita
 * cambiar: ajustar la rutina de soldadura es editar ConfigSeccion1.cpp, no
 * este archivo.
 */
namespace Seccion1 {

  void posicionInicial(BrazoRobotico &brazo1, BrazoRobotico &brazo2);
  void soldar(BrazoRobotico &brazo1, BrazoRobotico &brazo2);

}

#endif
