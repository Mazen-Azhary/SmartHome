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