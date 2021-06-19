#include "../include/my_utils.hpp"
#include <cmath>
using namespace cv;
/**
 * @brief 计算两点之间的欧式距离
 * 
 * @param p1 
 * @param p2 
 * @return float 
 */
float getEuclideanDistance(Point2f p1, Point2f p2)
{
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

/**
 * @brief 在图像上画矩形
 * 
 * @param img 
 * @param rect 
 * @return float 
 */
void drawRect(Mat& img, RotatedRect& rect,Scalar color,int thick)
{
    Point2f p[4];
    rect.points(p);
    for (int j = 0; j <= 3; j++)
    {
        line(img, p[j], p[(j + 1) % 4], color, thick);
    }
}