#pragma once

#include <cstdint>
#include <vector>

#pragma pack(push, 1)
struct BITMAPFILEHEADER {
	uint16_t bfType; // BM
	uint32_t bfSize;
	uint16_t bfReserved1; // 0
	uint16_t bfReserved2; // 0
	uint32_t bfOffBits;
};

struct BITMAPINFO {
	uint32_t biSize; // 40
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biPlanes; // 1
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage; // rowSize * imageHeight in pixels
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrUsed; // 0 default to 2^info.biBitCount 
	uint32_t biClrImportant;
};

struct RGBQUAD { // if bits per pixel <= 8
	uint8_t blue, green, red, reserved;
};
#pragma pack(pop)

class BMP {
private:
	BITMAPFILEHEADER header;
	BITMAPINFO info;

	int32_t flip, absHeight;
	uint16_t bytesWidth, padding;

	std::vector<std::vector<uint8_t> > data;
	std::vector<RGBQUAD> colors;
public:
	BMP();
	~BMP();

	void initBmp();

	void read(const char*);

	void write(const char*);

	void setPixel(int32_t, int32_t, uint8_t);

	uint8_t readPixel(int32_t, int32_t) const;

	const BITMAPFILEHEADER &getHeader() const;

	const BITMAPINFO &getInfo() const;

	int32_t getAbsHeight() const;

	int32_t getFlip() const;

	uint16_t getBytesWidth() const;

	std::vector<RGBQUAD> &getColors();

	std::vector<std::vector<uint8_t>> &getData();

	int32_t &width; // info.biWidth
	int32_t &height; // info.biHeight
	uint16_t &bitCount; // info.biBitCount
};
