#include "garage_sensors.h"


void setupGarageSensors() {

  pinMode(MOTION_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  stepperMotor.setSpeed(60);

}


uint8_t readMotionSensor(){

  int motionValue = digitalRead(MOTION_PIN);

    
  // Si detecta movimiento, enciende la luz del garaje
  if(motionValue == 1){
    digitalWrite(LED_PIN, HIGH);  
    return 1;
  }

  // Si no, la apaga
  digitalWrite(LED_PIN, HIGH);
  return 0;

}

uint8_t checkCarInside() {
  usDistance = sr04.Distance();
  
  if (usDistance < DISTANCE_THRESHOLD) {

    Serial.println("The car is inside");
    return 1;      
  }

  Serial.println("The car is not inside");
  return 0;
}


void checkDoorCode(){

  char pressedKey = customKeypad.getKey();


  if (pressedKey){
    if ((pressedKey == '*')){
      if (code.length() > 0) code = code.substring(0, code.length() - 1);
      return;
    } 

    code += pressedKey;  // Añade la tecla pulsada a la secuencia actual
    Serial.println(code);
    printOnOLEDScreen("Code\n", code, 2, Adafruit5x7);

    if(code.length() == masterCode.length()){

      if(code == masterCode){
        printOnOLEDScreen("Code\n", "Success", 2, Adafruit5x7);

        if (isClosed) {
          Serial.println("Opening");
          openGarageDoor();
        } else {
          Serial.println("Closing");
          closeGarageDoor();
        }

      } else {
        printOnOLEDScreen("Code\n", "Failed", 2, Adafruit5x7);
        Serial.println("Código Incorrecto");  // Si la secuencia no es correcta, avisa al usuario
      }

      code = ""; // Reinicia la secuencia para una nueva entrada
    }
  }

}

uint8_t getDoorState(){
  uint8_t state = (isClosed) ? 0 : 1;
  return state;
}

void openGarageDoor(){
  stepperMotor.step(STEPS_PER_REV); // Abre la puerta
  isClosed = false;
}

void closeGarageDoor(){
  stepperMotor.step(-STEPS_PER_REV); // Abre la puerta
  isClosed = true;
}
