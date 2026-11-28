#include "SerialReporter.h"

void reportarMovimiento(const char* idBrazo, const Postura &p) {
  // Enteros para que coincida exactamente con el parser del servidor
  // (app.py espera 5 valores separados por coma, sin decimales).
  Serial.print(idBrazo);        Serial.print(',');
  Serial.print((int)p.n1);      Serial.print(',');
  Serial.print((int)p.n2);      Serial.print(',');
  Serial.print((int)p.n3);      Serial.print(',');
  Serial.println(p.pausaMs);
}
