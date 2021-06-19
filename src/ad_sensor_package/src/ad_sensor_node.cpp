#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
static int arADVal[15];
void AD_Callback(const std_msgs::Int32MultiArray msg)
{
    if (msg.data.size() < 15)
        return;
    for (int i = 0; i < 15; i++)
    {
        arADVal[i] = msg.data[i];
    }
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "ad_sensor");
    ros::NodeHandle n;
    ros::Subscriber sub_ad = n.subscribe("/wpb_cv/ad", 100, AD_Callback);
    ros::Rate loop_rate(1);
    while (ros::ok())
    {
        printf("[AD]");
        for (int i = 0; i < 15; i++)
        {
            printf(" ad%d=%d ", i + 1, arADVal[i]);
        }
        printf("\n");
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}