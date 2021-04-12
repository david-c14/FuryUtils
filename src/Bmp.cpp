#include "../headers/header_Bmp.hpp"

#pragma pack(push, 1)
struct BitmapFileHeader {
	uint8_t b = 'B';
	uint8_t m = 'M';
	uint32_t size;
	uint32_t reserved = 0;
	uint32_t pixelOffset;
};

struct BitmapCoreHeader {
	uint32_t size;
	uint16_t width;
	uint16_t height;
	uint16_t colourPlanes;
	uint16_t bpp;
};

struct BitmapInfoHeader {
	uint32_t size = 40;
	uint32_t width;
	int32_t height;
	uint16_t colourPlanes = 1;
	uint16_t bpp;
	uint32_t compression = 0;
	uint32_t imageSize;
	uint32_t horzResolution = 0;
	uint32_t vertResolution = 0;
	uint32_t coloursInPalette = 0;
	uint32_t importantColours = 0;
};

struct BGRTriple {
	uint8_t b;
	uint8_t g;
	uint8_t r;
};
#pragma pack(pop)

Bmp::Bmp(std::vector<uint8_t> &inputPalette, std::vector<uint8_t> &inputPixels) : Imm(inputPalette, inputPixels) {
	MakeBmp();
}

Bmp::Bmp(std::vector<uint8_t> &inputBuffer) {
	uint32_t inputSize = inputBuffer.size();
	if (inputSize < sizeof(BitmapFileHeader) + sizeof(uint32_t)) {
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, Exceptions::ERROR_BMP_SHORT_HEADER);
	}
	uint8_t *inputArray = inputBuffer.data();
	BitmapFileHeader fh;
	memcpy(&fh, inputArray, sizeof(BitmapFileHeader));
	inputArray += sizeof(BitmapFileHeader);
	if (fh.b != 'B') {
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, Exceptions::ERROR_BMP_HEADER_MAGIC);
	}
	if (fh.m != 'M') {
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, Exceptions::ERROR_BMP_HEADER_MAGIC);
	}
	if (inputSize != fh.size) {
		Exceptions::ERROR(Exceptions::BUFFER_OVERFLOW, Exceptions::ERROR_BMP_SIZE_MISMATCH);
	}

	uint32_t paletteCount = 0;
	bool topToBottom = false;
	bool core = false;

	uint32_t headerSize = *inputArray;
	headerSize += 0x100 * *(inputArray + 1);
	headerSize += 0x10000 * *(inputArray + 2);
	headerSize += 0x1000000 * *(inputArray + 3);
	switch (headerSize) {
	case 40:
	case 108:
	case 124:
		if (inputSize < sizeof(BitmapFileHeader) + headerSize) {
			Exceptions::ERROR(Exceptions::BUFFER_OVERFLOW, Exceptions::ERROR_BMP_INFO_SIZE_MISMATCH);
		}
		{
			BitmapInfoHeader ih;
			memcpy(&ih, inputArray, sizeof(BitmapInfoHeader));
			inputArray += headerSize;
			_width = ih.width;
			if (ih.height < 0) {
				topToBottom = true;
				_height = -(ih.height);
			}
			else {
				_height = ih.height;
			}
			_depth = ih.bpp;
			paletteCount = ih.coloursInPalette ? ih.coloursInPalette : (1 << _depth);
		}
		if (inputSize < sizeof(BitmapFileHeader) + headerSize + paletteCount * sizeof(RGBAQuad)) {
			Exceptions::ERROR(Exceptions::BUFFER_OVERFLOW, Exceptions::ERROR_BMP_IMAGE_SIZE_MISMATCH);
		}
		break;
	case 12:
		core = true;
		if (inputSize < sizeof(BitmapFileHeader) + headerSize) {
			Exceptions::ERROR(Exceptions::BUFFER_OVERFLOW, Exceptions::ERROR_BMP_INFO_SIZE_MISMATCH);
		}
		{
			BitmapCoreHeader ch;
			memcpy(&ch, inputArray, sizeof(BitmapCoreHeader));
			inputArray += headerSize;
			_width = ch.width;
			_height = ch.height;
			_depth = ch.bpp;
			paletteCount = (1 << _depth);
		}
		if (inputSize < sizeof(BitmapFileHeader) + headerSize + paletteCount * sizeof(BGRTriple)) {
			Exceptions::ERROR(Exceptions::BUFFER_OVERFLOW, Exceptions::ERROR_BMP_IMAGE_SIZE_MISMATCH);
		}
		break;
	default:
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, Exceptions::ERROR_BMP_UNSUPPORTED_VERSION);
	}
	if (_depth != 4 && _depth != 8) {
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, Exceptions::ERROR_BMP_UNSUPPORTED_DEPTH);
	}
	uint32_t stride = 4 * ((_width / (32/_depth)) + ((_width % (32/_depth)) > 0));
	if (inputSize < fh.pixelOffset + stride * _height) {
		Exceptions::ERROR(Exceptions::BUFFER_OVERFLOW, Exceptions::ERROR_BMP_IMAGE_SIZE_MISMATCH);
	}
	if (paletteCount > (1u << _depth)) {
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, Exceptions::ERROR_BMP_PALETTE_SIZE_MISMATCH);
	}
	std::vector<RGBTriple> paletteVector((1 << _depth));
	paletteVector.swap(_palette);
	if (core) {
		BGRTriple triple;
		for (unsigned int i = 0; i < paletteCount; i++) {

			memcpy(&triple, inputArray, sizeof(BGRTriple));
			inputArray += sizeof(BGRTriple);
			_palette[i].r = triple.r;
			_palette[i].g = triple.g;
			_palette[i].b = triple.b;
		}
	}
	else {
		RGBAQuad quad;
		for (unsigned int i = 0; i < paletteCount; i++) {

			memcpy(&quad, inputArray, sizeof(RGBAQuad));
			inputArray += sizeof(RGBAQuad);
			_palette[i].r = quad.r;
			_palette[i].g = quad.g;
			_palette[i].b = quad.b;
		}
	}
	inputArray = inputBuffer.data() + fh.pixelOffset;
	std::vector<uint8_t> pixelVector(_width * _height);
	pixelVector.swap(_pixels);
	uint32_t start = topToBottom?0:(_height - 1);
	uint32_t end = topToBottom?_height:-1;
	uint32_t dir = topToBottom?1:-1;
	uint8_t *pixelArray = _pixels.data();
	if (_depth == 8) {
		for (signed int i = start; i != end; i += dir) {
			memcpy(pixelArray, inputArray + i * stride, _width);
			pixelArray += _width;
		}
	}
	else {
		for (signed int i = start; i != end; i += dir) {
			for (unsigned int j = 0; j < _width; j += 2) {
				uint8_t pair = inputArray[i * stride + j / 2];
				*pixelArray++ = pair >> 4;
				if (j < _width - 1u) {
					*pixelArray++ = pair & 0xf;
				}
			}
		}
	}
	MakeBmp();
}

void Bmp::MakeBmp() {
	BitmapFileHeader fh;
	uint32_t stride = 4 * ((_width / (32 / _depth)) + ((_width % (32 / _depth)) > 0));
	uint32_t paletteSize = sizeof(RGBAQuad) * _palette.size();
	fh.pixelOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + paletteSize;
	fh.size = fh.pixelOffset + stride * _height;
	BitmapInfoHeader ih;
	ih.width = _width;
	ih.height = _height;
	ih.bpp = _depth;
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
	if (_depth == 8) {
		while (i) {
			i--;
			memcpy(bufferArray + i * stride, pixelArray, _width);
			pixelArray += _width;
		}
	}
	else {
		while (i) {
			i--;
			for (unsigned int j = 0; j < _width; j += 2) {
				bufferArray[i * stride + j / 2] = (*pixelArray++) << 4;
				if (j < _width - 1u) {
					bufferArray[i * stride + j / 2] += (*pixelArray++) & 0x0f;
				}
			}
		}
	}
	_outputBuffer.swap(newBuffer);
}