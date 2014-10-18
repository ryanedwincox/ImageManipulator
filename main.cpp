#include "filter.h"
#include </usr/include/CL/cl.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

cl_context initializeOpenCL();

int main()
{
    // get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    // get first platform
    cl_platform_id platform;
    cl_int err = clGetPlatformIDs(1, &platform, NULL);

    // get device count
    cl_uint deviceCount;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &deviceCount);

    // get device count
    cl_device_id* devices;
    devices = new cl_device_id[deviceCount];
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, deviceCount, devices, NULL);

    // create a single context for all devices
    cl_context context = clCreateContext(NULL, deviceCount, devices, NULL, NULL, &err);
    std::cout << "context error: " << err << "\n";

    // Load image
    cv::Mat image;
    image = cv::imread("/home/bluhaptics1/Documents/ImageManipulator/images/Lenna.png", CV_LOAD_IMAGE_COLOR);

    // Check for invalid input
    if(! image.data )
    {
        std::cout <<  "Could not open or find the image" << std::endl;
        return -1;
    }

    size_t imageWidth = image.cols;
    size_t imageHeight = image.rows;
    size_t imageSize = imageHeight * imageWidth;

    // Will store filter results
    unsigned char newData [imageSize * 4];

    // Create each type of filter object
    lowPassFilter lpf;  // = new lowPassFilter();  ????

    // Create new filter
    filter f1(lpf, context, deviceCount, devices);  // = new filter(image, lpf);   ?????

    // Run filter

    // Display images
    cv::Mat newImage = cv::Mat(cv::Size(imageWidth,imageHeight), CV_8UC4, newData);

    cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE); // Create a window for display.
    cv::imshow("Original Image", f1.getImage());           // Show our image inside it.

    cv::namedWindow("Blured Image", cv::WINDOW_AUTOSIZE); // Create a window for display.
    cv::imshow("Blured Image", newImage);                 // Show our image inside it.

    cv::waitKey(0);
}



