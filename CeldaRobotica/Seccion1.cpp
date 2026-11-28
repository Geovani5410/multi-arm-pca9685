#include "Seccion1.h"
#include "ConfigSeccion1.h"
#include "RutinaRunner.h"

namespace Seccion1 {

void posicionInicial(BrazoRobotico &brazo1, BrazoRobotico &brazo2) {
  BrazoRobotico* grupo[2] = { &brazo1, &brazo2 };
  RutinaRunner::ejecutar(grupo, 2, POSICION_INICIAL_SECCION1, POSICION_INICIAL_SECCION1_LEN);
}

void soldar(BrazoRobotico &brazo1, BrazoRobotico &brazo2) {
  BrazoRobotico* grupo[2] = { &brazo1, &brazo2 };
  RutinaRunner::ejecutar(grupo, 2, RUTINA_SOLDAR, RUTINA_SOLDAR_LEN);
}

}
