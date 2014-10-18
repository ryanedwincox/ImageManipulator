#include "filter.h"

// Constructor
filter::filter(cv::Mat img, lowPassFilter lpf)
{
    setImage(img);
    this->lpf = lpf;
}

void filter::setImage(cv::Mat img)
{
    image = img;
}

void filter::runProgram()
{
    // initialize
    FILE* programHandle;
    char *programBuffer;
    size_t programSize;
    cl_program program;

    // get size of kernel source
    programHandle = fopen(lpf.clPath, "r");
    fseek(programHandle, 0, SEEK_END);
    programSize = ftell(programHandle);
    rewind(programHandle);

    // read kernel source into buffer
    programBuffer = (char*) malloc(programSize + 1);
    programBuffer[programSize] = '\0';
    fread(programBuffer, sizeof(char), programSize, programHandle);
    fclose(programHandle);

    // create program from buffer
    program = clCreateProgramWithSource(lpf.context, 1,
            (const char**) &programBuffer, &programSize, NULL);

    // build program
    const char* buildOptions = "";
    cl_int warn = clBuildProgram(program, deviceCount, devices, buildOptions, NULL, NULL);
    std::cout << "program error: " << warn << "\n";

    // create the log string and show it to the user. Then quit
    char buildLog[MAX_LOG_SIZE];
    err = clGetProgramBuildInfo(program,
                          devices[0],
                          CL_PROGRAM_BUILD_LOG,
                          MAX_LOG_SIZE,
                          &buildLog,
                          NULL);
    printf("**BUILD LOG**\n%s",buildLog);
    std::cout << "clGetProgramBuildInfo error: " << err << "\n";

    //create queue to which we will push commands for the device.
    cl_command_queue queue;
    queue = clCreateCommandQueue(context,devices[0],0,&err);
    std::cout << "command queue error: " << err << "\n";

    // Create an OpenCL buffer for the image
    cl_mem clImage = clCreateBuffer(context,
                                    CL_MEM_READ_ONLY,
                                    imageSize * 3,
                                    NULL,
                                    &err);
    std::cout << "clImage error: " << err << "\n";

    // Create an OpenCL buffer for the result
    cl_mem clResult = clCreateBuffer(context,
                                     CL_MEM_WRITE_ONLY,
                                     imageSize * 4,
                                     NULL,
                                     &err);
    std::cout << "clResult error: " << err << "\n";

    // Create an extra buffer for debugging
    cl_int debug_buffer_size = 50;
    cl_mem clDebug = clCreateBuffer(context,
                                     CL_MEM_WRITE_ONLY,
                                     debug_buffer_size,
                                     NULL,
                                     &err);
    std::cout << "clDebug error: " << err << "\n";

    // Create Gaussian mask
    int maskSize = 1;  // actually sigma
    //float * mask = createBlurMask(10.0f, &maskSize);

    // Create buffer for mask
//    cl_mem clMask = clCreateBuffer(context,
//                                   CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
//                                   sizeof(float)*(maskSize*2+1)*(maskSize*2+1),
//                                   mask,
//                                   &err);
//    std::cout << "clMask error: " << err << "\n";

    // create Gaussian kernel
    cl_kernel gaussianBlur = clCreateKernel(program, "gaussian_blur", &err);
    std::cout << "cl_kernel error: " << err << "\n";

    // set kernel arguments
    err = clSetKernelArg(gaussianBlur, 0, sizeof(cl_mem), (void *)&clImage);
    std::cout << "kernel arg 0 error: " << err << "\n";
//    clSetKernelArg(gaussianBlur, 1, sizeof(cl_mem), &clMask);
    err = clSetKernelArg(gaussianBlur, 1, sizeof(cl_mem), (void *)&clResult);
    std::cout << "kernel arg 1 error: " << err << "\n";
    err = clSetKernelArg(gaussianBlur, 2, sizeof(int), &imageWidth);
    std::cout << "kernel arg 2 error: " << err << "\n";
    err = clSetKernelArg(gaussianBlur, 3, sizeof(int), &imageHeight);
    std::cout << "kernel arg 3 error: " << err << "\n";
    clSetKernelArg(gaussianBlur, 4, sizeof(cl_int), &maskSize);
    std::cout << "kernel arg 4 error: " << err << "\n";
    err = clSetKernelArg(gaussianBlur, 5, sizeof(cl_mem), &clDebug);
    std::cout << "kernel arg 5 error: " << err << "\n";
//CL_SUCCESS
    // load image to device
    err = clEnqueueWriteBuffer(queue,
                               clImage,
                               CL_TRUE,
                               0,
                               imageSize * 3,
                               (void*) &image.data[0],
                               0,
                               NULL,
                               NULL);
    std::cout << "enqueueWriteImage error: " << err << "\n";

    // Set local and global workgroup sizes
    size_t localws[2] = {16,16};
    size_t globalws[2] = {imageWidth, imageHeight};

    // Run Gaussian kernel
    err = clEnqueueNDRangeKernel(queue,
                                 gaussianBlur,
                                 2,
                                 NULL,
                                 globalws,
                                 localws,
                                 0,
                                 NULL,
                                 NULL);
    std::cout << "clEnqueueNDRangeKernel error: " << err << "\n";

    // Transfer image back to host
    err = clEnqueueReadBuffer(queue,
                              clResult,
                              CL_TRUE,
                              0,
                              imageSize * 4,
                              (void*) newData,
                              NULL,
                              NULL,
                              NULL);
    std::cout << "enqueueReadImage error: " << err << "\n";

    // Transfer debug buffer back to host
    float debug [debug_buffer_size];
    err = clEnqueueReadBuffer(queue,
                              clDebug,
                              CL_TRUE,
                              0,
                              debug_buffer_size,
                              (void*) debug,
                              NULL,
                              NULL,
                              NULL);
    std::cout << "clDebug read buffer error: " << err << "\n";
}

cv::Mat filter::getImage()
{
    return image;
}
