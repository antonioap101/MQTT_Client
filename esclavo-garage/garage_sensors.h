#ifndef GARAGE_SENSORS_H
#define GARAGE_SENSORS_H

#include <SR04.h>
#include <Stepper.h>
#include <Keypad.h>

#define TRIG_PIN 6
#define ECHO_PIN 1
#define DISTANCE_THRESHOLD 50                        // Umbral de distancia en cm
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long usDistance;                                        // Distancia detectada

#define MOTION_PIN 0
#define LED_PIN 4

#define ROLE_PER_MINUTE 15                          // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
const int STEPS_PER_REV = 300;                         // change this to fit the number of steps per revolution
Stepper stepperMotor(STEPS_PER_REV, 2, 3, 4, 5);      // initialize the stepper library on pins 8 through 11:
bool isClosed = false;

#define ROWS 4 //four rows
#define COLS 4 //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Reconfigura los pines de las filas para que coincidan con las columnas anteriores
byte rowPins[ROWS] = {7, 8, 9, 10}; 
// Reconfigura los pines de las columnas para que coincidan con las filas anteriores
byte colPins[COLS] = {20, 19, 13, 14};

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

String code = "";
String masterCode = "1234"; // la secuencia que abre la cerradura

void setupGarageSensors();
uint8_t readMotionSensor();
uint8_t checkCarInside();
void checkDoorCode();
void openGarageDoor();
void closeGarageDoor();

#endif // GARAGE_SENSORS_H
