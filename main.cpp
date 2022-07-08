#include "bmp.hpp"
#include <cstdio>
#include <math.h>

int main() {
	BMP image;

	image.width = 32;
	image.height = 32;
	image.bitCount = 24;
	image.initBmp();

	for (uint32_t i= 0; i < image.getAbsHeight(); ++i) {
		for (uint32_t j = 0; j < image.getBytesWidth(); ++j)
			image.getData()[abs(image.getFlip() - i)][j] = i * j * 155;
	}
	
	image.write("images/24bpp.bmp");

	image.width = 32;
	image.height = 32;
	image.bitCount = 1;
	image.initBmp();
	image.getColors()[0] = {0,0,0,0};
	image.getColors()[1] = {255,255,255,0};
	for (uint32_t i= 0; i < image.getAbsHeight(); ++i) {
		for (uint32_t j = 0; j < image.getBytesWidth(); ++j)
			image.getData()[abs(image.getFlip() - i)][j] = 0b11110000;
	}
	image.write("images/1bpp.bmp");
	return 0;
}
