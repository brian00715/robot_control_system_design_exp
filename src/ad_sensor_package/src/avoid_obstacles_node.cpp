#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <geometry_msgs/Twist.h>
using namespace geometry_msgs;
using namespace ros;
using namespace std;
using namespace std_msgs;
Twist vel_cmd;
static int arADVal[15];
void AD_Callback(const Int32MultiArray msg)
{
    if (msg.data.size() < 15)
    {
        return;
    }
    for (int i = 0; i < 15; i++)
    {
        arADVal[i] = msg.data[i];
    }
}
int main(int argc, char **argv)
{
    init(argc, argv, "avoid_obstacles");
    NodeHandle n;
    Subscriber sub_ad = n.subscribe("/wpb_cv/ad", 100, AD_Callback);
    Publisher vel_pub = n.advertise<Twist>("/cmd_vel", 10);
    Time::init();
    Rate loop_rate(2);
    int nCountToStop = 0;
    while (ok())
    {
        printf("[AD]");
        for (int i = 0; i < 2; i++)
        {
            printf(" ad%d=%d ", i + 1, arADVal[i]);
        }
        printf("\n");
        vel_cmd.linear.x = 0.1;
        vel_cmd.angular.z = 0;
        if (arADVal[0] > 700)
        {
            vel_cmd.linear.x = 0;
            vel_cmd.angular.z = -0.3;
        }
        if (arADVal[1] > 700)
        {
            vel_cmd.linear.x = 0;
            vel_cmd.angular.z = 0.3;
        }
        nCountToStop++;
        // if (nCountToStop > 20)
        // {
        //     vel_cmd.linear.x = 0;
        //     vel_cmd.angular.z = 0;
        // }
        vel_pub.publish(vel_cmd);
        spinOnce();
        loop_rate.sleep();
    }
    return 0;
}