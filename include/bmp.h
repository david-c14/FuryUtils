#ifndef __BMP_H__
#define __BMP_H__

typedef void* bmp_p;

bmp_p Bmp_createFromBmp(uint8_t *buffer, uint32_t size);
bmp_p Bmp_createFromImmAndPam(uint8_t *pixelBuffer, uint32_t pixelSize, uint8_t *paletteBuffer, uint32_t paletteSize);
void Bmp_destroy(bmp_p bmp);

#endif
