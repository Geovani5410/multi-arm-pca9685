#ifndef MOTOR_L298N_H
#define MOTOR_L298N_H

#include <Arduino.h>

// Controla un solo canal (A o B) de un driver L298N: velocidad por PWM
// (pin EN) y sentido de giro por dos pines digitales (IN1/IN2 o IN3/IN4).
//
// Esta clase no sabe nada de finales de carrera, etapas, ni para qué se usa
// el motor: solo lo mueve hacia adelante, hacia atrás, o lo detiene. Sirve
// como base para controlar cualquier motor conectado al L298N (banda,
// elevador, etc.) sin repetir el código de bajo nivel en cada librería.
class MotorL298N {
  public:
    // pinEN: PWM de velocidad (ENA o ENB)
    // pinIN_A, pinIN_B: control de sentido (IN1/IN2 o IN3/IN4)
    MotorL298N(uint8_t pinEN, uint8_t pinIN_A, uint8_t pinIN_B);

    void iniciar();

    void adelante(uint8_t velocidadPWM);
    void atras(uint8_t velocidadPWM);
    void detener();

  private:
    uint8_t _pinEN, _pinIN_A, _pinIN_B;
};

#endif
