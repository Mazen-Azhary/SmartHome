#include <ros.h>
#include <std_msgs/Int32.h>

//defines for the Buzzer and the gasSensor
#define RED 10
#define BUZZER 2  
#define GasSensor A3
//////////
#define echo 5
#define trig 6
//defines for LDR 
#define LDR A0



//define variables used
int lightIntensity = 0;
int measurementTime = 0;
float dist = 0;

ros::NodeHandle nh;
std_msgs::Int32 gas_emergency_msg; // this is gas sensor message 
std_msgs::Int32 LDR_msg; //message containes the reading of the LDR
std_msgs::Int32 servo_msg; // this is ultrasonic message

ros::Publisher ultrasonic("ultrasonic", &servo_msg);
ros::Publisher gas_emergency_pub("gas_sensor_reading", &gas_emergency_msg);
ros::Publisher LDR("LDR_Topic", &LDR_msg);

void ultrasonic_sensor()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  measurementTime = pulseIn(echo, HIGH);
  dist = measurementTime * 0.034 / 2/10;
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");
}



void setup() {
  nh.initNode();
  pinMode(RED, OUTPUT);
  digitalWrite(RED, LOW);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  pinMode(LDR, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  nh.advertise(gas_emergency_pub);  //Advertise the publisher
  nh.advertise(ultrasonic);  //Advertise the ultrasonic
  nh.advertise(LDR);  //Advertise the LDR
  
  
}

void loop() {
  //initialize the ultrasonic sensor
  ultrasonic_sensor();
  //publish the ultrasonic msg
  servo_msg.data = dist;
  ultrasonic.publish(&servo_msg);

  //get the gas sensor reading
  gas_emergency_msg.data = analogRead(GasSensor);  
  // Publish the gas_emergency_msg
  gas_emergency_pub.publish(&gas_emergency_msg);

  //get the LDR reading
  lightIntensity = analogRead(LDR);
  LDR_msg.data = lightIntensity;
  //publish the LDR-reading
  LDR.publish(LDR_msg);
  
  delay(1000);
  nh.spinOnce();
}
