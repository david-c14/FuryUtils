#include "../headers/header_Bmp.hpp"

#pragma pack(push, 1)
struct BitmapFileHeader {
	uint8_t b = 'B';
	uint8_t m = 'M';
	uint32_t size;
	uint32_t reserved = 0;
	uint32_t pixelOffset;
};

struct BitmapInfoHeader {
	uint32_t size = 40;
	uint32_t width;
	uint32_t height;
	uint16_t colourPlanes = 1;
	uint16_t bpp;
	uint32_t compression = 0;
	uint32_t imageSize;
	uint32_t horzResolution = 0;
	uint32_t vertResolution = 0;
	uint32_t coloursInPalette = 0;
	uint32_t importantColours = 0;
};
#pragma pack(pop)

Bmp::Bmp(std::vector<uint8_t> &inputPalette, std::vector<uint8_t> &inputPixels) : Imm(inputPalette, inputPixels) {
	BitmapFileHeader fh;
	uint32_t stride = 4 * ((_width / 4) + ((_width % 4) > 0));
	uint32_t paletteSize = sizeof(RGBAQuad) * _palette.size();
	fh.pixelOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + paletteSize;
	fh.size = fh.pixelOffset + stride * _height;
	BitmapInfoHeader ih;
	ih.width = _width;
	ih.height = _height;
	ih.bpp = 8;
	ih.imageSize = stride * _height;
	std::vector<uint8_t> newBuffer(fh.size);

	uint8_t *bufferArray = newBuffer.data();
	memcpy(bufferArray, &fh, sizeof(BitmapFileHeader));
	bufferArray += sizeof(BitmapFileHeader);
	memcpy(bufferArray, &ih, sizeof(BitmapInfoHeader));
	bufferArray += sizeof(BitmapInfoHeader);
	RGBAQuad quad;
	quad.a = 0;
	for (unsigned int i = 0; i < _palette.size(); i++) {
		quad.r = _palette[i].r;
		quad.g = _palette[i].g;
		quad.b = _palette[i].b;
		memcpy(bufferArray, &quad, sizeof(RGBAQuad));
		bufferArray += sizeof(RGBAQuad);
	}

	uint8_t *pixelArray = _pixels.data();
	unsigned int i = _height;
	while (i) {
		i--;
		memcpy(bufferArray + i * stride, pixelArray, _width);
		pixelArray += _width;
	}
	_outputBuffer.swap(newBuffer);
}