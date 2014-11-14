#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

class lowPassFilter
{
public:
    const char* getClPath();
    int getMaskSize();
private:
    const char* clPath;
    const void* maskSizePointer;
};

#endif // LOWPASSFILTER_H
