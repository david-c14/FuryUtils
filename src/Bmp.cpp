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
#pragma pack(pop)

Bmp::Bmp(std::vector<uint8_t> &inputPalette, std::vector<uint8_t> &inputPixels) : Imm(inputPalette, inputPixels) {
	MakeBmp();
}

Bmp::Bmp(std::vector<uint8_t> &inputBuffer) {
	uint32_t inputSize = inputBuffer.size();
	if (inputSize < sizeof(BitmapFileHeader) + sizeof(uint32_t)) {
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, "Buffer is too short to contain a valid Bmp");
	}
	uint8_t *inputArray = inputBuffer.data();
	BitmapFileHeader fh;
	memcpy(&fh, inputArray, sizeof(BitmapFileHeader));
	inputArray += sizeof(BitmapFileHeader);
	if (fh.b != 'B') {
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, "First character of buffer is not 'B'");
	}
	if (fh.m != 'M') {
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, "Second character of buffer is not 'M'");
	}
	if (inputSize != fh.size) {
		Exceptions::ERROR(Exceptions::BUFFER_OVERFLOW, "Buffer size does not agree with declared size in header");
	}

	uint32_t paletteCount = 0;
	bool topToBottom = false;

	uint32_t headerSize = *inputArray;
	headerSize += 0x100 * *(inputArray + 1);
	headerSize += 0x10000 * *(inputArray + 2);
	headerSize += 0x1000000 * *(inputArray + 3);
	switch (headerSize) {
	case 40:
		if (inputSize < sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader)) {
			Exceptions::ERROR(Exceptions::BUFFER_OVERFLOW, "Buffer size is too small to hold info structure");
		}
		{
			BitmapInfoHeader ih;
			memcpy(&ih, inputArray, sizeof(BitmapInfoHeader));
			inputArray += sizeof(BitmapInfoHeader);
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
		if (inputSize < sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + paletteCount * sizeof(RGBAQuad)) {
			Exceptions::ERROR(Exceptions::BUFFER_OVERFLOW, "Buffer size is too small to hold specified Bmp");
		}
		break;
	default:
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, "Info header type is not recognized");
	}
	if (_depth != 4 && _depth != 8) {
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, "Only 16-color and 256-color Bmp are supported");
	}
	uint32_t stride = 4 * ((_width / (32/_depth)) + ((_width % (32/_depth)) > 0));
	if (inputSize < fh.pixelOffset + stride * _height) {
		Exceptions::ERROR(Exceptions::BUFFER_OVERFLOW, "Buffer is too small to hold Bmp pixel data");
	}
	if (paletteCount > (1u << _depth)) {
		Exceptions::ERROR(Exceptions::INVALID_FORMAT, "Declared palette size is too large");
	}
	std::vector<RGBTriple> paletteVector((1 << _depth));
	paletteVector.swap(_palette);
	RGBAQuad quad;
	for (unsigned int i = 0; i < paletteCount; i++) {
		
		memcpy(&quad, inputArray, sizeof(RGBAQuad));
		inputArray += sizeof(RGBAQuad);
		_palette[i].r = quad.r;
		_palette[i].g = quad.g;
		_palette[i].b = quad.b;
	}
	inputArray = inputBuffer.data() + fh.pixelOffset;
	std::vector<uint8_t> pixelVector(stride * _height);
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
					bufferArray[i * stride + j / 2] += (*pixelArray) & 0x0f;
				}
				pixelArray++;
			}
		}
	}
	_outputBuffer.swap(newBuffer);
}