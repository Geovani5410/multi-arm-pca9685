# Cómo entender la estructura de este proyecto
hola peralta
Esta guía está pensada para que cualquiera del equipo, al abrir el repositorio
por primera vez, sepa en qué archivo debe trabajar y cuáles nunca debe tocar.
Léela en orden la primera vez; después solo necesitarás el Paso 2 como
referencia rápida.

## Paso 1: Entiende las 3 capas del proyecto

El código está organizado en 3 capas. Entre más abajo está una capa, menos
frecuentemente debería cambiar:

```
┌─────────────────────────────────────────────────────────┐
│ CAPA 1 — CONFIGURACIÓN POR ETAPA   (la editas tú)        │
│   ConfigSeccion1.h/.cpp   ConfigSeccion2.h/.cpp   ...    │
│   → Solo arreglos de Postura (ángulos + pausas)          │
├─────────────────────────────────────────────────────────┤
│ CAPA 2 — PEGAMENTO POR ETAPA        (rara vez la tocas)  │
│   Seccion1.h/.cpp   Seccion2.h/.cpp   Seccion3.h/.cpp    │
│   → Nombra las rutinas y las conecta con RutinaRunner    │
├─────────────────────────────────────────────────────────┤
│ CAPA 3 — NÚCLEO COMPARTIDO          (NO LA TOCAS sin     │
│   Postura.h                          avisar al equipo)   │
│   BrazoRobotico.h/.cpp                                   │
│   RutinaRunner.h/.cpp                                    │
│   CeldaRobotica.ino                                      │
└─────────────────────────────────────────────────────────┘
```

**Idea clave:** el 95% de tu trabajo diario ocurre en la Capa 1. Si te
encuentras editando algo en la Capa 3, detente y pregúntale al equipo primero
— ese archivo afecta a todas las secciones, no solo a la tuya.

## Paso 2: Identifica qué archivos son "tuyos"

Busca tu etapa en esta tabla y memoriza solo esa fila:

| Tu etapa | Tu archivo de ángulos (Capa 1) | Tu pegamento (Capa 2) | Brazos involucrados |
|---|---|---|---|
| Sección 1 (soldadura, espejo) | `ConfigSeccion1.h/.cpp` | `Seccion1.h/.cpp` | Brazo1, Brazo2 |
| Sección 2 (etapa independiente) | `ConfigSeccion2.h/.cpp` | `Seccion2.h/.cpp` | Brazo3, Brazo4 |
| Sección 3 (ensamble de puerta) | `ConfigSeccion3.h/.cpp` | `Seccion3.h/.cpp` | Brazo5, Brazo6 |

Todo lo que no esté en esta tabla (`Postura.h`, `BrazoRobotico.*`,
`RutinaRunner.*`, `CeldaRobotica.ino`) es núcleo compartido: lo lees para
entender cómo funciona, pero no lo editas para ajustar tu rutina.

## Paso 3: Entiende el tipo `Postura` antes de tocar cualquier arreglo

Abre `Postura.h`. Vas a ver esto:

```cpp
struct Postura {
  float n1;         // Base   (0-180°, 90° = neutro)
  float n2;         // Hombro (0-180°, 90° = neutro)
  float n3;         // Codo   (0-180°, 90° = neutro)
  uint16_t pausaMs; // tiempo quieto en esta postura (0 = sin pausa)
};
```

Cada fila de tus rutinas es una de estas 4 cifras. No necesitas entender nada
más del código para empezar a ajustar ángulos.

## Paso 4: Abre tu `ConfigSeccionN.cpp` y ubica tus rutinas

Dentro encontrarás uno o más arreglos como este (ejemplo de Sección 1):

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

Para cambiar el movimiento:
1. Modifica los números de la fila que quieras ajustar, o
2. Agrega una fila nueva donde la necesites (antes o después de otra), o
3. Borra una fila que ya no uses.

En los tres casos, **no toques `..._LEN`**: se calcula solo con `sizeof(...)`.

## Paso 5: Prueba tu cambio sin afectar al resto del equipo

Como tu `ConfigSeccionN.cpp` es un archivo distinto al de las demás
secciones, puedes compilar y subir tu cambio sin riesgo de romper el trabajo
de tus compañeros: `RutinaRunner` y `BrazoRobotico` son genéricos y no saben
nada de los valores concretos que pusiste.

## Paso 6: Si necesitas una rutina *nueva* (no solo ajustar ángulos)

Esto sí toca la Capa 2, pero sigue siendo solo tu sección:

1. En tu `ConfigSeccionN.h`, declara el par:
   ```cpp
   extern const Postura MI_RUTINA_NUEVA[];
   extern const uint8_t MI_RUTINA_NUEVA_LEN;
   ```
2. En tu `ConfigSeccionN.cpp`, defínelo igual que las rutinas existentes.
3. En tu `SeccionN.h`, declara una función nueva, por ejemplo
   `void miRutinaNueva(BrazoRobotico &brazoX, ...);`
4. En tu `SeccionN.cpp`, impleméntala así:
   ```cpp
   void miRutinaNueva(BrazoRobotico &brazoX) {
     BrazoRobotico* grupo[1] = { &brazoX };
     RutinaRunner::ejecutar(grupo, 1, MI_RUTINA_NUEVA, MI_RUTINA_NUEVA_LEN);
   }
   ```
5. Avisa al equipo para que agreguen la llamada en `CeldaRobotica.ino`
   (ese archivo sí es compartido).

## Paso 7: Si el equipo crece y necesitan una Sección 4

1. Copien el patrón completo de cualquier sección existente:
   `ConfigSeccion4.h/.cpp` + `Seccion4.h/.cpp`.
2. En `CeldaRobotica.ino`, instancien los brazos nuevos y agreguen las
   llamadas a la nueva sección en `setup()` y `loop()`.

Ningún archivo de las Secciones 1, 2 o 3 necesita cambiar.

## Reglas que no deberías romper

- Nunca edites `BrazoRobotico.*`, `RutinaRunner.*` o `Postura.h` para ajustar
  el comportamiento de tu sección — si crees que necesitas hacerlo, probable-
  mente lo que buscas ya es posible desde tu `ConfigSeccionN.cpp`.
- No cuentes `..._LEN` a mano; siempre `sizeof(arreglo) / sizeof(Postura)`.
- Mantén los nombres de brazos consistentes con `CeldaRobotica.ino`
  (Brazo1...Brazo6), para que cualquiera del equipo sepa a qué brazo físico
  corresponde cada dato.
- Usa el prefijo `POSICION_INICIAL_...` para la postura de reposo y
  `RUTINA_...` para cualquier secuencia con propósito, así los nombres son
  predecibles entre secciones.
