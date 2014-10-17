#include "filter.h"
#include </usr/include/CL/cl.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

//using namespace std;

int main()
{
    // Load image
    cv::Mat image;
    image = cv::imread("/home/bluhaptics1/Documents/ImageManipulator/images/Lenna.png", CV_LOAD_IMAGE_COLOR);   // Read the file

    filter * f1 = new filter(image);

    cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE);// Create a window for display.
    cv::imshow("Original Image", f1->getImage());                   // Show our image inside it.

    cv::waitKey(0);
}

