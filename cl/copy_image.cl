__kernel void filter_kernel(
        __global uchar * image, //bgr
        //__constant float * mask,
        __global uchar * filteredImage, //bgra
	int imageWidth,
	int imageHeight,
        __private int maskSize,
	__global uchar * debugBuffer
    ) {
    int xpos = get_global_id(0);
    int ypos = get_global_id(1); 

    int imgPos = (ypos * imageWidth + xpos) * 3;
    int filImgPos = (ypos * imageWidth + xpos) * 4;
    filteredImage[filImgPos+0] = image[imgPos+0];
    filteredImage[filImgPos+1] = image[imgPos+1];
    filteredImage[filImgPos+2] = image[imgPos+2];
    filteredImage[filImgPos+3] = 100;

}

