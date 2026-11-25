#ifndef BRAZO_ROBOTICO_H
#define BRAZO_ROBOTICO_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

/**
 * BrazoRobotico
 * --------------
 * Representa UN brazo de 3 grados de libertad (Base N1, Hombro N2, Codo N3),
 * cableado a 3 canales de un módulo PCA9685.
 *
 * Responsabilidad única: traducir ángulos LÓGICOS (0-180°, con 90° como
 * referencia neutra) a pulsos PWM físicos en el canal correcto del PCA9685,
 * aplicando la inversión de "modo espejo" cuando el brazo está orientado en
 * sentido opuesto a su pareja (p. ej. Brazo2 frente a Brazo1).
 *
 * Esta clase NO sabe nada de soldadura, pintura ni ensamble. Esa lógica vive
 * en las librerías de estación (Seccion1/2/3), que solo manejan ángulos
 * lógicos y delegan en BrazoRobotico la traducción final a hardware. Así,
 * una rutina escrita para "Brazo1" funciona igual para "Brazo2" sin que su
 * autor tenga que acordarse de cuál de los dos tiene la base invertida.
 */
class BrazoRobotico {
  public:
    // Calibración de pulsos PCA9685 a 50Hz para servos estándar.
    // AJUSTAR estos valores con los servos reales (varían por marca/modelo).
    static const uint16_t PULSO_MIN = 102;  // ~1.0 ms -> 0°
    static const uint16_t PULSO_MAX = 512;  // ~2.0 ms -> 180°
    static const uint8_t  ANGULO_MIN = 0;
    static const uint8_t  ANGULO_MAX = 180;

    // Tamaño máximo de grupo para moverGrupoSincronizado. Se fija en
    // compilación (en vez de usar arreglos de tamaño variable) porque en un
    // microcontrolador con poca RAM es preferible reservar memoria fija y
    // predecible a depender de VLAs o de new/delete.
    static const uint8_t MAX_GRUPO = 6;

    /**
     * @param driver       Referencia al PCA9685 físico donde está cableado este brazo
     * @param canalBase    Canal (0-15) del PCA9685 para el servo de la Base (N1)
     * @param canalHombro  Canal (0-15) del PCA9685 para el servo del Hombro (N2)
     * @param canalCodo    Canal (0-15) del PCA9685 para el servo del Codo (N3)
     * @param espejoBase   true = invierte el ángulo de la Base (N1). Se usa en el
     *                     brazo "reflejado" de una pareja enfrentada (Sección 1),
     *                     para que ambos ejecuten la misma rutina lógica y se vean
     *                     simétricos sin que la rutina tenga que saberlo.
     * @param nombre       Identificador legible para logs/depuración (p. ej. "Brazo1")
     */
    BrazoRobotico(Adafruit_PWMServoDriver &driver,
                  uint8_t canalBase, uint8_t canalHombro, uint8_t canalCodo,
                  bool espejoBase = false,
                  const char* nombre = "Brazo");

    // Mueve una sola articulación (ángulo lógico 0-180°)
    void moverBase(float grados);
    void moverHombro(float grados);
    void moverCodo(float grados);

    // Mueve las 3 articulaciones a la vez (postura completa)
    void moverA(float anguloN1, float anguloN2, float anguloN3);

    // Postura de reposo / home. Útil al iniciar y al terminar cada rutina.
    void irAHome();

    const char* getNombre() const { return _nombre; }

    /**
     * Interpola linealmente un GRUPO de brazos hacia un conjunto de posturas
     * objetivo de forma SINCRONIZADA: todos avanzan al mismo tiempo, paso a
     * paso, en lugar de que cada uno llegue a su destino por separado.
     *
     * Esto es lo que hace que Brazo1 y Brazo2 (modo espejo) se vean
     * ejecutando "la misma" coreografía en tiempo real, aunque uno tenga la
     * base invertida internamente.
     *
     * @param brazos     Arreglo de punteros a los brazos a mover
     * @param cantidad   Número de brazos en el arreglo (máx. MAX_GRUPO)
     * @param n1,n2,n3   Arreglos de ángulos objetivo, uno por brazo, mismo
     *                   orden que 'brazos'
     * @param pasos      Pasos de interpolación (más pasos = más resolución temporal)
     * @param msPorPaso  Milisegundos de espera entre cada paso
     *
     * La interpolación aplica una curva de aceleración/desaceleración
     * (smoothstep) en vez de velocidad constante, para que el servo no
     * arranque ni frene de golpe en cada postura.
     */
    static void moverGrupoSincronizado(BrazoRobotico* brazos[], uint8_t cantidad,
                                        const float n1[], const float n2[], const float n3[],
                                        uint8_t pasos = 20, uint16_t msPorPaso = 15);

  private:
    Adafruit_PWMServoDriver &_driver;
    uint8_t _canalBase, _canalHombro, _canalCodo;
    bool _espejoBase;
    const char* _nombre;

    // Posición lógica actual de cada articulación. Se guarda para poder
    // interpolar "desde donde el brazo realmente está" y no desde una
    // posición asumida.
    float _actualN1, _actualN2, _actualN3;

    // Convierte un ángulo lógico (0-180°) a cuentas de pulso PWM (12 bits)
    uint16_t anguloAPulso(float grados) const;

    // Aplica la inversión de espejo únicamente a la Base (N1), que es el eje
    // que debe invertirse para que dos brazos enfrentados ejecuten la misma
    // rutina de forma simétrica. Hombro y Codo no cambian de "lado" al
    // enfrentar los brazos, así que no se invierten.
    float aplicarEspejo(float grados) const;

    // Escribe en el PCA9685 un ángulo lógico ya resuelto (con espejo aplicado)
    void escribirCanal(uint8_t canal, float grados) const;
};

#endif
