#ifndef BANDA_H
#define BANDA_H

#include <Arduino.h>
#include "MotorL298N.h"

// Sentido actual de giro del motor de la banda.
enum class SentidoBanda : uint8_t {
  AVANCE = 0,
  RETROCESO = 1
};

// Controla el motor de la banda transportadora (canal A del L298N, vía
// MotorL298N) y los dos finales de carrera (uno en cada extremo) que
// indican cuándo invertir el sentido de giro.
class Banda {
  public:
    // pinENA, pinIN1, pinIN2: canal del L298N asignado a la banda
    // pinFinAvance: final de carrera del lado de avance
    // pinFinRetroceso: final de carrera del lado de retroceso
    Banda(uint8_t pinENA, uint8_t pinIN1, uint8_t pinIN2,
          uint8_t pinFinAvance, uint8_t pinFinRetroceso);

    // Configura pines y arranca en sentido de AVANCE, detenida.
    void iniciar(uint8_t velocidadPWM = 200);

    // Avanza en el sentido actual durante duracionMs, o menos si se activa
    // antes el final de carrera correspondiente. Si el final de carrera se
    // activa, detiene el motor e invierte el sentido para la próxima llamada.
    // Devuelve true si el final de carrera se activó durante este tramo.
    bool avanzarEtapa(uint16_t duracionMs);

    // Detiene el motor.
    void detener();

    // Lee el final de carrera correspondiente al sentido actual.
    bool finDeCarreraActivado();

    SentidoBanda sentidoActual() const;

  private:
    MotorL298N _motor;
    uint8_t _pinFinAvance, _pinFinRetroceso;
    uint8_t _velocidad;
    SentidoBanda _sentido;
};

#endif
