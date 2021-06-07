#include "../../../include/exceptions.h"
#include "../../../include/dat.h"

int Test_GetExceptionCode() {
	return GetExceptionCode();
}

const char *Test_GetExceptionString() {
	return GetExceptionString();
}

dat_p Test_Dat_createNew() {
	return Dat_createNew();
}

dat_p Test_Dat_create(uint8_t *buffer, uint32_t size) {
	return Dat_create(buffer, size);
}

void Test_Dat_destroy(dat_p dat) {
	Dat_destroy(dat);
}

int Test_Dat_entryCount(dat_p dat) {
	return Dat_entryCount(dat);
}

void Test_Dat_reset(dat_p dat) {
	Dat_reset(dat);
}

uint8_t Test_Dat_next(dat_p dat, DatHeader_t * header) {
	return Dat_next(dat, header);
}

uint8_t Test_Dat_header(dat_p dat, uint32_t index, DatHeader_t * header) {
	return Dat_header(dat, index, header);
}

uint8_t Test_Dat_entry(dat_p dat, uint32_t index, uint8_t *buffer, uint32_t size) {
	return Dat_entry(dat, index, buffer, size);
}

void Test_Dat_add(dat_p dat, char *fileName, uint8_t *buffer, uint32_t size, uint8_t compress) {
	Dat_add(dat, fileName, buffer, size, compress);
}

uint32_t Test_Dat_size(dat_p dat) {
	return Dat_size(dat);
}

uint8_t Test_Dat_buffer(dat_p dat, uint8_t *buffer, uint32_t size) {
	return Dat_buffer(dat, buffer, size);
}
