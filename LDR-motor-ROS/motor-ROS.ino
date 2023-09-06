#include <ros.h>
#include <std_msgs/Int32.h>

#define H1 7
#define H2 8
#define EN 6
int light = 0;
int flag = 0;

ros::NodeHandle nh;

std_msgs::Int32 light_msg;

void lightCallback(const std_msgs::Int32& msg) 
{
  light = msg.data;
  Serial.println(light);

  if (light > 350 && flag == 0)
  {
    motoropen();
    delay(1000);
    motorstop();
    flag = 1;
  }

  if (light < 350 && flag == 1)
  {
    motorclose();
    delay(1000);
    motorstop();
    flag = 0;
  }
}

ros::Subscriber<std_msgs::Int32> light_sub("/ldr_data", &lightCallback);



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

void setup()
{
  Serial.begin(9600);
  pinMode(H1, OUTPUT);
  pinMode(H2, OUTPUT);
  pinMode(EN, OUTPUT);

  nh.initNode();
  nh.subscribe(light_sub); 
}



void loop()
{
  nh.spinOnce();
  delay(1000);
}