#include "BrazoRobotico.h"

BrazoRobotico::BrazoRobotico(Adafruit_PWMServoDriver &driver,
                              uint8_t canalBase, uint8_t canalHombro, uint8_t canalCodo,
                              bool espejoBase, const char* nombre)
  : _driver(driver),
    _canalBase(canalBase), _canalHombro(canalHombro), _canalCodo(canalCodo),
    _espejoBase(espejoBase), _nombre(nombre),
    _actualN1(90), _actualN2(90), _actualN3(90) {}

uint16_t BrazoRobotico::anguloAPulso(float grados) const {
  grados = constrain(grados, ANGULO_MIN, ANGULO_MAX);
  // map() de Arduino trabaja con enteros (long); se multiplica x10 antes de
  // mapear para conservar un decimal de resolución angular.
  long gradosX10 = (long)(grados * 10.0f);
  return (uint16_t) map(gradosX10, (long)ANGULO_MIN * 10, (long)ANGULO_MAX * 10,
                         (long)PULSO_MIN, (long)PULSO_MAX);
}

float BrazoRobotico::aplicarEspejo(float grados) const {
  // Invierte alrededor del punto neutro (90°): un espejo real de la rotación
  // de la base, no solo un desplazamiento de rango.
  return _espejoBase ? (float(ANGULO_MAX) - grados) : grados;
}

void BrazoRobotico::escribirCanal(uint8_t canal, float grados) const {
  grados = constrain(grados, ANGULO_MIN, ANGULO_MAX);
  _driver.setPWM(canal, 0, anguloAPulso(grados));
}

void BrazoRobotico::moverBase(float grados) {
  _actualN1 = grados;
  escribirCanal(_canalBase, aplicarEspejo(grados));
}

void BrazoRobotico::moverHombro(float grados) {
  _actualN2 = grados;
  // N2 y N3 no requieren inversión: al enfrentar dos brazos, la base cambia
  // de "lado" pero el hombro y el codo mantienen su sentido de giro relativo.
  escribirCanal(_canalHombro, grados);
}

void BrazoRobotico::moverCodo(float grados) {
  _actualN3 = grados;
  escribirCanal(_canalCodo, grados);
}

void BrazoRobotico::moverA(float anguloN1, float anguloN2, float anguloN3) {
  moverBase(anguloN1);
  moverHombro(anguloN2);
  moverCodo(anguloN3);
}

void BrazoRobotico::irAHome() {
  moverA(90, 90, 90);
}

// Convierte un avance lineal (0.0 -> 1.0) en una curva de aceleración/
// desaceleración suave ("smoothstep"), en vez de velocidad constante.
// Esto es lo que evita que el servo arranque y frene de golpe en cada
// postura: al inicio y al final del tramo se mueve despacio, y en medio
// alcanza su velocidad máxima — igual que un movimiento humano o el de un
// brazo industrial real con rampas de aceleración.
static float suavizar(float t) {
  return t * t * (3.0f - 2.0f * t);
}

void BrazoRobotico::moverGrupoSincronizado(BrazoRobotico* brazos[], uint8_t cantidad,
                                            const float n1[], const float n2[], const float n3[],
                                            uint8_t pasos, uint16_t msPorPaso) {
  if (cantidad > MAX_GRUPO) cantidad = MAX_GRUPO; // protección básica de límites

  // Punto de partida real de cada brazo, para interpolar "desde donde está"
  float inicioN1[MAX_GRUPO], inicioN2[MAX_GRUPO], inicioN3[MAX_GRUPO];
  for (uint8_t i = 0; i < cantidad; i++) {
    inicioN1[i] = brazos[i]->_actualN1;
    inicioN2[i] = brazos[i]->_actualN2;
    inicioN3[i] = brazos[i]->_actualN3;
  }

  if (pasos == 0) pasos = 1; // evita división entre cero

  for (uint8_t paso = 1; paso <= pasos; paso++) {
    float t = (float) paso / (float) pasos; // 0.0 -> 1.0 (avance lineal en el tiempo)
    float tSuave = suavizar(t);             // 0.0 -> 1.0 (avance en el ángulo, con rampa)

    // Se comandan TODOS los brazos del grupo en cada paso antes de esperar,
    // para que avancen en el mismo instante y no uno después del otro.
    for (uint8_t i = 0; i < cantidad; i++) {
      float a1 = inicioN1[i] + (n1[i] - inicioN1[i]) * tSuave;
      float a2 = inicioN2[i] + (n2[i] - inicioN2[i]) * tSuave;
      float a3 = inicioN3[i] + (n3[i] - inicioN3[i]) * tSuave;
      brazos[i]->moverA(a1, a2, a3);
    }
    delay(msPorPaso);
  }
}
