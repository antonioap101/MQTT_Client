#include "home_sensors.h"

void setupHomeSensors() {  
  dht.begin();
  pinMode(GAS_PIN,INPUT);
  pinMode(FLAME_PIN,INPUT);

  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
}

uint8_t readHumidity() {
  float humidity = dht.readHumidity();
  
  if (isnan(humidity)) {
    Serial.println("ERROR EN EL SENSOR DE HUMEDAD");
    return 255; // Valor de error
  }
  uint8_t humidity_byte = static_cast<uint8_t>(humidity);
  return humidity_byte;
}

uint8_t readTemperature() {     
  float readTemp = dht.readTemperature();
  
  if (isnan(readTemp)) {
    Serial.println("ERROR EN EL SENSOR DE TEMPERATURA");
    return 255; // Valor de error
  }

  // Aseguramos que la temperatura que se lee [-10, 40] -> [0, 50]
  float temperature = max(-10.0, min(readTemp, 40.0)) + 10;

  // 6 bits para la parte entera y 2 bits para la parte decimal
  int parte_entera = int(temperature); // Truncar la parte entera
  int parte_decimal = int((temperature - parte_entera) * 4); // Obtener la parte decimal escalada a 2 bits            
  
  uint8_t temperatura_byte = static_cast<uint8_t>((parte_entera << 2) | parte_decimal); // Combinar la parte entera y la parte decimal en un solo byte

  // Configurar el color del LED basado en la temperatura
  setLEDColor(readTemp);

  return temperatura_byte;
}

uint8_t readGasSensor(){
  valGas=analogRead(GAS_PIN);//Read Gas value from analog 0  
  
  if (!transmitting){
    if(valGas >= 160) {activateBuzzer();}
    if(digitalRead(BUZZER_PIN)==HIGH && valGas<180 && valFlame<400){deactivateBuzzer();}
  }

  uint8_t valGas_byte = static_cast<uint8_t>(valGas);
  return valGas_byte;
}

uint8_t readFlameSensor(){
  valFlame=analogRead(FLAME_PIN); // read the analog value of the sensor   
  if (!transmitting){
    if(valFlame >= 140) {activateBuzzer();}
    if(digitalRead(BUZZER_PIN)==HIGH && valFlame<400 && valGas<180){deactivateBuzzer();}
  }

  uint8_t valFlame_byte =  static_cast<uint8_t>(valFlame);
  return valFlame_byte;
}
void activateBuzzer(){
  digitalWrite(BUZZER_PIN,HIGH);
}

void deactivateBuzzer(){
  digitalWrite(BUZZER_PIN,LOW);
}


// Función para establecer el color del LED basado en la temperatura
void setLEDColor(float temperature) {  
  if (temperature < 20) {
    analogWrite(BLUE_PIN, 255);  // Blue
    analogWrite(GREEN_PIN, 0);   // Green
    analogWrite(RED_PIN, 0);     // Red
    //Serial.println("FRIO");
  } else if (temperature > 29) {
    analogWrite(BLUE_PIN, 0);    // Blue
    analogWrite(GREEN_PIN, 0);   // Green
    analogWrite(RED_PIN, 255);   // Red
    //Serial.println("CALOR");
  } else {
    analogWrite(BLUE_PIN, 0);    // Blue  
    analogWrite(GREEN_PIN, 255); // Green
    analogWrite(RED_PIN, 0);   // Red
    //Serial.println("NORMAL");
  }
}


