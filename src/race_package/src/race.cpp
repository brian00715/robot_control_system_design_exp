
/**
 * @file race.cpp
 * @author smkk
 * @brief 比赛核心源文件
 * @version 1.2
 * @date 2021-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../include/race.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <tf/transform_broadcaster.h>
#include "../include/my_utils.hpp"

using namespace cv;
using namespace std;

// =================================================
//                   主函数
// =================================================
int main(int argc, char **argv)
{
    // using namespace PID_s;
    vel_cmd.linear.x = 0;
    vel_cmd.linear.y = 0;
    vel_cmd.linear.z = 0;
    vel_cmd.angular.x = 0;
    vel_cmd.angular.y = 0;
    vel_cmd.angular.z = 0;
    using namespace std;
    ros::init(argc, argv, "race");

    ros::NodeHandle n;
    // ros::Subscriber sub_ad = n.subscribe("/wpb_cv/ad", 4, AD_Callback);
    ros::Subscriber rgb_sub = n.subscribe("/camera/rgb/image_rect_color", 2, Cam_RGB_Callback);
    // ros::Subscriber sub_imu = n.subscribe("imu/data_raw", 2, IMU_Callback);

    ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // 速度初始化
    vel_cmd.linear.x = 0;
    vel_cmd.linear.y = 0;
    vel_cmd.linear.x = 0;
    vel_cmd.linear.y = 0;
    vel_cmd.linear.z = 0;
    vel_cmd.angular.x = 0;
    vel_cmd.angular.y = 0;
    vel_cmd.angular.z = 0;
    vel_cmd.linear.z = 0;
    vel_cmd.angular.x = 0;
    vel_cmd.angular.y = 0;
    vel_cmd.angular.z = 0;
    vel_pub.publish(vel_cmd);

    ros::Rate loop_rate(60);
    while (ros::ok())
    {
        //向底盘发送速度值
        vel_pub.publish(vel_cmd);

        //延时
        if (!endRaceFlag)
        {
            ros::spinOnce(); // 在此处监听话题，调用回调函数
        }
        else
        {
            vel_cmd.linear.x = 0;
            vel_cmd.linear.y = 0;
            vel_cmd.angular.z = 0;
        }
        //生成图像显示和参数调节的窗口
        namedWindow("Control", CV_WINDOW_AUTOSIZE);
        cvCreateTrackbar("LowH", "Control", &Black_H_low, 179); //Hue (0 - 179)
        cvCreateTrackbar("HighH", "Control", &Black_H_high, 179);
        cvCreateTrackbar("LowS", "Control", &Black_S_low, 255); //Saturation (0 - 255)
        cvCreateTrackbar("HighS", "Control", &Black_S_high, 255);
        cvCreateTrackbar("LowV", "Control", &Black_V_low, 255); //Value (0 - 255)
        cvCreateTrackbar("HighV", "Control", &Black_V_high, 255);
        loop_rate.sleep();
    }
    vel_cmd.linear.x = 0;
    vel_cmd.angular.z = 0;
    vel_pub.publish(vel_cmd);
    return 0;
}

/**
 * @brief IMU话题回调函数
 * 
 * @param msg 
 */
void IMU_Callback(const sensor_msgs::Imu::ConstPtr &msg)
{

    static tf::TransformBroadcaster odom_broadcaster;
    tf::Transform transform;
    transform.setOrigin(tf::Vector3(0, 0, 0));
    tf::Quaternion q(msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);
    yaw = tf::getYaw(q) * 180 / 3.1415;
    ROS_INFO("[wpb_cv_imu_demo] Yaw= %.3f ", yaw);
    // transform.setRotation(q);
    // odom_broadcaster.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "imu_base", "base_footprint"));
}

/**
 * @brief 对比两个轮廓的面积大小
 * 
 * @param contour1 
 * @param contour2 
 * @return true 
 * @return false 
 */
bool Contour_Area(vector<Point> contour1, vector<Point> contour2)
{
    return contourArea(contour1) > contourArea(contour2);
}

/**
 * @brief 摄像头回调函数
 * 
 * @param msg 
 */
void Cam_RGB_Callback(const sensor_msgs::ImageConstPtr &msg)
{
    if (trackingBall && !endRaceFlag)
    {
        end_count++;
        if (end_count > 225)
        {
            endRaceFlag = 1;
            printf("end race!\r\n");
            vel_cmd.linear.x = 0;
            vel_cmd.linear.y = 0;
            vel_cmd.angular.z = 0;
            return;
        }
    }
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception &e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    Mat imgOriginal = cv_ptr->image;

    //将RGB图片转换成HSV
    Mat imgHSV, imgGray;
    vector<Mat> hsvSplit;
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
    cvtColor(imgOriginal, imgGray, COLOR_BGR2GRAY);

    Mat imgFiltered;
    medianBlur(imgOriginal, imgFiltered, 3); // 中值滤波，去除椒盐噪声

    IplImage imgPathROI_Ipl(imgFiltered);                     // 设置赛道图像ROI，避免干扰
    cvSetImageROI(&imgPathROI_Ipl, cvRect(70, 80, 180, 160)); // 180x160
    Mat imgPathROI = cvarrToMat(&imgPathROI_Ipl);

    //>>>在HSV空间做直方图均衡化<<<
    split(imgHSV, hsvSplit);
    equalizeHist(hsvSplit[2], hsvSplit[2]);
    merge(hsvSplit, imgHSV);

    // >>>二值化<<<
    Mat imgThresByHSV, imgThresByBlack;
    //使用上面的Hue,Saturation和Value的阈值范围对图像进行二值化
    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresByHSV);
    inRange(imgPathROI, Scalar(Black_H_low, Black_S_low, Black_V_low), Scalar(Black_H_high, Black_S_high, Black_V_high), imgThresByBlack);

    //>>>开操作 (去除一些噪点)<<<
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(imgThresByHSV, imgThresByHSV, MORPH_OPEN, element);
    morphologyEx(imgThresByBlack, imgThresByBlack, MORPH_OPEN, element);
    // erode(imgThresByBlack,imgThresByBlack,cv::Mat()); // 腐蚀降噪
    //>>>闭操作 (连接一些连通域)<<<
    morphologyEx(imgThresByHSV, imgThresByHSV, MORPH_CLOSE, element);
    morphologyEx(imgThresByBlack, imgThresByBlack, MORPH_CLOSE, element);
    // bilateralFilter(imgThresByBlack,imgThresByBlackFiltered,filterDValue,filterSigmaColorValue,filterSigmaSpaceValue);

    // >>>网球轮廓检测<<<
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat imgThresCopy = imgThresByHSV.clone();
    findContours(imgThresCopy, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    sort(contours.begin(), contours.end(), Contour_Area); // 对轮廓按面积排序
    Point2f gate;                                         // 两个球的中心点
    if (contours.size() > 0)
    {
        float ctrl_x = 0, ctrl_y = 0, ctrl_yaw = 0;
        RotatedRect rect = minAreaRect(contours[0]);
        Point2f midPoint1 = rect.center;
        Point2f midPoint2(midPoint1);
        if (contours.size() > 1) // 找到两个球的轮廓
        {
            rect = minAreaRect(contours[1]);
            midPoint2 = rect.center;
            float slope = (midPoint2.y - midPoint1.y) / (midPoint2.x - midPoint1.x);
            printf("-- slope:%5.2f\r\n", slope);
            ctrl_yaw = CatchBallPID_yaw.getOutput(slope, 0);

            line(imgOriginal, midPoint1, midPoint2, Scalar(255, 255, 255), 2);
            gate.x = (midPoint2.x + midPoint1.x) / 2;
            gate.y = (midPoint2.y + midPoint1.y) / 2;
            // 图像大小640x480(30hz) 320x240(60hz)
            float delta_x = gate.x - 158;
            float delta_y = 238 - gate.y;
            // gate.y  = -1; // !!!!!!!
            // printf("gate_y:%.2f\r\n",gate.y);
            if (gate.y > viewThres_y) // 开始追球
            {
                printf("-- Found two balls...\r\n");
                trackingBall = 1;
                beginTrackBall = 1; // 一旦发现球便切换控制模式
                ctrl_y = CatchBallPID_x.getOutput(delta_x, 0);
                ctrl_x = -CatchBallPID_y.getOutput(delta_y, 0);
                if (ctrl_x < 0.3)
                {
                    ctrl_x = 0.3;
                }
                if (fabs(ctrl_y) < ctrlThres_y) // 死区控制
                {
                    ctrl_y = 0;
                }
                if (gate.y > normalCorrThres)
                {
                    ctrl_y = 0;
                }
                if (fabs(ctrl_yaw) < ctrlThres_yaw)
                {
                    ctrl_yaw = 0;
                }
                vel_cmd.linear.y = ctrl_y;
                vel_cmd.linear.x = ctrl_x;
                vel_cmd.angular.z = ctrl_yaw;
                // printf("机器人运动速度( linear|x=%.2f y=%.2f, angular= %.2f )\n", ctrl_x, ctrl_y, vel_cmd.angular.z);

            }
            else
            {
                trackingBall = 0;
            }
            rect = minAreaRect(contours[0]);
            drawRect(imgOriginal,rect,Scalar(0, 0, 255), 2);
            rect = minAreaRect(contours[1]);
            drawRect(imgOriginal,rect,Scalar(0, 255, 0), 2);
        }
        else
        {
            ctrl_yaw = 0;
        }
    }

    // >>>赛道轮廓检测<<<
    vector< vector<Point> > pathContours;
    imgThresCopy = imgThresByBlack.clone();
    findContours(imgThresCopy, pathContours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    // 找最大轮廓
    int maxContourIndex = -1;
    float maxContourArea = -1;
    for (int i = 0; i < pathContours.size(); i++)
    {
        float area = contourArea(pathContours[i]);
        if (area > maxContourArea)
        {
            maxContourArea = area;
            maxContourIndex = i;
        }
    }
    if (maxContourIndex >= 0) // 找到赛道轮廓
    {
        drawContours(imgOriginal, pathContours, maxContourIndex, Scalar(255, 255, 0), 2, 8, hierarchy);
        Point2f rectPoints[4];
        RotatedRect rect = minAreaRect(pathContours[maxContourIndex]);
        rect.points(rectPoints);
        drawRect(imgOriginal, rect, Scalar(0, 255, 0), 2);
        if (getEuclideanDistance(rectPoints[0], rectPoints[3]) > getEuclideanDistance(rectPoints[1], rectPoints[0])) // 如果0,3是长边
        {
            traceAngle = 90 + rect.angle;
        }
        else
        {
            traceAngle = rect.angle;
        }
        // printf("traceAngle：%.2f\r\n",traceAngle);
        traceNormalErr = (rectPoints[1].x + rectPoints[0].x) / 2 - 90; // 原图大小180x200
        if (!beginTrackBall)                                           // 视觉巡线
        {
            float ctrl_turn = 0, ctrl_y = 0;
            ctrl_turn = PathFollowPID_turn.getOutput(traceAngle, 0);
            ctrl_y = PathFollowPID_normal.getOutput(traceNormalErr, 0);
            if (fabs(ctrl_turn) < pathCtrlThres_turn)
            {
                ctrl_turn = 0;
            }
            if (fabs(ctrl_y) < pathCtrlThres_normal)
            {
                ctrl_y = 0;
            }
            vel_cmd.angular.z = ctrl_turn;
            vel_cmd.linear.x = 0.58;
            vel_cmd.linear.y = ctrl_y;
            printf("ctrl_turn:%.2f ctrl_y:%.2f\r\n", ctrl_turn, ctrl_y);
        }
    }
    else
    {
        if(!beginTrackBall)
        {
            vel_cmd.angular.z = 0;
            vel_cmd.angular.x = 0.2;
            vel_cmd.linear.y = 0;
        }

    }

    //显示处理结果
    imshow("原图", imgOriginal);
    imshow("赛道检测", imgThresByBlack);
    // imshow("HSV", imgHSV);
    imshow("网球检测", imgThresByHSV);
    waitKey(1);
}

float last_omega = 0;
float last_vel_y = 0;
/**
 * @brief AD话题回调函数
 * 
 * @param msg 
 */
void AD_Callback(const std_msgs::Int32MultiArray msg)
{
    if (msg.data.size() < 15)
        return;

    //获取AD值
    for (int i = 0; i < 4; i++)
    {
        arADVal[i] = msg.data[i];
        arADVal[i] = 1 - ((arADVal[i] - ad_min[i]) / (ad_max[i] - ad_min[i])); // nomorlize
    }

    if (endADCount > 100) //驶出黑线
    {
        endADFlag = 1;
    }
    //显示AD值
    // for (int i = 0; i < 4; i++)
    // {
    //     printf(" ad%d=%.2f ", i + 1, arADVal[i]);
    // }
    // printf("\n");
    ad_err = (arADVal[1] - arADVal[2]);
    // printf("ad1:%5.2f ad2:%5.2f ad_err=%5.2f\r\n",arADVal[1],arADVal[2],ad_err);
    if (fabs(ad_err) < 0.4 || fabs(ad_err) > 1.5)
    {
        ad_err = 0;
    }
    if (!beginTrackBall)
    {
        if (arADVal[0] < 0.15 && arADVal[1] < 0.15 && arADVal[2] < 0.15 && arADVal[3] < 0.15) // 4个传感器都检测不到黑线
        {
            endADCount++;
            adTrack = 0;
            vel_cmd.angular.z = last_omega;
            vel_cmd.linear.y = last_vel_y;
        }
        else
        {
            adTrack = 1;
            endADFlag = 0;
            endADCount = 0;
        }
        vel_cmd.linear.y = ad_err * 0.03;               // 横向修正
        vel_cmd.angular.z = ad_err * 0.25;              // 稍微带点自转
        vel_cmd.linear.x = 0.28 - fabs(ad_err * 0.048); // 有角速度时降低线速度，避免冲出去
        // 两侧检测到黑线时进行强力校正
        if (arADVal[0] > 0.3)
        {
            vel_cmd.angular.z = 0.3;
            vel_cmd.linear.y = 0.036;
            vel_cmd.linear.x = 0.08;
            last_omega = vel_cmd.angular.z;
            last_vel_y = vel_cmd.linear.y;
        }
        if (arADVal[3] > 0.3)
        {
            vel_cmd.angular.z = -0.3;
            vel_cmd.linear.y = -0.036;
            vel_cmd.linear.x = 0.08;
            last_omega = vel_cmd.angular.z;
            last_vel_y = vel_cmd.linear.y;
        }
    }

    // vel_cmd.linear.x = 0;
    // printf("ad_err:%.2f omega:%.2f\r\n", ad_err, vel_cmd.angular.z);
}