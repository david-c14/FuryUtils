#pragma once

#ifndef APIENTRY
#define APIENTRY
#endif

#ifndef BMP_P
typedef void* bmp_p;
#endif

extern "C" {

	APIENTRY bmp_p __cdecl Bmp_createFromBmp(uint8_t *buffer, uint32_t size);

	APIENTRY bmp_p __cdecl Bmp_createFromImmAndPam(uint8_t *pixelBuffer, uint32_t pixelSize, uint8_t *paletteBuffer, uint32_t paletteSize);

	APIENTRY void __cdecl Bmp_destroy(bmp_p bmp);

}