#include "Banda.h"

Banda::Banda(uint8_t pinENA, uint8_t pinIN1, uint8_t pinIN2,
             uint8_t pinFinAvance, uint8_t pinFinRetroceso)
  : _motor(pinENA, pinIN1, pinIN2),
    _pinFinAvance(pinFinAvance), _pinFinRetroceso(pinFinRetroceso),
    _velocidad(0), _sentido(SentidoBanda::AVANCE) {}

void Banda::iniciar(uint8_t velocidadPWM) {
  _motor.iniciar();
  // Finales de carrera normalmente abiertos, a GND cuando se activan.
  pinMode(_pinFinAvance, INPUT_PULLUP);
  pinMode(_pinFinRetroceso, INPUT_PULLUP);

  _velocidad = velocidadPWM;
  _sentido = SentidoBanda::AVANCE;
}

void Banda::detener() {
  _motor.detener();
}

bool Banda::finDeCarreraActivado() {
  uint8_t pin = (_sentido == SentidoBanda::AVANCE)
                  ? _pinFinAvance
                  : _pinFinRetroceso;
  // INPUT_PULLUP: el switch activado lee LOW.
  return digitalRead(pin) == LOW;
}

bool Banda::avanzarEtapa(uint16_t duracionMs) {
  if (_sentido == SentidoBanda::AVANCE) {
    _motor.adelante(_velocidad);
  } else {
    _motor.atras(_velocidad);
  }

  unsigned long inicio = millis();
  bool activado = false;

  while (millis() - inicio < duracionMs) {
    if (finDeCarreraActivado()) {
      activado = true;
      break;
    }
  }

  _motor.detener();

  if (activado) {
    // Se llegó al extremo: invierte el sentido para el próximo tramo.
    _sentido = (_sentido == SentidoBanda::AVANCE)
                 ? SentidoBanda::RETROCESO
                 : SentidoBanda::AVANCE;
  }

  return activado;
}

SentidoBanda Banda::sentidoActual() const {
  return _sentido;
}
