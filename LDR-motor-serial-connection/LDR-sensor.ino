#define LDR A0

char msg = ' ';
int Closed = 0;
int lightIntensity = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LDR, INPUT); // Set LDR pin as input
}

void loop() { 
  lightIntensity = analogRead(LDR);
  Serial.println(lightIntensity);
  delay(1000);
  
  if (lightIntensity > 350 && !Closed) {
    msg = '1';
    Serial.write(msg);
    delay(100);
    Closed = !Closed;
  } 
  if (lightIntensity < 350 && Closed) {
    msg = '0';
    Serial.write(msg);
    delay(100);
    Closed = !Closed;
  }
  delay(100);
}
