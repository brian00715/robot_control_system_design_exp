#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

static int state = 0;
int main(int argc, char **argv)
{
    using namespace ros;
    using namespace geometry_msgs;
    using namespace std;
    init(argc, argv, "mecnum_control");
    NodeHandle n;
    Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    Rate loop_rate(0.5);
    Twist vel_cmd;
    cout << "start!" << endl;
    while (ok())
    {
        switch (state)
        {
        case 0:
            vel_cmd.linear.x = 0;
            vel_cmd.linear.y = 0.2;
            vel_cmd.linear.z = 0;
            vel_cmd.angular.x = 0;
            vel_cmd.angular.y = 0;
            vel_cmd.angular.z = 0;
            vel_pub.publish(vel_cmd);
            state++;
            break;
        case 1:
            vel_cmd.linear.x = 0;
            vel_cmd.linear.y = -0.2;
            vel_cmd.linear.z = 0;
            vel_cmd.angular.x = 0;
            vel_cmd.angular.y = 0;
            vel_cmd.angular.z = 0;
            vel_pub.publish(vel_cmd);
            state++;
            break;
        case 2:
            vel_cmd.linear.x = -0.1;
            vel_cmd.linear.y = 0;
            vel_cmd.linear.z = 0;
            vel_cmd.angular.x = 0;
            vel_cmd.angular.y = 0;
            vel_cmd.angular.z = 0;
            vel_pub.publish(vel_cmd);
            state++;
            break;
        case 3:
            vel_cmd.linear.x = 0;
            vel_cmd.linear.y = 0.1;
            vel_cmd.linear.z = 0;
            vel_cmd.angular.x = 0;
            vel_cmd.angular.y = 0.1;
            vel_cmd.angular.z = 0;
            vel_pub.publish(vel_cmd);
            state++;
            break;
        case 4:
            vel_cmd.linear.x = 0;
            vel_cmd.linear.y = -0.1;
            vel_cmd.linear.z = 0;
            vel_cmd.angular.x = 0;
            vel_cmd.angular.y = 0;
            vel_cmd.angular.z = 0;
            vel_pub.publish(vel_cmd);
            state++;
            break;
        case 5:
            vel_cmd.linear.x = 0;
            vel_cmd.linear.y = 0;
            vel_cmd.linear.z = 0;
            vel_cmd.angular.x = 0;
            vel_cmd.angular.y = 0;
            vel_cmd.angular.z = 0.2;
            vel_pub.publish(vel_cmd);
            state++;
            break;
        case 5:
            vel_cmd.linear.x = 0;
            vel_cmd.linear.y = 0;
            vel_cmd.linear.z = 0;
            vel_cmd.angular.x = 0;
            vel_cmd.angular.y = 0;
            vel_cmd.angular.z = 0;
            vel_pub.publish(vel_cmd);
            state++;
            break;
        }
        spinOnce();
        loop_rate.sleep();
    }
    return 0;
}