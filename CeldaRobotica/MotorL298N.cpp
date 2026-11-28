#include "MotorL298N.h"

MotorL298N::MotorL298N(uint8_t pinEN, uint8_t pinIN_A, uint8_t pinIN_B)
  : _pinEN(pinEN), _pinIN_A(pinIN_A), _pinIN_B(pinIN_B) {}

void MotorL298N::iniciar() {
  pinMode(_pinEN, OUTPUT);
  pinMode(_pinIN_A, OUTPUT);
  pinMode(_pinIN_B, OUTPUT);
  detener();
}

void MotorL298N::adelante(uint8_t velocidadPWM) {
  digitalWrite(_pinIN_A, HIGH);
  digitalWrite(_pinIN_B, LOW);
  analogWrite(_pinEN, velocidadPWM);
}

void MotorL298N::atras(uint8_t velocidadPWM) {
  digitalWrite(_pinIN_A, LOW);
  digitalWrite(_pinIN_B, HIGH);
  analogWrite(_pinEN, velocidadPWM);
}

void MotorL298N::detener() {
  digitalWrite(_pinIN_A, LOW);
  digitalWrite(_pinIN_B, LOW);
  analogWrite(_pinEN, 0);
}
