#ifndef FILTER_H
#define FILTER_H

#include "lowpassfilter.h"
#include </usr/include/CL/cl.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

class filter
{
public:
    filter(cv::Mat img, lowPassFilter lpf);
    void setImage(cv::Mat img);
    void runProgram();
    cv::Mat getImage();
private:
    //filter() {}
    cv::Mat image;
    lowPassFilter lpf;
};

#endif // FILTER_H
