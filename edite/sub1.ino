#include <ros.h>
#include <std_msgs/Int8.h>
#include <Servo.h>
#include <EEPROM.h>
// Pin Definitions
#define RED 10
#define BUZZER 2
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

int roomservosignal = 5;
Servo roomservo;
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
  myServo1.attach(roomservosignal);
  pinMode(WHITE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(H1, OUTPUT);
  pinMode(H2, OUTPUT);
  pinMode(EN, OUTPUT);
}

void loop() {
  nh.spinOnce();
  delay(1000);
}

// Servo On and Off functions
void servo_on() {
  for (pos1 = 0; pos1 <= 90; pos1 += 1) {
    roomservo.write(pos1);
    delay(5);
  }
  
}

void servo_off() {
  for (pos1 = 90; pos1 >= 0; pos1 -= 1) {
    roomservo.write(pos1);
    delay(2);
  }
}


void motorclose() {
   digitalWrite(WHITE,LOW);
  analogWrite(EN, 100);
  digitalWrite(H2, HIGH);
  digitalWrite(H1, LOW);  //--------------------------------------------- put delay to see the change----------------------------------------------------------

}

void motoropen() {
  digitalWrite(WHITE,HIGH);
  analogWrite(EN, 100);
  digitalWrite(H1, HIGH);
  digitalWrite(H2, LOW);  //--------------------------------------------- put delay to see the change-----------------------------------------------
}

void motorstop() {
   digitalWrite(WHITE,LOW);
  analogWrite(EN, 0);
  digitalWrite(H1, LOW);
  digitalWrite(H2, LOW);
}
