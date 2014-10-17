#include "filter.h"

// Constructor
filter::filter(cv::Mat img)
{
    setImage(img);
}

void filter::setImage(cv::Mat img)
{
    image = img;
}

cv::Mat filter::getImage()
{
    return image;
}
