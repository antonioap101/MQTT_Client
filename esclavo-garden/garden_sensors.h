#ifndef GARDEN_SENSORS_H
#define GARDEN_SENSORS_H

#define PHOTORESISTOR_PIN A1
#define LED_PIN 7

#define GROUND_PIN A2

void setupGardenSensors();
uint8_t readLightStatus();
uint8_t readGroundHumidity();

#endif // GARDEN_SENSORS_H

