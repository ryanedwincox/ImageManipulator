__kernel void filter_kernel(
        __global uchar * image, //bgr
        __global uchar * blurredImage, //bgr
	int imageWidth,
	int imageHeight,
        __private int maskSize,
	__global uchar * debugBuffer
    ) {
    int xpos = get_global_id(0);
    int ypos = get_global_id(1); 

    int r = maskSize / 2;

    clamp(xpos, r, imageWidth - r);
    clamp(ypos, r, imageHeight - r);

    int imgPos = (ypos * imageWidth + xpos) * 3;
    uchar sum0 = 0;
    uchar sum1 = 0;
    uchar sum2 = 0;

    for (int i = -r; i <= r; i++) {
	for (int j = -r; j <= r; j++) {
	    sum0 = sum0 + image[imgPos - imageWidth * j * 3 + i * 3 + 0] / (maskSize * maskSize);
	    sum1 = sum1 + image[imgPos - imageWidth * j * 3 + i * 3 + 1] / (maskSize * maskSize);
	    sum2 = sum2 + image[imgPos - imageWidth * j * 3 + i * 3 + 2] / (maskSize * maskSize);
	}
    }
    blurredImage[imgPos+0] = sum0;
    blurredImage[imgPos+1] = sum1;
    blurredImage[imgPos+2] = sum2;
}


