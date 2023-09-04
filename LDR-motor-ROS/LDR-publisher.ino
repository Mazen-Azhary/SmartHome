

#include <ros.h>
#include <std_msgs/Int32.h>
#define LDR A0

int lightIntensity = 0;
ros::NodeHandle  nh;

std_msgs::Int32 msg;
ros::Publisher chatter("chatter", &str_msg);


void setup()
{
  nh.initNode();
  nh.advertise(chatter);
  pinMode(LDR, INPUT);
}

void loop()
{
  lightIntensity = analogRead(LDR);
  msg.data = lightIntensity;
  ROS_INFO("%d", msg.data);
  chatter.publish(msg);
  nh.spinOnce();
  delay(1000);
}
