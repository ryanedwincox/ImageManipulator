#include "filter.h"

// Constructor
// Builds OpenCl program
filter::filter(cl_context context, cl_uint deviceCount, cl_device_id* devices, const char* clPath, cl_int maskSize)
{
    this->context = context;
    this->clPath = clPath;
    this->maskSize = maskSize;

    // get size of kernel source
    const char* kernelSource = clPath;
    programHandle = fopen(kernelSource, "r");
    fseek(programHandle, 0, SEEK_END);
    programSize = ftell(programHandle);
    rewind(programHandle);

    // read kernel source into buffer
    programBuffer = (char*) malloc(programSize + 1);
    programBuffer[programSize] = '\0';
    fread(programBuffer, sizeof(char), programSize, programHandle);
    fclose(programHandle);

    // create program from buffer
    program = clCreateProgramWithSource(context, 1,
            (const char**) &programBuffer, &programSize, NULL);

    // build program
    const char* buildOptions = "";
    err = clBuildProgram(program, deviceCount, devices, buildOptions, NULL, NULL);
    std::cout << "program error: " << err << "\n";

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

    //create queue to which we will push commands for the device
    queue = clCreateCommandQueue(context,devices[0],0,&err);
    std::cout << "command queue error: " << err << "\n";

    // build kernel
    kernel = clCreateKernel(program, "filter_kernel", &err);
    std::cout << "cl_kernel error: " << err << "\n";
}

// Stores image to process
// Creates buffers to store image on device
void filter::setImage(cv::Mat img)
{
    std::cout << "Creating image buffers" << std::endl;
    image = img;

    imageWidth = image.cols;
    imageHeight = image.rows;
    imageSize = imageHeight * imageWidth;

    // Create an OpenCL buffer for the image
    clImage = clCreateBuffer(context,
                             CL_MEM_READ_ONLY,
                             imageSize * 3,
                             NULL,
                             &err);
    std::cout << "clImage Buffer error: " << err << "\n";

    // Create an OpenCL buffer for the result
    clResult = clCreateBuffer(context,
                              CL_MEM_WRITE_ONLY,
                              imageSize * 4,
                              NULL,
                              &err);
    std::cout << "clResult Buffer error: " << err << "\n";

    // Create an extra buffer for debugging
    clDebug = clCreateBuffer(context,
                             CL_MEM_WRITE_ONLY,
                             DEBUG_BUFFER_SIZE,
                             NULL,
                             &err);
    std::cout << "clDebug Buffer error: " << err << "\n";

}

void* filter::runProgram()
{
    unsigned char newData [imageSize * 4];

    // set kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&clImage);
    std::cout << "kernel arg 0 error: " << err << "\n";
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&clResult);
    std::cout << "kernel arg 1 error: " << err << "\n";
    err = clSetKernelArg(kernel, 2, sizeof(int), &imageWidth);
    std::cout << "kernel arg 2 error: " << err << "\n";
    err = clSetKernelArg(kernel, 3, sizeof(int), &imageHeight);
    std::cout << "kernel arg 3 error: " << err << "\n";
    err = clSetKernelArg(kernel, 4, sizeof(cl_int), &maskSize);
    std::cout << "kernel arg 4 error: " << err << "\n";
    err = clSetKernelArg(kernel, 5, sizeof(cl_mem), &clDebug);
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

    // Run filter kernel
    err = clEnqueueNDRangeKernel(queue,
                                 kernel,
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
                              0,
                              NULL,
                              NULL);
    std::cout << "enqueueReadImage error: " << err << "\n";

    // Transfer debug buffer back to host
    //float debug [DEBUG_BUFFER_SIZE];
    unsigned char debug [DEBUG_BUFFER_SIZE];
    err = clEnqueueReadBuffer(queue,
                              clDebug,
                              CL_TRUE,
                              0,
                              DEBUG_BUFFER_SIZE,
                              debug,
                              0,
                              NULL,
                              NULL);
    std::cout << "clDebug read buffer error: " << err << "\n";

    return newData;
}

cv::Mat filter::getImage()
{
    return image;
}
