#ifndef FILTER_H
#define FILTER_H

#include </usr/include/CL/cl.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

class filter
{
public:
    filter(cv::Mat img);
    void setImage(cv::Mat img);
    cv::Mat getImage();
private:
    filter() {}
    cv::Mat image;
};

#endif // FILTER_H
