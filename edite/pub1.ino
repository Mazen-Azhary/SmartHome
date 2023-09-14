#include <ros.h>
#include <std_msgs/Int8.h>

// Pin Definitions
#define RED 10
#define BUZZER 2
#define GasSensor A3
#define echo 5
#define trig 6
#define LDR_pin A0

// Thresholds and Constants
#define ServoMinimumDistance 10
#define LDRThreshold 200
#define DangerousGasThreshold 40
// Message Symbols to be sent to subscriber switchcase function
#define ServoOnSymbol 1
#define ServoOffSymbol 2
#define GasSymbol 3
#define CurtainsOnSymbol 4
#define CurtainsOffSymbol 5

int lightIntensity = 0;
int dist = 0;
int measurementTime =0 ;

ros::NodeHandle nh;
std_msgs::Int8 PublisherMessage;
ros::Publisher Arduino1("Orders_Channel", &PublisherMessage);//one topic only where symbols will be sent



void setup() {
  nh.initNode();
  pinMode(RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LDR_pin, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  attachInterrupt(digitalPinToInterrupt(BUZZER ), gasInterrupt, CHANGE); //use interuppt when the gas sensor run
}



void loop() {
  ultrasonicSensor();
  gasFunction();
  LDRFunction();
  nh.spinOnce();
}




void ultrasonicSensor() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
   measurementTime = pulseIn(echo, HIGH);
  dist = measurementTime * 0.034 / 2;

  while(dist < ServoMinimumDistance) {
    publishMessage(ServoOnSymbol);//I don't know if this is the best approach but I did a while object is close keep sending servo_on symbol
  }
    publishMessage(ServoOffSymbol);
}




void gasFunction() {
  int gasReading = analogRead(GasSensor);
}
void servoInterrupt(){
 if (gasReading >= DangerousGasThreshold) {
    publishMessage(GasSymbol);}
}





void LDRFunction() {
  lightIntensity = analogRead(LDR_pin);

  if (lightIntensity > LDRThreshold) {
    publishMessage(CurtainsOnSymbol);
  } else {
    publishMessage(CurtainsOffSymbol);
  }
}
//----------------------------------------------??????????????????????????????????---------------------------
void publishMessage(int symbol) {
  PublisherMessage.data = symbol;
  Arduino1.publish(&PublisherMessage);
}