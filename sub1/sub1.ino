#include <ros.h>
#include <EEPROM.h>
#include <Servo.h>
#include <std_msgs/Int32.h>

//define the led and the buzzer
#define RED 10
#define Buzzer 2

//define the servo
Servo myServo;
#define ServoPin 4

//servo2 (bab el sha2a)
int servosignal=5;
Servo myServo1 ;
int pos1 =0 ;
//define the DC-motor
#define H1 7
#define H2 8
#define EN 6
#define WHITE 5
////////////////////
volatile bool isPasswordCorrect = false;
char storedPassword[] = "123"; 
//eeprom pass

//initializing variables
int light = 0;

//
int pos = 0;
int closed = 0;
int servo1 = 0;

ros::NodeHandle nh;

std_msgs::Int32 gas_emergency_msg;
std_msgs::Int32 servo_msg;
std_msgs::Int32 light_msg;



//ISR function
void interruptFunction() {
  char receivedPassword[50];
  for (int i = 0; i < sizeof(storedPassword); i++) {
    receivedPassword[i] = EEPROM.read(i);
  }
  if (strcmp(receivedPassword, storedPassword) == 0) {
    isPasswordCorrect = true;
  }
}




//Servo CallBack function
void servoCallback(const std_msgs::Int32& msg)
{
  servo1 = msg.data;

  if (servo1 < 15)
  {
    servo_on();
    delay(5000);
    
  }

  if (servo1 > 15 )
  {
    servo_off();
    delay(5000);
    
  }
}


void gasEmergencyCallback(const std_msgs::Int32& msg) {
  gas_emergency_msg = msg;
  if (gas_emergency_msg.data > 40) {
    digitalWrite(RED, HIGH);
    digitalWrite(Buzzer, HIGH);
    servo_on();
    servo2_on();
    motoropen();
    
    delay(10000);
  }
  else if (gas_emergency_msg.data > 25) {
    digitalWrite(RED, LOW);
    digitalWrite(Buzzer, LOW);
  } else {
    digitalWrite(RED, LOW);
    digitalWrite(Buzzer, LOW);
  }
}


// LDR CallBack function
void lightCallback(const std_msgs::Int32& msg)
{
  light = msg.data;

  if (light > 200 && closed == 0)
  {
    motoropen();
    delay(5000);
    motorstop();
     closed = !closed;
  }

  if (light < 200 && closed == 1)
  {
    motorclose();
    delay(5000);
    motorstop();
    closed = !closed;
  }
}




ros::Subscriber<std_msgs::Int32> gas_emergency_sub("gas_sensor_reading", &gasEmergencyCallback);
ros::Subscriber<std_msgs::Int32> servo_sub("ultrasonic", &servoCallback);
ros::Subscriber<std_msgs::Int32> LDR_sub("LDR_Topic", &lightCallback);


void setup() {
  nh.initNode();
  
  myServo.attach(ServoPin);
  nh.subscribe(servo_sub);
  
pinMode(WHITE, OUTPUT);
  pinMode(RED, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(WHITE, LOW);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);
  nh.subscribe(gas_emergency_sub);

  pinMode(H1, OUTPUT);
  pinMode(H2, OUTPUT);
  pinMode(EN, OUTPUT);
  nh.subscribe(LDR_sub);
attachInterrupt(ServoPin, interruptFunction, RISING);
// for some reason here the DigitalPinToInterrupt func was producing compilation errors here so we removed it 

}

void loop() {
  

  
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


void servo2_on(){
     	for(pos1=0 ;pos1<=90 ;pos1+=1) //make servo angle inc
        {myServo1.write(pos1);
           delay(5);}
         for(pos1=90 ;pos1>=0 ;pos1-=1) //make servo angle dec
            {
          myServo1.write(pos1);
                 delay(2);}
}
void servo2_off(){
  myServo1.write(0);
  
        delay(10);  }

// Motor On,Off and Stop Functions
void motorclose()
{
  digitalWrite(WHITE,LOW);
  analogWrite(EN, 100);
  digitalWrite(H2, HIGH);
  digitalWrite(H1, LOW);
  delay(1000);
}

void motoropen()
{
  digitalWrite(WHITE,HIGH);
  analogWrite(EN, 100);
  digitalWrite(H1, HIGH);
  digitalWrite(H2, LOW);
  delay(1000);
}

void motorstop()
{
    digitalWrite(WHITE,LOW);

  analogWrite(EN, 0);
  digitalWrite(H1, LOW);
  digitalWrite(H2, LOW);
}
