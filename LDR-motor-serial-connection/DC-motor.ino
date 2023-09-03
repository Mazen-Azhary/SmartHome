#define H_Input1 13
#define H_Input2 12
#define en 11

char receivedChar = ' ';
boolean newData = false; 

void setup() {
  Serial.begin(9600); 
  pinMode(H_Input1, OUTPUT);
  pinMode(H_Input2, OUTPUT);
  pinMode(en, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    receivedChar = Serial.read();
    newData = true; 
  }
  
  if (newData) {
    // Handle the received character
    Serial.println(receivedChar);
    if (receivedChar == '1') {
      MotorClose();
      delay(1000);
      MotorStop();
    } else if (receivedChar == '0') {
      MotorOpen();
      delay(1000);
      MotorStop();
    }
    newData = false; // Reset the newData flag
  }
}

void MotorOpen() {
  analogWrite(en, 100);
  Serial.println("Motor Opening");
  digitalWrite(H_Input1, HIGH);
  digitalWrite(H_Input2, LOW);
}

void MotorClose() {
  analogWrite(en, 100);
  Serial.println("Motor Closing");
  digitalWrite(H_Input1, LOW);
  digitalWrite(H_Input2, HIGH);
}

void MotorStop() {
  analogWrite(en, 0);
  Serial.println("Motor Stopped");
  digitalWrite(H_Input1, LOW);
  digitalWrite(H_Input2, LOW);
}
