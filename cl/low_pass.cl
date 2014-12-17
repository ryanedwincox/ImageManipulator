// TODO convert to float4 to use GPU vector arithimitic optimization
// TODO make static parameters constant
#define BLOCK_SIZE 16  // size of local workspace
__kernel void filter_kernel(
        __global uchar * image, //bgr
        __global uchar * blurredImage, //bgr
        int imageWidth,
        int imageHeight,
        int maskSize,
	__global uchar * debugBuffer
    ) {

    int r = maskSize / 2;

    // storing image data locally
    __local uchar P[3*(BLOCK_SIZE+4)*3*(BLOCK_SIZE+4)];

    // Identify workgroup
    int i = get_group_id(0);
    int j = get_group_id(1);

    // TODO: avoid edge cases
    /*clamp(i, 1, imageWidth / 16 - 2);
    clamp(j, 1, imageHeight / 16 - 2);*/

    // Indentify workitem
    int iDx = get_local_id(0);
    int iDy = get_local_id(1);
    int ii = i*BLOCK_SIZE + iDx;  // == get_global_id(0)
    int jj = j*BLOCK_SIZE + iDy;  // == get_global_id(1)

    // Calculate local and global indexes
    int imgPosGlobal = (jj * imageWidth + ii) * 3;
    int imgPosLocal = (iDy * (BLOCK_SIZE+2*r) + iDx) * 3;

    // moves the local workspace from the center of the roi to the upper left corner of the roi
    int buffOffset = (2*BLOCK_SIZE+5*r)*3;

    // Read pixels in workspace
    P[imgPosLocal+0] = image[imgPosGlobal+0 - buffOffset];
    P[imgPosLocal+1] = image[imgPosGlobal+1 - buffOffset];
    P[imgPosLocal+2] = image[imgPosGlobal+2 - buffOffset];

    // Copies borders on the right side
    if (iDx < maskSize)  // for maskSize of 5 this is less than 4
    {
        // TODO: edge cases
        P[imgPosLocal+0 + BLOCK_SIZE*3] = image[imgPosGlobal+0 - buffOffset + BLOCK_SIZE*3];
        P[imgPosLocal+1 + BLOCK_SIZE*3] = image[imgPosGlobal+1 - buffOffset + BLOCK_SIZE*3];
        P[imgPosLocal+2 + BLOCK_SIZE*3] = image[imgPosGlobal+2 - buffOffset + BLOCK_SIZE*3];
    }
    // Copies borders of the bottom
    if (iDy < maskSize)
    {
        // TODO: edge cases
        P[imgPosLocal+0 + (BLOCK_SIZE+2*r)*BLOCK_SIZE*3] = image[imgPosGlobal+0 - buffOffset + BLOCK_SIZE*imageWidth*3];
        P[imgPosLocal+1 + (BLOCK_SIZE+2*r)*BLOCK_SIZE*3] = image[imgPosGlobal+1 - buffOffset + BLOCK_SIZE*imageWidth*3];
        P[imgPosLocal+2 + (BLOCK_SIZE+2*r)*BLOCK_SIZE*3] = image[imgPosGlobal+2 - buffOffset + BLOCK_SIZE*imageWidth*3];
    }
    // Copies bottom right corner
    if (iDx > BLOCK_SIZE - maskSize && iDy > BLOCK_SIZE - maskSize)
    {
        P[imgPosLocal+0 + (r*2 + (BLOCK_SIZE+r*2)*r*2)*3] = image[imgPosGlobal+0 - buffOffset + (r*2 + imageWidth*r*2)*3];
        P[imgPosLocal+1 + (r*2 + (BLOCK_SIZE+r*2)*r*2)*3] = image[imgPosGlobal+1 - buffOffset + (r*2 + imageWidth*r*2)*3];
        P[imgPosLocal+2 + (r*2 + (BLOCK_SIZE+r*2)*r*2)*3] = image[imgPosGlobal+2 - buffOffset + (r*2 + imageWidth*r*2)*3];
    }

    // Make sure all threads have finished loading all pixels
    barrier(CLK_LOCAL_MEM_FENCE);

    // copies image for testing, not need for convolution
    //blurredImage[imgPosGlobal+0 - buffOffset + 3] = P[imgPosLocal+0 + 3];
    //blurredImage[imgPosGlobal+1 - buffOffset + 3] = P[imgPosLocal+1 + 3];
    //blurredImage[imgPosGlobal+2 - buffOffset + 3] = P[imgPosLocal+2 + 3];

    // Perform convolution
    uchar sum0 = 0;
    uchar sum1 = 0;
    uchar sum2 = 0;

    for (int i = -r; i <= r; i++) {
        for (int j = -r; j <= r; j++) {
            sum0 = sum0 + P[imgPosLocal+0 + buffOffset + 3 + ((BLOCK_SIZE+r*2) * j + i)*3] / (maskSize * maskSize);
            sum1 = sum1 + P[imgPosLocal+1 + buffOffset + 3 + ((BLOCK_SIZE+r*2) * j + i)*3] / (maskSize * maskSize);
            sum2 = sum2 + P[imgPosLocal+2 + buffOffset + 3 + ((BLOCK_SIZE+r*2) * j + i)*3] / (maskSize * maskSize);
        }
    }
    blurredImage[imgPosGlobal+0 - buffOffset + 9] = sum0;
    blurredImage[imgPosGlobal+1 - buffOffset + 9] = sum1;
    blurredImage[imgPosGlobal+2 - buffOffset + 9] = sum2;
}

