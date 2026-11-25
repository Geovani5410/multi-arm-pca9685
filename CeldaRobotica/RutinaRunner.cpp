#include "RutinaRunner.h"

namespace RutinaRunner {

void ejecutar(BrazoRobotico* grupo[], uint8_t cantidad,
              const Postura rutina[], uint8_t numPosturas,
              uint8_t pasosPorTramo, uint16_t msPorPaso) {

  // Se reutiliza el mismo límite MAX_GRUPO que ya usa BrazoRobotico, para no
  // depender de memoria dinámica ni de arreglos de tamaño variable.
  uint8_t cantidadSegura = (cantidad > BrazoRobotico::MAX_GRUPO) ? BrazoRobotico::MAX_GRUPO : cantidad;

  float n1[BrazoRobotico::MAX_GRUPO];
  float n2[BrazoRobotico::MAX_GRUPO];
  float n3[BrazoRobotico::MAX_GRUPO];

  for (uint8_t p = 0; p < numPosturas; p++) {
    const Postura &postura = rutina[p];

    // "Difunde" la misma postura lógica a cada brazo del grupo. Si el grupo
    // es de un solo brazo (rutinas independientes, ej. Sección 2), esto
    // simplemente mueve a ese único brazo.
    for (uint8_t i = 0; i < cantidadSegura; i++) {
      n1[i] = postura.n1;
      n2[i] = postura.n2;
      n3[i] = postura.n3;
    }

    BrazoRobotico::moverGrupoSincronizado(grupo, cantidadSegura, n1, n2, n3, pasosPorTramo, msPorPaso);

    if (postura.pausaMs > 0) {
      delay(postura.pausaMs);
    }
  }
}

}
