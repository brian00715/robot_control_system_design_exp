#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <stdio.h>
#include <std_msgs/Float64.h>

double roll, pitch, yaw;
double x, y, z;
std_msgs::Float64 yaw_float64;
void LaserOdomCB(nav_msgs::Odometry odom)
{
    tf::Quaternion RQ2;
    tf::quaternionMsgToTF(odom.pose.pose.orientation, RQ2);
    tf::Matrix3x3(RQ2).getRPY(roll, pitch, yaw);
    x = odom.pose.pose.position.x;
    y = odom.pose.pose.position.y;
    z = odom.pose.pose.position.z;
    printf("-- x:%.2f y:%.2f yaw:%.2f\r\n", x * 100, y * 100, yaw * 57.3);
    yaw_float64.data = yaw * 57.3;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "test_laser_odom");
    ros::NodeHandle nh;
    ros::Publisher yaw_pub = nh.advertise<std_msgs::Float64>("odom_rf2o/yaw", 5);
    ros::Subscriber laser_odom_sub = nh.subscribe("odom_rf2o", 5, LaserOdomCB);
    ros::Rate loop_rate(10);

    while (ros::ok())
    {
        yaw_pub.publish(yaw_float64);
        ros::spinOnce();
        loop_rate.sleep();
    }
}