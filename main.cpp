#include "bmp.hpp"
#include "math.h"

int main() {
	BMP image;

	image.width = 33;
	image.height = 33;
	image.bitCount = 1;
	image.initBmp();
	image.getColors()[0] = {0,0,0,0};
	image.getColors()[1] = {255,255,255,0};

	for (int32_t y = 0; y < image.getAbsHeight(); ++y) {
		for (int32_t x = 0; x < image.width; ++x)
			if ((y % 2) && (x % 2))
				image.setPixel(x, y, 1);
	}

	printf("\nreadPixel %d\n", image.readPixel(0,0));

	image.write("images/grid.bmp");
	return 0;
}
