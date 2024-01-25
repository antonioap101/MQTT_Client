

void setupPIRSensor() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

int readPIRSensor() {
  int pirValue = digitalRead(PIR_PIN);
  digitalWrite(LED_BUILTIN, pirValue);
  Serial.println("READ FROM readPIRSensor: " + String(pirValue));
  return pirValue; // Devuelve el valor leído del sensor PIR
}
