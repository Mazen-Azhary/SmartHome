#define LDR A0
#define H_Input1 13
#define H_Input2 12
#define en 11

int Closed = 1;
int lightIntensity = 0;

void setup() {
  Serial.begin(9600); //configure  serial to talk to computer
  pinMode(H_Input1, OUTPUT);
  pinMode(H_Input2, OUTPUT);
  pinMode(en, OUTPUT);
}

void  loop() {
  lightIntensity = analogRead(LDR);
  Serial.println(lightIntensity);

  if(lightIntensity>350 && !Closed)
  {
    MotorClose();
    delay(1000);
    MotorStop();
    Closed = !Closed;
  }
  if(lightIntensity<350 && Closed)
  {
    MotorOpen();
    delay(1000);
    MotorStop();
    Closed = !Closed;
  }


  delay(1000); // don't spam the computer!
}


void MotorOpen()
{
  analogWrite(en, 100);
  digitalWrite(H_Input1, HIGH);
  digitalWrite(H_Input2, LOW);
}

void MotorClose()
{
  analogWrite(en, 100);
  digitalWrite(H_Input1, LOW);
  digitalWrite(H_Input2, HIGH);
}

void MotorStop()
{
  analogWrite(en, 0);
  digitalWrite(H_Input1, LOW);
  digitalWrite(H_Input2, LOW);
}
