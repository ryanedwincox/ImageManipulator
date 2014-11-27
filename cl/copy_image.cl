// TODO convert to float4 to use GPU vector arithimitic optimization
// TODO make static parameters constant
#define BLOCK_SIZE 16  // size of local workspace
__kernel void filter_kernel(
        __global uchar * image, //bgr
        __global uchar * filteredImage, //bgr
        int imageWidth,
        int imageHeight,
        int maskSize,
	__global uchar * debugBuffer
    ) {
    // storing image data locally
    __local uchar P[3*BLOCK_SIZE*3*BLOCK_SIZE];
    // Identify workgroup
    int i = get_group_id(0);
    int j = get_group_id(1);
    // Indentify workitem
    int iDx = get_local_id(0);
    int iDy = get_local_id(1);
    int ii = i*BLOCK_SIZE + iDx;  // == get_global_id(0)
    int jj = j*BLOCK_SIZE + iDy;  // == get_global_id(1)

    int imgPosGlobal = (jj * imageWidth + ii) * 3;
    int imgPosLocal = (iDy * BLOCK_SIZE + iDx) * 3;

    // Read pixels
    P[imgPosLocal+0] = image[imgPosGlobal+0];
    P[imgPosLocal+1] = image[imgPosGlobal+1];
    P[imgPosLocal+2] = image[imgPosGlobal+2];
    barrier(CLK_LOCAL_MEM_FENCE);

    filteredImage[imgPosGlobal+0] = P[imgPosLocal+0];
    filteredImage[imgPosGlobal+1] = P[imgPosLocal+1];
    filteredImage[imgPosGlobal+2] = P[imgPosLocal+2];

    // This is cool, it shows each workgroup
    //filteredImage[imgPosGlobal+0] = P[0];
    //filteredImage[imgPosGlobal+1] = P[1];
    //filteredImage[imgPosGlobal+2] = P[2];

}

// TODO convert to float4 to use GPU vector arithimitic optimization
// TODO make static parameters constant
//__kernel void filter_kernel(
//        __global uchar * image, //bgr
//        __global uchar * filteredImage, //bgr
//        int imageWidth,
//        int imageHeight,
//        int maskSize,
//	__global uchar * debugBuffer
//    ) {
//    int xpos = get_global_id(0);
//    int ypos = get_global_id(1);
//
//    int imgPos = (ypos * imageWidth + xpos) * 3;
//    filteredImage[imgPos+0] = image[imgPos+0];
//    filteredImage[imgPos+1] = image[imgPos+1];
//    filteredImage[imgPos+2] = image[imgPos+2];
//
//}


