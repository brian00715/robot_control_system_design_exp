#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
int main(int argc, char **argv)
{
    ros::init(argc, argv, "velocity_control");
    ros::NodeHandle n;
    ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    ros::Rate loop_rate(30); // 30hz
    int nCountToStop = 0;
    while (ros::ok())
    {
        geometry_msgs::Twist vel_cmd; // 创建twist类对象
        vel_cmd.linear.x = 0;
        vel_cmd.linear.y = 0;
        vel_cmd.linear.z = 0;
        vel_cmd.angular.x = 0;
        vel_cmd.angular.y = 0;
        vel_cmd.angular.z = 0.4;
        nCountToStop++;
        if (nCountToStop > 200) // 控制运行时间
        {
            break;
            vel_cmd.linear.x = 0;
            vel_cmd.angular.z = 0;
            ROS_WARN("Stop!");
        }
        vel_pub.publish(vel_cmd);
        // ros::spinOnce();
        loop_rate.sleep();
    }
    geometry_msgs::Twist vel_cmd; // 创建twist类对象
    vel_cmd.linear.x = 0;
    vel_cmd.linear.y = 0;
    vel_cmd.linear.z = 0;
    vel_cmd.angular.x = 0;
    vel_cmd.angular.y = 0;
    vel_cmd.angular.z = 0;
    vel_pub.publish(vel_cmd);
    return 0;
}