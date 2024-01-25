int pirPin = 7; // Input for HC-S501

int pirValue = 0; // Place to store read PIR Value


void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pirPin, INPUT);
 
  digitalWrite(LED_BUILTIN, LOW);
  
  
  
}

void loop() {
  pirValue = digitalRead(pirPin);
  digitalWrite(LED_BUILTIN, pirValue);
  Serial.println(pirValue);

}