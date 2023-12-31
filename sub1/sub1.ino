#include <ros.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Int32.h>
#include <Servo.h>
#include <EEPROM.h>
// Pin Definitions
#define H1 7  //H BRIDGE PIN
#define H2 8
#define EN 6
#define WHITE 5
/////////////////////////////////////////////////////////////
#define doorServoPin 3
Servo doorservo;    //door servo
const int passwordAddress = 0; // EEPROM address to store password
int storedPassword = 123; // Stored password
volatile bool receivedInterrupt = false; // Flag to indicate interrupt received
volatile bool passwordMatched = false; // Flag to indicate password matched
int pos2 = 0; // Determine the position of the door servo
int  entered_pass; // Variable used to store the data received from the Python node
///////////////////////////////////////////////////////////
// Message Symbols to be received from the publisher
#define ServoOnSymbol 1  //room servo symbol
#define ServoOffSymbol 2
#define GasSymbol 3
#define CurtainsOnSymbol 4
#define CurtainsOffSymbol 5

int roomservosignal = 5;
Servo roomservo;
int pos1 = 0;  //position of the room servo
bool gasEmergencyActive = false;  
unsigned long gasEmergencyStartTime; // Timestamp of start of gas emergency to be used later to give gas emergency enough time
const unsigned long gasEmergencyDuration = 10000; // Duration 



void SwitchCaseFunction(const std_msgs::Int8& Received_Message) { // Large switch function to respond to each symbol
  int Order = Received_Message.data;
  switch (Order) {
    case ServoOnSymbol:  
      servo_on();
      break;
    case ServoOffSymbol: 
      servo_off();
      break;
    case GasSymbol: 
      GasEmergency();
      break;
    case CurtainsOnSymbol:
      motoropen();
      break;
    case CurtainsOffSymbol:
      motorclose();
      break;
    default:
      // no appropriate default case I think
      break;
  }
}



void passwordCallback(const std_msgs::Int32& msg)  //door password call back func
{
  entered_pass = msg.data;  //variable for stored in it the data come from py node
}




ros::Subscriber<std_msgs::Int32> passwordSubscriber("password_topic", &passwordCallback);

ros::NodeHandle nh;
std_msgs::Int8 Received_Message;
ros::Subscriber<std_msgs::Int8> Arduino2("Orders Channel", &SwitchCaseFunction);





void setup() {
  nh.initNode();
  nh.subscribe(Arduino2);
  roomservo.attach(roomservosignal) ;
  pinMode(WHITE, OUTPUT);
  pinMode(H1, OUTPUT);
  pinMode(H2, OUTPUT);
  pinMode(EN, OUTPUT);

  EEPROM.write(passwordAddress,storedPassword);
  nh.subscribe(passwordSubscriber);
  doorservo.attach(doorServoPin);
  attachInterrupt(doorServoPin, servoInterrupt, CHANGE); // Use interrupt to open the room door
}


void loop() {
  nh.spinOnce();

  // Check and manage gas emergency state
  if (gasEmergencyActive) {
    unsigned long currentTime = millis();
    if (currentTime - gasEmergencyStartTime >= gasEmergencyDuration) {
      //turn off gas after its time is over
      gasEmergencyActive = false;
      motorstop();
      servo_off();

    }
  }


   //intrrupt of the door 
    
   if (receivedInterrupt) // Flag used to check if interrupt is started
  {
    receivedInterrupt = false; // Reset flag

    if (passwordMatched) // Flag used to check if the entered password is the same or not
    {
      doorservo_on(); // Function used to open the door servo
    }
    else
    {
      nh.loginfo("Invalid password!"); // Send to the ROS node terminal "Invalid password"
    }

    // Reset flags
    passwordMatched = false;
  }



  delay(1000);
}




void servoInterrupt()
{
  // Check if entered password matches stored password
 
  
    if (entered_pass == storedPassword)
    {
      passwordMatched = true; // Set flag to indicate password matched
    }
 

  receivedInterrupt = true; // Set flag to indicate interrupt received
}

void doorservo_on()
{
  for (pos2 = 0; pos2 <= 90; pos2 += 1)  //pos2 is used in door servo
  {
    doorservo.write(pos2);
    delay(2);
  }
  delay(100);
}




// Servo On and Off functions
void servo_on() {
  for (pos1 = 0; pos1 <= 90; pos1 += 1) {
    roomservo.write(pos1);
    delay(5);
  }
  delay(100) ;
}

void servo_off() {
  delay(1000) ;
  for (pos1 = 90; pos1 >= 0; pos1 -= 1) {
    roomservo.write(pos1);
    delay(2);
  }
  delay(100) ;
}






void motorclose() {
  digitalWrite(WHITE, LOW);
  analogWrite(EN, 100);
  digitalWrite(H2, HIGH);
  digitalWrite(H1, LOW);

  // Delay to keep the motor on for a specific duration
  delay(2000); // 2seconds (adjust as needed)
  motorstop() ;
}

void motoropen() {
  digitalWrite(WHITE, HIGH);
  analogWrite(EN, 100);
  digitalWrite(H1, HIGH);
  digitalWrite(H2, LOW);
  // Delay to keep the motor on for a specific duration
  delay(2000); // 2 seconds (adjust as needed)
   motorstop() ;
}

void motorstop() {
  digitalWrite(WHITE, LOW);
  analogWrite(EN, 0);
  digitalWrite(H1, LOW);
  digitalWrite(H2, LOW);
}






void GasEmergency() {
  motoropen();
  servo_on();
  gasEmergencyActive = true;
  gasEmergencyStartTime = millis(); // calling opening functions here and carrying out the process for 10 seconds 
}
