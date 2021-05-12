#ifndef __DAT_H__
#define __DAT_H__

typedef void* dat_p;

#pragma pack(push, 1)
struct _declspec(dllimport) DatHeader {
	char FileName[13];
	uint32_t UncompressedSize;
	uint32_t CompressedSize;
	uint8_t IsNotCompressed;
};
#pragma pack(pop)

dat_p Dat_createNew();
dat_p Dat_create(uint8_t *buffer, uint32_t size);
void Dat_destroy(dat_p dat);
int Dat_entryCount(dat_p dat);
void Dat_reset(dat_p dat);
uint8_t Dat_next(dat_p dat, DatHeader * header);
uint8_t Dat_header(dat_p dat, uint32_t index, DatHeader * header);
uint8_t Dat_entry(dat_p dat, uint32_t index, uint8_t *buffer, uint32_t size);
void Dat_add(dat_p dat, char *fileName, uint8_t *buffer, uint32_t size, uint8_t compress);
uint32_t Dat_size(dat_p dat);
uint8_t Dat_buffer(dat_p dat, uint8_t *buffer, uint32_t size);

#endif