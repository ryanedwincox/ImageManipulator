#include "lowpassfilter.h"

lowPassFilter::lowPassFilter()
{
    clPath = "/home/bluhaptics1/Documents/ImageManipulator/cl/gaussian_blur.cl";
    int maskSize = 3;
    maskSizePointer = &maskSize;
}

const char* lowPassFilter::getClPath()
{
    return clPath;
}

int getMaskSize()
{
    return maskSizePointer;
}
