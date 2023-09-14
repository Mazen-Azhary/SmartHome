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
#define ultrasonicMinimumDistance 10
#define LDRThreshold 200
#define DangerousGasThreshold 40
// Message Symbols to be sent to subscriber switchcase function
#define ServoOnSymbol 1 //symbol used to send it o the sub to start servo
#define ServoOffSymbol 2
#define GasSymbol 3
#define CurtainsOnSymbol 4
#define CurtainsOffSymbol 5



int gasReading;
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

  attachInterrupt(BUZZER, gasInterrupt, CHANGE); //use interuppt when the gas sensor run

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

  if (dist <= ultrasonicMinimumDistance && dist > 0) {
    publishMessage(ServoOnSymbol);
  } else {
    publishMessage(ServoOffSymbol);
  }
}




void gasFunction() {
   gasReading = analogRead(GasSensor);
}
void gasInterrupt(){
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

void publishMessage(int symbol) {      // func used to send data to the sub 
  PublisherMessage.data = symbol;
  Arduino1.publish(&PublisherMessage);
}
