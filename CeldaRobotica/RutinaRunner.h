#ifndef RUTINA_RUNNER_H
#define RUTINA_RUNNER_H

#include "BrazoRobotico.h"
#include "Postura.h"

/**
 * RutinaRunner
 * -------------
 * Motor GENÉRICO y compartido (núcleo) para ejecutar una rutina —un arreglo
 * de Postura— sobre un grupo de uno o más brazos sincronizados entre sí.
 *
 * Es la pieza que separa "QUÉ ángulos mover" (los archivos
 * ConfigSeccionN.h/.cpp, que cada compañero edita de forma independiente)
 * de "CÓMO se mueven" (este archivo). RutinaRunner no sabe nada de
 * soldadura, ensamble ni de qué sección es cada brazo: solo recibe un
 * arreglo de Postura y un grupo de BrazoRobotico, y los mueve en conjunto
 * (reutilizando BrazoRobotico::moverGrupoSincronizado, con su suavizado de
 * velocidad ya incluido), respetando la pausa de cada postura.
 *
 * Al ser núcleo compartido, NO debería modificarse para ajustar ángulos de
 * una sección: eso se hace siempre en el ConfigSeccionN correspondiente.
 */
namespace RutinaRunner {

  /**
   * @param grupo          Arreglo de punteros a los brazos a mover. Todos
   *                        reciben la MISMA postura lógica en cada paso
   *                        (útil para parejas en espejo, p. ej. Sección 1).
   *                        Para un brazo que actúa solo, pasar un grupo de
   *                        tamaño 1.
   * @param cantidad        Número de brazos en 'grupo' (máx. BrazoRobotico::MAX_GRUPO)
   * @param rutina          Arreglo de Postura a ejecutar, en orden
   * @param numPosturas     Número de elementos de 'rutina'
   * @param pasosPorTramo   Resolución de interpolación entre posturas consecutivas
   * @param msPorPaso       Milisegundos de espera entre cada paso de interpolación
   */
  void ejecutar(BrazoRobotico* grupo[], uint8_t cantidad,
                const Postura rutina[], uint8_t numPosturas,
                uint8_t pasosPorTramo = 25, uint16_t msPorPaso = 12);

}

#endif
