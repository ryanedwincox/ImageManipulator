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

#define MAX_LOG_SIZE (0x100000)
#define DEBUG_BUFFER_SIZE 50 // cl_int

class filter
{
public:
    filter(lowPassFilter lpf, cl_context context, cl_uint deviceCount, cl_device_id* devices);
    void setImage(cv::Mat img);
    void runProgram();
    cv::Mat getImage();
private:
    //filter() {}
    cv::Mat image;
    size_t imageWidth;
    size_t imageHeight;
    size_t imageSize;
    void* newDataPointer;
    lowPassFilter lpf;

    FILE* programHandle;
    char *programBuffer;
    size_t programSize;
    cl_program program;
    cl_context context;
    cl_uint deviceCount;
    cl_device_id* devices;
    cl_int err;
    cl_kernel kernel;
    cl_mem clImage;
    cl_mem clResult;
    cl_mem clDebug;
    cl_command_queue queue;
};

#endif // FILTER_H
