#define H_Input1 13
#define H_Input2 12
#define en 11

int light = 0;
int Closed = 0;

void setup() {
  Serial.begin(9600);
  pinMode(H_Input1, OUTPUT);
  pinMode(H_Input2, OUTPUT);
  pinMode(en, OUTPUT);
}

void loop() {
  if (Serial.available() >= sizeof(light)) {
    Serial.readBytes((char*)&light, sizeof(light));
    
    if (light > 350 && !Closed) {
    MotorClose();
    delay(1000);
    MotorStop();
    Closed = !Closed;
  }
  if (light < 350 && Closed) {
    MotorOpen();
    delay(1000);
    MotorStop();
    Closed = !Closed;
  }
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
