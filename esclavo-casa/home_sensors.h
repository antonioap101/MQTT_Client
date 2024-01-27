#ifndef HOME_SENSORS_H
#define HOME_SENSORS_H

#include <DHT.h>

#define DHTTYPE DHT11

#define GAS_PIN 0
#define FLAME_PIN 1
#define BLUE_PIN 2
#define GREEN_PIN 3
#define RED_PIN 4
#define DHTPIN 5
#define BUZZER_PIN 6


// Declaración de funciones
void setupHomeSensors();
uint8_t readHumidity();
uint8_t readTemperature();
uint8_t readGasSensor();
uint8_t readFlameSensor();
void activateBuzzer();
void deactivateBuzzer();

DHT dht(DHTPIN, DHTTYPE);
int valFlame, valGas;


#endif // HOME_SENSORS_H
