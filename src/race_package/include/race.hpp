#ifndef RACE_HPP_
#define RACE_HPP_
#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <geometry_msgs/Twist.h>
#include "../include/pid_s.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "sensor_msgs/Imu.h"
#include <tf/transform_broadcaster.h>
#include <math.h>

// =================================================
//                   函数声明
// =================================================

void IMU_Callback(const sensor_msgs::Imu::ConstPtr &msg);
void Cam_RGB_Callback(const sensor_msgs::ImageConstPtr &msg);
void AD_Callback(const std_msgs::Int32MultiArray msg);

// =================================================
//                   全局变量
// =================================================
geometry_msgs::Twist vel_cmd; //速度消息包
ros::Publisher vel_pub;       //速度发送
int nCountToStop = 0;         //计时然后停止
int endRaceFlag = 0;          // 比赛完成标识位

/* 视觉追踪参数*********************/
static int iLowH = 27;
static int iHighH = 80;
static int iLowS = 50;
static int iHighS = 255;
static int iLowV = 100;
static int iHighV = 255;
int Black_H_low = 0;
int Black_H_high = 179;
int Black_S_low = 0;
int Black_S_high = 255;
int Black_V_low = 0;
int Black_V_high = 92;
float yaw;                                         // 由imu得到的偏航角
PID_s CatchBallPID_x(0.004, 0.0, 0.0, 10, 5, 0.3); // 对应移动y方向
PID_s CatchBallPID_y(0.05, 0.0, 0.0, 10, 5, 0.55); // 对应移动x方向
PID_s CatchBallPID_yaw(1.8, 0, 0, 0, 0, 0.4);
PID_s PathFollowPID_normal(0.0043, 0.00, 0.01, 1, 5, 0.35); // 横向修正
PID_s PathFollowPID_turn(0.05, 0.00, 0.02, 1, 5, 0.45);     // 转向修正
float ctrlThres_y = 0.05;                                   // 追球横向死区
float ctrlThres_x = 0.02;                                   // 追球纵向死区
float ctrlThres_yaw = 0.05;                                 // 追球偏航角死区
float pathCtrlThres_turn = 0.05;
float pathCtrlThres_normal = 0.03; // 巡迹横向死区
int findBallOK = 0;
int beginTrackBall = 0;    // 视野中发现球
int viewThres_y = 5;       // 球在视野中的纵向阈值
float end_count = 0;       // 球的视野丢失后运行延时
int normalCorrThres = 195; // 追球横向修正图像坐标阈值
int trackingBall = 0;      // 正在追球
int traceThres = 35;       // 赛道分割阈值
float traceAngle = 0;      // 赛道包络矩形的夹角
int traceNormalErr = 0;    // 赛道与视野中心的横向偏差
int filterDValue = 3;
int filterSigmaColorValue = 100;
int filterSigmaSpaceValue = 100;

/* 灰度寻迹参数*********************/
float omega_add = 0;
static int endADFlag = 0;
int endADCount = 0;
int adTrack = 0;
static float arADVal[15];
float ad_a = 0, ad_b = 1.0; //差比和系数
float ad_p = 1;             // 比例系数
float ad_err = 0;
const float ad_max[4] = {3785, 4092, 3830, 4080};
const float ad_min[4] = {1885, 2710, 2180, 2350};

#endif