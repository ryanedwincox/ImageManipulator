#include "filter.h"
#include </usr/include/CL/cl.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

int main()
{
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

    // Will store filter results
    unsigned char* newDataPointer;

    const char* copyImageClPath = "/home/bluhaptics1/Documents/ImageManipulator/cl/copy_image.cl";
    const char* lowPassClPath = "/home/bluhaptics1/Documents/ImageManipulator/cl/low_pass.cl";
    cl_int lpfMaskSize = 5;

    // Create filter object
    filter f1;

    // Copy filter
    f1.buildProgram(copyImageClPath, 0);
    f1.setImage(image);
    f1.runProgram();
    newDataPointer = (unsigned char*) f1.readOutput();
    cv::Mat copiedImage = cv::Mat(cv::Size(imageWidth,imageHeight), CV_8UC3, newDataPointer);

    // lpf filter
    f1.buildProgram(lowPassClPath, lpfMaskSize);
    f1.runProgram();
    newDataPointer = (unsigned char*) f1.readOutput();
    cv::Mat newImage = cv::Mat(cv::Size(imageWidth,imageHeight), CV_8UC3, newDataPointer);

    // Display images
    std::cout << "Display images" << std::endl;

    cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE); // Create a window for display.
    cv::imshow("Original Image", f1.getInputImage());           // Show our image inside it.

    cv::namedWindow("Blured Image", cv::WINDOW_AUTOSIZE); // Create a window for display.
    cv::imshow("Blured Image", newImage);                 // Show our image inside it.

    cv::waitKey(0);
}


