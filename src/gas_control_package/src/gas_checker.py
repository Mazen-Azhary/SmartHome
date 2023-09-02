#!/usr/bin/env python3
import rospy
from std_msgs.msg import Int32
from std_msgs.msg import Bool
from time import sleep

# Define pins, you can update these with actual pin numbers
GREEN = 2
ORANGE = 3
RED = 4
BUZZER = 5

# Define sensor threshold values
DANGEROUS_THRESHOLD = 40.0
WARNING_THRESHOLD = 25.0

def setup():
    # Initialize ROS node
    rospy.init_node("gas_sensor_node")
    rospy.loginfo("Gas checker is running")

    # Set up GPIO pins
    for pin in [GREEN, ORANGE, RED, BUZZER]:
        rospy.set_param(f"/gpio{pin}", "output")

def loop():
    pub = rospy.Publisher("gas_emergency", Bool, queue_size=10)

    while not rospy.is_shutdown():
        for pin in [GREEN, ORANGE, RED, BUZZER]:
            rospy.set_param(f"/gpio{pin}", "low")

        try:
            #these 2 lines are intended to wait for a message from sensor readings topic and then extract the number and put it into sensor value
            sensor_value_msg = rospy.wait_for_message("gas_sensor_reading", Int32, timeout=15.0)
            sensor_value = sensor_value_msg.data
            rospy.loginfo(f"Sensor Value: {sensor_value}")  # for testing

            if sensor_value >= DANGEROUS_THRESHOLD:
                rospy.set_param(f"/gpio{RED}", "high")
                rospy.set_param(f"/gpio{BUZZER}", "high")
                rospy.logwarn("DANGEROUS GAS LEVEL")
                pub.publish(True)
            elif sensor_value >= WARNING_THRESHOLD:
                rospy.set_param(f"/gpio{ORANGE}", "high")
                rospy.logwarn("Potential gas leak, be careful")
            else:
                rospy.set_param(f"/gpio{GREEN}", "high")
                pub.publish(False)
        except rospy.ROSException:
            rospy.logwarn("No gas_sensor_reading data received in the last 5 seconds.")

        sleep(1)  # Adjust the sleep interval as needed

if __name__ == "__main__":
    try:
        setup()
        loop()
    except rospy.ROSInterruptException:
        pass
