#include <LiquidCrystal.h>

const int rs = 6, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  lcd.begin(16,2);
  lcd.print("Temperature: ");
  lcd.setCursor(14,0);
  lcd.print("C");
}

void loop() {
  int celsius = 0;
  float suma = 0;
  for(int i = 0; i<5; i++){
    int tempLecture = analogRead(A0);
    celsius = int(((tempLecture * 3300) / 1024) / 10);
    suma = celsius + suma;
  }
  celsius = suma/5;
  print_Temp(celsius);
  Serial.print("Hay ");
  Serial.print(celsius,0);
  Serial.println(" ºC Grados Celsius");
  if (celsius < 20 ){
    analogWrite(2, 255);  //Blue
    analogWrite(3, 0);    //Green
    analogWrite(4, 0);    //Red
    Serial.println("FRIO");
  } else if(celsius > 32) {
    analogWrite(2, 0);    //Blue
    analogWrite(3, 0);    //Green
    analogWrite(4, 255);  //Red
    Serial.println("CALOR");
  }else{
    analogWrite(2, 0);    //Blue  
    analogWrite(3, 255);  //Green
    analogWrite(4, 255);  //Red
    Serial.println("NORMAL");
  }
  delay(500);
}

void print_Temp(int temperature) {
  Serial.print(temperature);
  lcd.setCursor(12,0);
  lcd.print(temperature);
}
