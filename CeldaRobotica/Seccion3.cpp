#include "Seccion3.h"
#include "ConfigSeccion3.h"
#include "RutinaRunner.h"

namespace Seccion3 {

void posicionInicial(BrazoRobotico &brazo5, BrazoRobotico &brazo6) {
  BrazoRobotico* grupo5[1] = { &brazo5 };
  BrazoRobotico* grupo6[1] = { &brazo6 };
  RutinaRunner::ejecutar(grupo5, 1, POSICION_INICIAL_BRAZO5, POSICION_INICIAL_BRAZO5_LEN);
  RutinaRunner::ejecutar(grupo6, 1, POSICION_INICIAL_BRAZO6, POSICION_INICIAL_BRAZO6_LEN);
}

void ensamblarPuerta(BrazoRobotico &brazo5, BrazoRobotico &brazo6) {
  BrazoRobotico* grupo5[1] = { &brazo5 };
  BrazoRobotico* grupo6[1] = { &brazo6 };
  // El ORDEN del relevo (Brazo5 primero, Brazo6 después) es la única
  // decisión de coreografía que vive en este archivo; los ángulos de cada
  // uno están en ConfigSeccion3.cpp.
  RutinaRunner::ejecutar(grupo5, 1, RUTINA_BRAZO5, RUTINA_BRAZO5_LEN);
  RutinaRunner::ejecutar(grupo6, 1, RUTINA_BRAZO6, RUTINA_BRAZO6_LEN);
}

}
