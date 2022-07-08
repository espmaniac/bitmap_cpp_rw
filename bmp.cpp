#include <fstream>
#include <cstdio>
#include <cmath>
#include "bmp.hpp"

/* bits per pixel <= 8
	BITMAPFILEHEADER 14 byte
	BITMAPINFO       40 byte
	2^bitsPerPixel * RGBQUAD // RGBQUAD = 4 bytes
	array of pixels
*/

BMP::BMP() : width(info.biWidth), height(info.biHeight), bitCount(info.biBitCount),
	header{0}, info{0}, flip(0), absHeight(0), bytesWidth(0), padding(0) {}
BMP::~BMP() {}

void BMP::initBmp() {
	flip = (info.biHeight > 0) ? (info.biHeight - 1) : 0;
	absHeight = abs(info.biHeight);
	bytesWidth = (info.biWidth * info.biBitCount + 7) / 8; // width in bytes rounded up
	padding = (4 - (bytesWidth % 4)) % 4;

	header.bfType = 0x4D42;
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;

	info.biSize = sizeof(BITMAPINFO); // 40
	//info.biWidth set by reference
	//info.biHeight set by reference
	info.biPlanes = 1;
	//info.biBitCount set by reference
	info.biCompression = 0;
	info.biSizeImage = (((info.biBitCount * info.biWidth + 31) / 32) * 4) * absHeight;
	info.biXPelsPerMeter = 0;
	info.biYPelsPerMeter = 0;
	info.biClrUsed = 1 << info.biBitCount; // pow(2, info.biBitCount);
	info.biClrImportant	= 0;

	header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO);
	header.bfSize = header.bfOffBits + info.biSizeImage;

	if (info.biBitCount <= 8) {
		header.bfOffBits += info.biClrUsed * sizeof(RGBQUAD);
		colors.resize(1 << info.biBitCount);
	}

	data.resize(absHeight);
	for (uint32_t i = 0; i < data.size(); ++i)
		data[i].resize(bytesWidth);
}

void BMP::read(const char* path) {
	std::fstream file(path, std::ios::in |  std::ios::binary);

	if (!file.is_open())
		return;

	file.read((char*) &header, sizeof(BITMAPFILEHEADER));

	if (header.bfType != 0x4d42)
		return;

	file.read((char*) &info, sizeof(BITMAPINFO));

	if (info.biSize != 40 || info.biCompression != 0 || info.biPlanes != 1) 
		return;

	if (info.biBitCount <= 8) {
		colors.resize(1 << info.biBitCount); // info.biClrUsed can be 0
		file.read((char*) &colors[0], colors.size() * sizeof(RGBQUAD));
	}

	file.seekg(header.bfOffBits, std::ios::beg);

	flip = (info.biHeight > 0) ? (info.biHeight - 1) : 0;
	absHeight = abs(info.biHeight);
	bytesWidth = (info.biWidth * info.biBitCount + 7) / 8; // width in bytes rounded up
	padding = (4 - (bytesWidth % 4)) % 4;

	data.resize(absHeight);
	for (uint32_t i = 0; i < data.size(); ++i)
		data[i].resize(bytesWidth);

	for (int32_t row = 0; row < absHeight; ++row) {
		/*
		std::vector<uint8_t> scanLine(bytesWidth);
		file.read((char*) &scanLine[0], bytesWidth);
		//for (uint16_t col = 0; col < bytesWidth; ++col)
			//scanLine.push_back(file.get());
		data.insert(data.begin(), scanLine);
		*/
		file.read((char*) &data[abs(flip - row)][0], bytesWidth);

		file.seekg(padding, std::ios::cur);
	}
	

	file.close();
}

void BMP::write(const char* path) {
	std::fstream file(path, std::ios::out | std::ios::binary);

	if (!file.is_open())
		return;

	file.write((char*) &header, sizeof(BITMAPFILEHEADER));
	file.write((char*) &info, sizeof(BITMAPINFO));

	if (info.biBitCount <= 8)
		file.write((char*) &colors[0], colors.size() * sizeof(RGBQUAD));

	for (int32_t row = 0; row < absHeight; ++row) {
		file.write((char*) &data[abs(flip - row)][0], bytesWidth);
		/*
		for (uint16_t col = 0; col < bytesWidth; ++col)
			file.put(data[abs(flip - row)][col]);
		*/
		for (uint8_t i = 0; i < padding; ++i)
			file.put(0x00);
	}

	file.close();
}

const BITMAPFILEHEADER &BMP::getHeader() const {
	return header;
}

const BITMAPINFO &BMP::getInfo() const {
	return info;
}

int32_t BMP::getAbsHeight() const {
	return absHeight;
}

int32_t BMP::getFlip() const {
	return flip;
}

uint16_t BMP::getBytesWidth() const {
	return bytesWidth;
}

std::vector<RGBQUAD> &BMP::getColors() {
	return colors;
}

std::vector<std::vector<uint8_t>> &BMP::getData() {
	return data;
}
