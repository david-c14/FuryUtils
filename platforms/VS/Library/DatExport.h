#pragma once

#ifndef APIENTRY
#define APIENTRY
#endif

#ifndef DAT_P
typedef void* dat_p;
#endif

extern "C" {
	APIENTRY dat_p __cdecl Dat_createNew();

	APIENTRY dat_p __cdecl Dat_create(uint8_t *buffer, int size);

	APIENTRY void __cdecl Dat_destroy(dat_p dat);

	APIENTRY int __cdecl Dat_entryCount(dat_p dat);

	APIENTRY void __cdecl Dat_reset(dat_p dat);

	APIENTRY bool __cdecl Dat_next(dat_p dat, DatHeader * header);

	APIENTRY bool __cdecl Dat_header(dat_p dat, uint32_t index, DatHeader * header);

	APIENTRY bool __cdecl Dat_entry(dat_p dat, uint32_t index, uint8_t *buffer, uint32_t size);

	APIENTRY void __cdecl Dat_add(dat_p dat, char *fileName, uint8_t *buffer, uint32_t size, bool compress);

	APIENTRY uint32_t __cdecl Dat_size(dat_p dat);

	APIENTRY bool __cdecl Dat_buffer(dat_p dat, uint8_t *buffer, uint32_t size);
}
