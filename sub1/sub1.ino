#include <ros.h>
#include <Servo.h>
#include <std_msgs/Int32.h>

//define the led and the buzzer
#define RED 10
#define Buzzer 2

//define the servo
Servo myServo;
#define ServoPin 4

//define the DC-motor
#define H1 7
#define H2 8
#define EN 6

//initializing variables
int light = 0;
int flag = 0;
int pos = 0;
int closed = 0;
int servo1 = 0;

ros::NodeHandle nh;

std_msgs::Int32 gas_emergency_msg;
std_msgs::Int32 servo_msg;
std_msgs::Int32 light_msg;



//Servo CallBack function
void servoCallback(const std_msgs::Int32& msg)
{
  servo1 = msg.data;

  if (servo1 < 15 && flag == 0)
  {
    servo_on();
    delay(1000);
    flag = 1;
  }

  if (servo1 > 15 && flag == 1)
  {
    servo_off();
    delay(1000);
    flag = 0;
  }
}

// Gas Sensor callBack function
void gasEmergencyCallback(const std_msgs::Int32& msg) {
  gas_emergency_msg = msg;
}


// LDR CallBack function
void lightCallback(const std_msgs::Int32& msg)
{
  light = msg.data;

  if (light > 200 && closed == 0)
  {
    motoropen();
    delay(1000);
    motorstop();
    flag = 1;
  }

  if (light < 200 && closed == 1)
  {
    motorclose();
    delay(1000);
    motorstop();
    flag = 0;
  }
}




ros::Subscriber<std_msgs::Int32> gas_emergency_sub("gas_sensor_reading", &gasEmergencyCallback);
ros::Subscriber<std_msgs::Int32> servo_sub("ultrasonic", &servoCallback);
ros::Subscriber<std_msgs::Int32> LDR_sub("LDR_Topic", &lightCallback);


void setup() {
  nh.initNode();
  Serial.begin(9600);

  myServo.attach(ServoPin);
  nh.subscribe(servo_sub);

  pinMode(RED, OUTPUT);
  digitalWrite(RED, LOW);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);
  nh.subscribe(gas_emergency_sub);

  pinMode(H1, OUTPUT);
  pinMode(H2, OUTPUT);
  pinMode(EN, OUTPUT);
  nh.subscribe(LDR_sub);


}

void loop() {
  if (gas_emergency_msg.data > 40) {
    digitalWrite(RED, HIGH);
    digitalWrite(Buzzer, HIGH);
    servo_on();
    delay(10000);
  }
  else if (gas_emergency_msg.data > 25) {
    digitalWrite(RED, LOW);
    digitalWrite(Buzzer, LOW);
  } else {
    digitalWrite(RED, LOW);
    digitalWrite(Buzzer, LOW);
  }

  
  nh.spinOnce();
  delay(1000);
}


// Servo On and Off functions
void servo_on() {
  for (pos = 0; pos <= 90; pos += 1) {
    myServo.write(pos);
    delay(5);
  }
  for (pos = 90; pos >= 0; pos -= 1) {
    myServo.write(pos);
    delay(2);
  }
}
void servo_off() {
  myServo.write(0);
  delay(2);
}


// Motor On,Off and Stop Functions
void motorclose()
{
  analogWrite(EN, 100);
  digitalWrite(H2, HIGH);
  digitalWrite(H1, LOW);
  delay(1000);
}

void motoropen()
{
  analogWrite(EN, 100);
  digitalWrite(H1, HIGH);
  digitalWrite(H2, LOW);
  delay(1000);
}

void motorstop()
{
  analogWrite(EN, 0);
  digitalWrite(H1, LOW);
  digitalWrite(H2, LOW);
}
