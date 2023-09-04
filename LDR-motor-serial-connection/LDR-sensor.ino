#define LDR A0

int lightIntensity = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LDR, INPUT); // Set LDR pin as input
}

void loop() { 
  lightIntensity = analogRead(LDR);
  Serial.write((byte*)&lightIntensity, sizeof(lightIntensity));
 
  delay(100);
}

