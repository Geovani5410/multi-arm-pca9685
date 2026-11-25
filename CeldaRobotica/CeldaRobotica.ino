/**
 * CeldaRobotica.ino
 * ------------------
 * Orquestador principal. Su única responsabilidad es:
 *   1. Instanciar el hardware (2x PCA9685, 6x BrazoRobotico)
 *   2. Llamar, en orden legible, las rutinas de cada estación
 *
 * No contiene lógica de movimiento ni de hardware de bajo nivel: eso vive en
 * BrazoRobotico.h/.cpp (hardware) y en Seccion1/2/3 (secuencias). Gracias a
 * esta separación, añadir una Sección 4 en el futuro es tan simple como
 * crear Seccion4.h/.cpp y agregar un par de líneas aquí, sin tocar nada más.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "BrazoRobotico.h"
#include "Seccion1.h"
#include "Seccion2.h"
#include "Seccion3.h"

// ---------- Hardware: dos módulos PCA9685 ----------
// Direcciones I2C configuradas por los jumpers A0-A5 de cada placa.
// Repartir 6 brazos x 3 servos = 18 canales entre 2 placas de 16 canales
// cada una (9 de 16 usados por placa) deja margen para una futura Sección 4
// sin añadir una tercera placa.
Adafruit_PWMServoDriver pcaA = Adafruit_PWMServoDriver(0x40); // Brazos 1, 2, 3
Adafruit_PWMServoDriver pcaB = Adafruit_PWMServoDriver(0x41); // Brazos 4, 5, 6

// ---------- Brazos robóticos ----------
// Canales dentro de cada placa: (Base, Hombro, Codo) -> 3 canales por brazo.
// Nótese que espejoBase=true SOLO en Brazo2: es el único dato que relaciona
// esta instancia con "estar enfrentado a Brazo1"; el resto del código no
// necesita saberlo.
BrazoRobotico brazo1(pcaA, 0, 1, 2, false, "Brazo1"); // Sección 1 (referencia)
BrazoRobotico brazo2(pcaA, 3, 4, 5, true,  "Brazo2"); // Sección 1 (espejo de Brazo1)
BrazoRobotico brazo3(pcaA, 6, 7, 8, false, "Brazo3"); // Sección 2
BrazoRobotico brazo4(pcaB, 0, 1, 2, false, "Brazo4"); // Sección 2
BrazoRobotico brazo5(pcaB, 3, 4, 5, false, "Brazo5"); // Sección 3
BrazoRobotico brazo6(pcaB, 6, 7, 8, false, "Brazo6"); // Sección 3


void setup() {
  Serial.begin(115200);

  pcaA.begin();
  pcaA.setPWMFreq(50); // 50 Hz es el estándar para servos analógicos

  pcaB.begin();
  pcaB.setPWMFreq(50);

  delay(100); // Margen recomendado tras setPWMFreq antes de enviar pulsos

  // Llevar toda la celda a una postura conocida antes de empezar a operar
  Seccion1::posicionInicial(brazo1, brazo2);
  Seccion2::posicionInicial(brazo3, brazo4);
  Seccion3::posicionInicial(brazo5, brazo6);

  Serial.println(F("Celda robotica lista."));
}

void loop() {
  // El bucle principal es deliberadamente simple y legible: cada línea dice
  // "qué estación hace qué", sin ningún detalle de cómo lo hace por dentro.
  Seccion1::soldar(brazo1, brazo2);
  Seccion2::procesarEtapaDos(brazo3, brazo4);
  Seccion3::ensamblarPuerta(brazo5, brazo6);

  delay(1000); // Pausa entre ciclos completos de la celda (ajustar según proceso real)
}
