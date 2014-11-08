__kernel void filter_kernel(
        __global uchar * image, //bgr
        __global uchar * filteredImage, //bgr
	int imageWidth,
	int imageHeight,
        __private int maskSize,
	__global uchar * debugBuffer
    ) {
    int xpos = get_global_id(0);
    int ypos = get_global_id(1); 

    int imgPos = (ypos * imageWidth + xpos) * 3;
    filteredImage[imgPos+0] = image[imgPos+0];
    filteredImage[imgPos+1] = image[imgPos+1];
    filteredImage[imgPos+2] = image[imgPos+2];

}


