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
#include "Banda.h"
#include "MotorL298N.h"

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
// Nota: el nombre ("N1", "N2"...) ya no es solo para logs -- es el mismo
// identificador que lee el HMI por Serial (ver SerialReporter), así que debe
// coincidir exactamente con lo que espera app.py en la Raspberry Pi/laptop.
BrazoRobotico brazo1(pcaA, 0, 1, 2, false, "N1"); // Sección 1 (referencia)
BrazoRobotico brazo2(pcaA, 3, 4, 5, true,  "N2"); // Sección 1 (espejo de Brazo1)
BrazoRobotico brazo3(pcaA, 6, 7, 8, false, "N3"); // Sección 2
BrazoRobotico brazo4(pcaB, 0, 1, 2, false, "N4"); // Sección 2
BrazoRobotico brazo5(pcaB, 3, 4, 5, false, "N5"); // Sección 3
BrazoRobotico brazo6(pcaB, 6, 7, 8, false, "N6"); // Sección 3

// ---------- Banda y Elevador (canal A y B del L298N) ----------
// Pines según el mapa de pines del ESP32 (README): ENA/IN1/IN2 = 25/26/27,
// finales de carrera 32/33; elevador en ENB/IN3/IN4 = 23/18/5.
Banda banda(25, 26, 27, 32, 33);
MotorL298N elevador(23, 18, 5);

const uint8_t VELOCIDAD_BANDA = 200;
const uint8_t VELOCIDAD_ELEVADOR = 200;

// Cota máxima de seguridad para los tramos de banda que deben moverse
// "hasta que detecten el final de carrera": si por algún motivo el switch
// no se activa, el motor igual se detiene a los 60s en vez de girar
// indefinidamente.
const uint16_t TIEMPO_MAX_HASTA_SWITCH = 60000;

// Ejecuta una rutina de brazos y, si termina antes de msObjetivo, completa
// el tiempo restante en espera -- así "ejecutar N1 y N2 durante 30s"
// siempre dura 30s reales, sin importar cuánto tarde la rutina en sí.
void ejecutarDurante(void (*rutina)(), uint32_t msObjetivo) {
  uint32_t inicio = millis();
  rutina();
  int32_t restante = (int32_t)msObjetivo - (int32_t)(millis() - inicio);
  if (restante > 0) delay((uint32_t)restante);
}

// Envoltorios sin argumentos, para poder pasarlos como puntero a función
// a ejecutarDurante() (las rutinas de Seccion1/2/3 reciben los brazos).
void rutinaSeccion1() { Seccion1::soldar(brazo1, brazo2); }
void rutinaSeccion2() { Seccion2::procesarEtapaDos(brazo3, brazo4); }
void rutinaSeccion3() { Seccion3::ensamblarPuerta(brazo5, brazo6); }

void subirElevador(uint16_t duracionMs) {
  elevador.adelante(VELOCIDAD_ELEVADOR);
  delay(duracionMs);
  elevador.detener();
}

// Ejecuta el ciclo completo de la celda, en el orden exacto solicitado.
// Es una secuencia de UNA sola pasada (no un bucle): termina deteniéndose
// con la banda de regreso en el primer final de carrera.
void cicloCompleto() {
  // 1) Arranca la banda y la mueve 20s
  banda.avanzarEtapa(20000);

  // 2) Mueve N1 y N2 durante 30s
  ejecutarDurante(rutinaSeccion1, 30000);

  // 3) Mueve la banda 30s
  banda.avanzarEtapa(30000);

  // 4) Ejecuta N3 y N4 durante 30s
  ejecutarDurante(rutinaSeccion2, 30000);

  // 5) Mueve la banda 30s
  banda.avanzarEtapa(30000);

  // 6) Ejecuta N5 y N6 (sin tiempo fijo pedido: corre la rutina completa una vez)
  rutinaSeccion3();

  // 7) Mueve la banda 20s
  banda.avanzarEtapa(20000);

  // 8) Sube el elevador 10s
  subirElevador(10000);

  // 9) Mueve la banda hacia adelante hasta que detecte el final de carrera
  //    del extremo de avance...
  banda.avanzarEtapa(TIEMPO_MAX_HASTA_SWITCH);
  // ...avanzarEtapa() ya invirtió el sentido internamente al detectar el
  // switch, así que esta segunda llamada regresa en automático hasta el
  // PRIMER final de carrera (el de origen) y se detiene ahí.
  banda.avanzarEtapa(TIEMPO_MAX_HASTA_SWITCH);

  banda.detener();
  Serial.println(F("Ciclo completo. Celda detenida."));
}

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

  banda.iniciar(VELOCIDAD_BANDA);
  elevador.iniciar();

  Serial.println(F("Celda robotica lista."));

  cicloCompleto();
}

void loop() {
  // El ciclo es de una sola pasada (ver cicloCompleto()); al terminar, la
  // celda se queda detenida a propósito. Para repetirlo, mueve la llamada
  // a cicloCompleto() aquí en vez de en setup().
}