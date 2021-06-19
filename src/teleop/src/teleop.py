import serial
from geometry_msgs.msg import Twist
import rospy
if __name__ == "__main__":
    COM = "/dev/ttyUSB1"
    BAUD_RATE = 115200
    rospy.init_node("init_node")
    vel_pub = rospy.Publisher("/cmd_vel", Twist, queue_size=10)
    twist = Twist()
    ser = serial.Serial(COM, BAUD_RATE)
    print(ser)
    if not ser.isOpen():
        ser.open()
    while True:
        # rx = str(ser.readline(), "utf-8")
        rx = ser.read(1).decode("utf-8")
        if rx == "i":
            twist.linear.x = 0.3
        elif rx == "k":
            twist.linear.x = 0
            twist.linear.y = 0
            twist.angular.z = 0
        elif rx == ",":
            twist.linear.x = -0.3
        elif rx == "j":
            twist.angular.z = 0.6
        elif rx == "l":
            twist.angular.z = -0.6
        elif rx == "u":
            twist.linear.y = 0.3
        elif rx == "o":
            twist.linear.y = -0.3
        elif rx == 'm':
            twist.angular.z = 0
        print(rx)
        vel_pub.publish(twist)
