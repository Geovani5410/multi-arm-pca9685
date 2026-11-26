# Celda Robótica Modular — Simulación Tipo KUKA KR120 (Proyecto Integradora III)

## Descripción general

Este proyecto implementa una celda de manufactura robótica a escala que
simula una línea de ensamblaje automotriz, tomando como referencia el
robot industrial **KUKA KR120**. Un único **ESP32** controla todo el
sistema, que se divide en dos subsistemas:

1. **Brazos robóticos** — 6 brazos de 3 grados de libertad cada uno (18
   servomotores en total), organizados en 3 secciones, operados mediante
   dos módulos PCA9685 por I2C.
2. **Banda transportadora** — un motor DC controlado por un driver L298N,
   que avanza en tramos cronometrados y se detiene en cada sección para
   que los brazos correspondientes ejecuten su rutina. Dos finales de
   carrera (uno en cada extremo) invierten el sentido de giro
   automáticamente.

| Sección | Brazos involucrados | Etapa que representa |
|---|---|---|
| Sección 1 | Brazo1, Brazo2 | Soldadura (modo espejo) |
| Sección 2 | Brazo3, Brazo4 | Etapa independiente |
| Sección 3 | Brazo5, Brazo6 | Ensamble de puerta |
| Sección 4 | — | Reservada para una futura estación (p. ej. elevador) |

## Arquitectura del proyecto

El código se organiza en 3 capas, una por cada subsistema más un núcleo
compartido. Cuanto más baja está una capa, con menor frecuencia debería
modificarse:

![Arquitectura del proyecto](img/arquitectura.png)

**Idea clave:** la gran mayoría del trabajo de desarrollo ocurre en la
Capa 1 de cada subsistema. Modificar la Capa 3 (núcleo compartido) afecta
a todo el sistema, por lo que cualquier cambio ahí debe coordinarse con el
resto del equipo de mantenimiento del proyecto.

## Requisitos

- Entorno de desarrollo Arduino.
- ESP32 (DOIT DevKit o equivalente).
- 18 servomotores + 2x módulos driver PCA9685 (I2C).
- 1x driver L298N controlando hasta 2 motores DC (canal A y canal B).
- 2 finales de carrera (uno por extremo de la banda).
- Módulo de 4 relevadores (5 electroimanes).
- Botón de paro de emergencia físico.
- Librería de control I2C para PCA9685 (p. ej. *Adafruit PWM Servo Driver
  Library*) u otra equivalente.

---

## Subsistema 1 — Brazos robóticos

### Estructura de archivos por sección

| Sección | Archivo de ángulos (Capa 1) | Lógica de la sección (Capa 2) | Brazos involucrados |
|---|---|---|---|
| Sección 1 | `ConfigSeccion1.h/.cpp` | `Seccion1.h/.cpp` | Brazo1, Brazo2 |
| Sección 2 | `ConfigSeccion2.h/.cpp` | `Seccion2.h/.cpp` | Brazo3, Brazo4 |
| Sección 3 | `ConfigSeccion3.h/.cpp` | `Seccion3.h/.cpp` | Brazo5, Brazo6 |

Todo lo que no esté en esta tabla (`Postura.h`, `BrazoRobotico.*`,
`RutinaRunner.*`, `MotorL298N.*`, `CeldaRobotica.ino`) es núcleo
compartido: se lee para entender el funcionamiento general, pero no se
edita para ajustar el comportamiento de una sección en particular.

### El tipo `Postura`

Cada posición del brazo se representa con la estructura `Postura`,
definida en `Postura.h`:

```cpp
struct Postura {
  float n1;         // Base   (0-180°, 90° = neutro)
  float n2;         // Hombro (0-180°, 90° = neutro)
  float n3;         // Codo   (0-180°, 90° = neutro)
  uint16_t pausaMs; // tiempo quieto en esta postura (0 = sin pausa)
};
```

### Rutinas: ejemplo

Dentro de cada `ConfigSeccionN.cpp` se definen uno o más arreglos de este
tipo (ejemplo de la Sección 1):

```cpp
const Postura RUTINA_SOLDAR[] = {
//  N1   N2   N3   pausaMs
  { 45,  60,  30,  0   },  // 1. Acercarse
  { 45,  90,  90,  0   },  // 2. Bajar electrodo
  { 45,  90,  120, 200 },  // 3. Soldar (pausa 200ms)
  { 45,  90,  90,  0   },  // 4. Levantar
  { 90,  90,  90,  0   },  // 5. Home
};
const uint8_t RUTINA_SOLDAR_LEN = sizeof(RUTINA_SOLDAR) / sizeof(Postura);
```

### Modificar una rutina existente

1. Localiza el arreglo correspondiente en el `ConfigSeccionN.cpp` de la
   sección a modificar.
2. Ajusta los valores de la fila deseada, agrega una fila nueva donde se
   necesite, o elimina una fila que ya no se use.
3. **No calcules `..._LEN` manualmente**: siempre se obtiene con
   `sizeof(arreglo) / sizeof(Postura)`.

### Agregar una rutina nueva a una sección existente

1. En el `ConfigSeccionN.h` correspondiente, declara el par:
   ```cpp
   extern const Postura MI_RUTINA_NUEVA[];
   extern const uint8_t MI_RUTINA_NUEVA_LEN;
   ```
2. En el `ConfigSeccionN.cpp`, defínelo igual que las rutinas existentes.
3. En el `SeccionN.h`, declara una función nueva, por ejemplo
   `void miRutinaNueva(BrazoRobotico &brazoX, ...);`
4. En el `SeccionN.cpp`, impleméntala así:
   ```cpp
   void miRutinaNueva(BrazoRobotico &brazoX) {
     BrazoRobotico* grupo[1] = { &brazoX };
     RutinaRunner::ejecutar(grupo, 1, MI_RUTINA_NUEVA, MI_RUTINA_NUEVA_LEN);
   }
   ```
5. Agrega la llamada correspondiente en `CeldaRobotica.ino`, o coordina
   este paso con el equipo de mantenimiento del proyecto.

### Escalar el proyecto: agregar una nueva sección

1. Replica el patrón completo de una sección existente:
   `ConfigSeccion4.h/.cpp` + `Seccion4.h/.cpp`.
2. En `CeldaRobotica.ino`, instancia los brazos nuevos y agrega las
   llamadas a la nueva sección en `setup()` y `loop()`.

---

## Subsistema 2 — Banda transportadora

La banda avanza en tramos cronometrados; al final de cada tramo se
detiene para que los brazos de la sección correspondiente ejecuten su
rutina, y vuelve a avanzar. Al llegar a cualquiera de los extremos, un
final de carrera invierte el sentido de giro automáticamente.

![Lógica de la banda transportadora](img/flujo_banda.png)

### Componentes

- **`MotorL298N.h/.cpp`** — clase genérica que controla un solo canal
  (A o B) del driver L298N: velocidad por PWM y sentido de giro. No sabe
  nada de finales de carrera ni de etapas; sirve como base reutilizable
  para cualquier motor conectado al L298N.
- **`Banda.h/.cpp`** — usa un `MotorL298N` internamente y agrega la
  lógica propia de la banda: avanzar por tramos cronometrados y detectar
  los finales de carrera para invertir el sentido.

El driver L298N tiene dos canales independientes. El canal A ya está
asignado a la banda transportadora; el canal B queda disponible para un
segundo motor (por ejemplo, el motor con encoder pendiente de conectar)
instanciando otro `MotorL298N` directamente, sin tocar la lógica de
`Banda`.

### Uso típico en `CeldaRobotica.ino`

```cpp
#include "Banda.h"
#include "MotorL298N.h"

Banda banda(25, 26, 27, 32, 33);   // Motor A: banda transportadora
MotorL298N motorB(23, 18, 5);      // Motor B: por definir

const uint16_t TIEMPOS_ETAPA[] = { 1200, 1200, 1200, 1200 };
const uint8_t TIEMPOS_ETAPA_LEN = sizeof(TIEMPOS_ETAPA) / sizeof(uint16_t);

void setup() {
  banda.iniciar(200);   // velocidad PWM
  motorB.iniciar();
}

void loop() {
  for (uint8_t i = 0; i < TIEMPOS_ETAPA_LEN; i++) {
    banda.avanzarEtapa(TIEMPOS_ETAPA[i]);
    // Aquí se ejecuta la rutina de brazos de la sección i
  }
}
```

---

## Mapa de pines del ESP32

![Mapa de pines del ESP32](img/pines.png)

| Subsistema | Señal | GPIO |
|---|---|---|
| I2C — 2x PCA9685 (servos) | SDA / SCL | 21 / 22 |
| Serial con Raspberry Pi (UART2) | TX2 / RX2 | 17 / 16 |
| Banda — Motor A (L298N canal A) | ENA / IN1 / IN2 | 25 / 26 / 27 |
| | Fin de carrera avance / retroceso | 32 / 33 |
| Motor B (L298N canal B) | ENB / IN3 / IN4 | 23 / 18 / 5 |
| Relevadores (4 → 5 electroimanes) | RELE1–4 | 4, 13, 14, 19 |




