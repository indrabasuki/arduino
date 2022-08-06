const int relay1 = 5;
const int relay2 = 4;
const int relay3 = 0;
const int relay4 = 2;

void setup() {
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

}

void loop() {
  // Normally Open configuration, send LOW signal to let current flow
  // (if you're usong Normally Closed configuration send HIGH signal)
  digitalWrite(relay1, LOW);
  Serial.println("Current Flowing");
  delay(200); 
   digitalWrite(relay2, LOW);
  Serial.println("Current Flowing");
  delay(200); 
   digitalWrite(relay3, LOW);
  Serial.println("Current Flowing");
  delay(200); 
   digitalWrite(relay4, LOW);
  Serial.println("Current Flowing");
  delay(200); 
  
  // Normally Open configuration, send HIGH signal stop current flow
  // (if you're usong Normally Closed configuration send LOW signal)
  digitalWrite(relay1, HIGH);
  Serial.println("Current not Flowing");
  delay(200);
    digitalWrite(relay2, HIGH);
  Serial.println("Current not Flowing");
  delay(200);
    digitalWrite(relay3, HIGH);
  Serial.println("Current not Flowing");
  delay(200);
    digitalWrite(relay4, HIGH);
  Serial.println("Current not Flowing");
  delay(200);
}
