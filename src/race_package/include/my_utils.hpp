#ifndef MY_UTILS_HPP_
#define MY_UTILS_HPP_

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
float getEuclideanDistance(cv::Point2f p1,cv::Point2f p2);
void drawRect(cv::Mat& img, cv::RotatedRect& rect,cv::Scalar color,int thick);

#endif