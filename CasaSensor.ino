#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 5

DHT dht(DHTPIN, DHTTYPE);

int valGas;
int valFlame;
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  delay(2000);
  readTemperatureAndHumidity();
  readGasSensor();

}

void readTemperatureAndHumidity() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if(isnan(humidity) || isnan(temperature)){
    Serial.println("ERROR EN EL SENSOR");
    return;
  }

  if (temperature < 20 ){
    analogWrite(2, 255);  //Blue
    analogWrite(3, 0);    //Green
    analogWrite(4, 0);    //Red
    //Serial.println("FRIO");
  } else if(temperature > 32) {
    analogWrite(2, 0);    //Blue
    analogWrite(3, 0);    //Green
    analogWrite(4, 255);  //Red
    //Serial.println("CALOR");
  }else{
    analogWrite(2, 0);    //Blue  
    analogWrite(3, 255);  //Green
    analogWrite(4, 255);  //Red
    //Serial.println("NORMAL");
  }

  Serial.print("Humidity: ");
  Serial.print(humidity,0);
  Serial.print("%");
  Serial.print("\tTemperature: ");
  Serial.print(temperature,0);
  Serial.println("C");
}

void readGasSensor(){
  valGas=analogRead(0);//Read Gas value from analog 0
  Serial.print("Lectura del gas: ");
  Serial.println(valGas,DEC);//Print the value to serial port
  if(valGas > 160) {activateBuzzer();}
  if(digitalRead(6)==HIGH && valGas<=160){deactivateBuzzer();}
}

void activateBuzzer(){
  digitalWrite(6,HIGH);
}

void deactivateBuzzer(){
  digitalWrite(6,LOW);
}

