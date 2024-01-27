int photoresistorPin = A1;
int lightVal;
int ledPIN = 7;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(photoresistorPin, INPUT);
  pinMode(ledPIN, OUTPUT);
  digitalWrite(ledPIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  lightVal = analogRead(photoresistorPin);
  if(lightVal > 300){
    digitalWrite(ledPIN, HIGH);
  } else {
    digitalWrite(ledPIN, LOW);
  }
  delay(1000);
}
