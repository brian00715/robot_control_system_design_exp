import rospy
from geometry_msgs.msg import Twist
if __name__ == "__main__":
    pub = rospy.Publisher('cmd_vel', Twist, queue_size=2)
    rospy.init_node('stop')
    twist = Twist()
    twist.linear.x = 0
    twist.angular.z = 0
    pub.publish(twist)
