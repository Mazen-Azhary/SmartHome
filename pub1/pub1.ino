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
#define ServoMinimumDistance 50
#define LDRThreshold 200
#define DangerousGasThreshold 40

// Message Symbols to be sent to the subscriber switch-case function
#define ServoOnSymbol 1
#define ServoOffSymbol 2
#define GasSymbol 3
#define CurtainsOnSymbol 4
#define CurtainsOffSymbol 5

int lightIntensity = 0;
float dist = 0;
ros::NodeHandle nh;
std_msgs::Int8 PublisherMessage;
ros::Publisher Arduino1("Orders_Channel", &PublisherMessage);

void setup() {
  nh.initNode();
  pinMode(RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LDR_pin, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  GasCheckFunction(); // Check for gas reading first

  // If the gas reading is high, GasSymbol is sent and nothing else is executed
  if (PublisherMessage.data == GasSymbol) {
    delay(5000);
    nh.spinOnce(); 
    return; // Exit the loop now
  }

  ultrasonicSensor();
  LDRFunction();
  nh.spinOnce(); 
}

void ultrasonicSensor() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  unsigned long measurementTime = pulseIn(echo, HIGH);
  dist = measurementTime * 0.034 / 2;

  if (dist <= ServoMinimumDistance && dist > 0) {
    publishMessage(ServoOnSymbol);
  } else {
    publishMessage(ServoOffSymbol);
  }
}

void GasCheckFunction() {
  int gasReading = analogRead(GasSensor);

  if (gasReading >= DangerousGasThreshold) {
    publishMessage(GasSymbol); 
  }
}

void LDRFunction() {
  lightIntensity = analogRead(LDR_pin);
  if (lightIntensity > LDRThreshold) {
    publishMessage(CurtainsOnSymbol);
  } else {
    publishMessage(CurtainsOffSymbol);
  }
}

void publishMessage(int symbol) {
  PublisherMessage.data = symbol;
  Arduino1.publish(&PublisherMessage);
}