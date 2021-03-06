#ifndef FILTER_H
#define FILTER_H

//#include </usr/include/CL/cl.hpp>
#include <CL/cl.h>
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
    filter();
    void setImage(cv::Mat img);
    void buildProgram(const char* clPath, cl_int maskSize);
    void runProgram();
    void* readOutput();
    void* readDebugOutput();
    cv::Mat getInputImage();
private:
    cv::Mat image;
    size_t imageWidth;
    size_t imageHeight;
    size_t imageSize;
    const char* clPath;
    cl_int maskSize;

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
