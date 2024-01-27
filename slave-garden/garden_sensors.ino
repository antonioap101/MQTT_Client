#include "garden_sensors.h"


void setupGardenSensors() {
  // put your setup code here, to run once:
  pinMode(PHOTORESISTOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
}


uint8_t readLightStatus() {
  int lightVal = analogRead(PHOTORESISTOR_PIN);

  // EstÃ¡ muy oscuro
  if(lightVal > 300){
    digitalWrite(LED_PIN, HIGH);  // Enciende el LED
    return 1;
  }
  
  // Hay suficiente luz
  digitalWrite(LED_PIN, LOW);     // Apaga el LED
  return 0;
}

uint8_t readGroundHumidity() {
  return analogRead(GROUND_PIN);
}
