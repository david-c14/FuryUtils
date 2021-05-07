#define APIENTRY __declspec(dllexport)
#include "../../../headers/BinaryIO.hpp"
#include "../../../src/Bmp.cpp"

#ifndef BMP_P
#define BMP_P
typedef Bmp* bmp_p;
#endif

#include "BmpExport.h"

extern "C" {

	Bmp * __cdecl Bmp_createFromBmp(uint8_t *buffer, uint32_t size) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			std::vector<uint8_t> vBuffer(buffer, buffer + size);
			return new Bmp(vBuffer);
		}
		catch (...) {
			Exceptions::HANDLE();
			return NULL;
		}
	}

	Bmp * __cdecl Bmp_createFromImmAndPam(uint8_t *pixelBuffer, uint32_t pixelSize, uint8_t *paletteBuffer, uint32_t paletteSize) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			std::vector<uint8_t> vBufferPalette(paletteBuffer, paletteBuffer + paletteSize);
			std::vector<uint8_t> vBufferPixel(pixelBuffer, pixelBuffer + pixelSize);
			return new Bmp(vBufferPalette, vBufferPixel);
		}
		catch (...) {
			Exceptions::HANDLE();
			return NULL;
		}
	}

	void __cdecl Bmp_destroy(Bmp *bmp) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			delete bmp;
		}
		catch (...) {
			Exceptions::HANDLE();
		}
	}

}
