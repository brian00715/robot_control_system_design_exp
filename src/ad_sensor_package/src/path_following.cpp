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

static int turn_flag[4] = {0};
static int turn_sum = 0;

int main(int argc, char **argv)
{
    init(argc, argv, "path_following");
    NodeHandle n;
    Subscriber sub_ad = n.subscribe("/wpb_cv/ad", 100, AD_Callback);
    Publisher vel_pub = n.advertise<Twist>("/cmd_vel", 10);
    Time::init();
    Rate loop_rate(30);
    int nCountToStop = 0;
    while (ok())
    {
        printf("[AD]");
        for (int i = 3; i < 7; i++)
        {
            printf(" ad%d=%d ", i + 1, arADVal[i]);
        }
        printf("\n");
        vel_cmd.linear.x = 0.08;
        vel_cmd.angular.z = 0;

        if (arADVal[3] < 1900)
        {
            // turn_sum++;
            turn_flag[3] = 1;
            vel_cmd.linear.x = 0.05;
            vel_cmd.angular.z = 0.5;
        }
        else
        {
            turn_flag[3] = 0;
        }

        if (arADVal[4] < 1710)
        {
            turn_flag[4] = 1;
            // vel_cmd.linear.x = 0.05;
            vel_cmd.angular.z = 0.5;
            if (turn_flag[5] == 0)
            {

                // vel_cmd.angular.z = 0.35;
            }
            else
            {
                // vel_cmd.angular.z = 0;
            }
        }
        else
        {
            turn_flag[4] = 0;
        }

        if (arADVal[5] < 1750)
        {
            turn_flag[5] = 1;
            // vel_cmd.linear.x = 0.05;
            vel_cmd.angular.z = -0.5;
            // if (turn_flag[4] == 0)
            // vel_cmd.angular.z = -0.35;
            // else
            {
                // vel_cmd.angular.z = 0;
            }
        }
        else
        {
            turn_flag[5] = 0;
        }

        if (arADVal[6] < 2000)
        {

            turn_flag[6] = 1;
            vel_cmd.linear.x = 0.05;
            vel_cmd.angular.z = -0.5;
        }
        else
        {
            turn_flag[6] = 0;
        }

        // diaotou
        // if (turn_flag[3] == 0 && turn_flag[4] == 0 && turn_flag[5] == 0&&turn_flag[6] == 0 )
        // {
        //     vel_cmd.linear.x = 0.0;
        //     vel_cmd.angular.z = 0.4;
        // }

        nCountToStop++;
        if (nCountToStop > 2000)
        {
            vel_cmd.linear.x = 0;
            vel_cmd.angular.z = 0;
        }
        vel_pub.publish(vel_cmd);
        spinOnce();
        loop_rate.sleep();
    }
    vel_cmd.linear.x = 0;
    vel_cmd.angular.z = 0;
    vel_pub.publish(vel_cmd);
    return 0;
}