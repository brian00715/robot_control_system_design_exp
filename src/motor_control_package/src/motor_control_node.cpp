#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>

int main(int argc,char** argv)
{
    using namespace ros;
    using namespace std;
    ros::init(argc,argv,"motor_ctrl");
    ros::NodeHandle n;
    ros::Rate loop_rate(1);
    Publisher motor_pub = n.advertise<std_msgs::Int32MultiArray>("/motor_ctrl",10);
    std_msgs::Int32MultiArray motor_val;
    double val = 0;
    while(ros::ok())
    {
        motor_val.data.clear();
        motor_val.data.push_back(100);
        motor_val.data.push_back(200);
        motor_val.data.push_back(300);
        motor_val.data.push_back(400);
        motor_pub.publish(motor_val);
        cout<<"电机速度"<<endl;
        loop_rate.sleep();
        spinOnce();
    }
    return 0;
}