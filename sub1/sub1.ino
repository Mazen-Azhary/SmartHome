#include <ros.h>
#include <std_msgs/Int8.h>
#include <Servo.h>
#include <EEPROM.h>
// Pin Definitions
#define RED 10
#define BUZZER 2
#define GasSensor A3
#define echo 5
#define trig 6
#define LDR_pin A0
#define ServoMinimumDistance 50
#define LDRThreshold 200
#define DangerousGasThreshold 40
#define H1 7
#define H2 8
#define EN 6
#define WHITE 5
// Message Symbols to be received from publisher
#define ServoOnSymbol 1
#define ServoOffSymbol 2
#define GasSymbol 3
#define CurtainsOnSymbol 4
#define CurtainsOffSymbol 5
//
int lightIntensity = 0;
float dist = 0;
int servosignal = 5;
Servo myServo1;
int pos1 = 0;
int pos = 0;

void SwitchCaseFunction(const std_msgs::Int8& Received_Message) {
  int Order = Received_Message.data;
  switch (Order) {
    case ServoOnSymbol:  
      servo_on();
      break;
    case ServoOffSymbol: 
      servo_off();
      break;
    case GasSymbol: 
     
     break;
    case CurtainsOnSymbol:
    motoropen();
    break;
    case CurtainsOffSymbol:
    motorclose();
      break;
    default:
      // Handle default case
      break;
  }
}

ros::NodeHandle nh;
std_msgs::Int8 Received_Message;
ros::Subscriber<std_msgs::Int8> Arduino2("Orders Channel", &SwitchCaseFunction);



void setup() {
  nh.initNode();
  nh.subscribe(Arduino2);
  myServo1.attach(servosignal);
  pinMode(WHITE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(H1, OUTPUT);
  pinMode(H2, OUTPUT);
  pinMode(EN, OUTPUT);
  attachInterrupt(servosignal, interruptFunction, RISING);
}

void loop() {
  nh.spinOnce();
  delay(1000);
}

// Servo On and Off functions
void servo_on() {
  for (pos1 = 0; pos1 <= 90; pos1 += 1) {
    myServo1.write(pos1);
    delay(5);
  }
  for (pos1 = 90; pos1 >= 0; pos1 -= 1) {
    myServo1.write(pos1);
    delay(2);
  }
}

void servo_off() {
  myServo1.write(0);
  delay(2);
}

void interruptFunction() {
  char receivedPassword[50];
  // Implement the interrupt function
}

void motorclose() {
   digitalWrite(WHITE,LOW);
  analogWrite(EN, 100);
  digitalWrite(H2, HIGH);
  digitalWrite(H1, LOW);
}

void motoropen() {
  digitalWrite(WHITE,HIGH);
  analogWrite(EN, 100);
  digitalWrite(H1, HIGH);
  digitalWrite(H2, LOW);
}

void motorstop() {
   digitalWrite(WHITE,LOW);
  analogWrite(EN, 0);
  digitalWrite(H1, LOW);
  digitalWrite(H2, LOW);
}

