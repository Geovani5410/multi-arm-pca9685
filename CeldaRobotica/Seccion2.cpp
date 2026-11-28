#include "Seccion2.h"
#include "ConfigSeccion2.h"
#include "RutinaRunner.h"

namespace Seccion2 {

void posicionInicial(BrazoRobotico &brazo3, BrazoRobotico &brazo4) {
  BrazoRobotico* grupo3[1] = { &brazo3 };
  BrazoRobotico* grupo4[1] = { &brazo4 };
  RutinaRunner::ejecutar(grupo3, 1, POSICION_INICIAL_BRAZO3, POSICION_INICIAL_BRAZO3_LEN);
  RutinaRunner::ejecutar(grupo4, 1, POSICION_INICIAL_BRAZO4, POSICION_INICIAL_BRAZO4_LEN);
}

void procesarEtapaDos(BrazoRobotico &brazo3, BrazoRobotico &brazo4) {
  BrazoRobotico* grupo3[1] = { &brazo3 };
  BrazoRobotico* grupo4[1] = { &brazo4 };
  // Grupos de tamaño 1: cada brazo ejecuta su propia rutina, sin sincronía
  // entre ellos (a diferencia de Sección 1).
  RutinaRunner::ejecutar(grupo3, 1, RUTINA_BRAZO3, RUTINA_BRAZO3_LEN);
  RutinaRunner::ejecutar(grupo4, 1, RUTINA_BRAZO4, RUTINA_BRAZO4_LEN);
}

}
