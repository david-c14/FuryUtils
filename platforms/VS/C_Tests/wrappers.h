typedef void * dat_p;

extern "C" int Test_GetExceptionCode();
extern "C" const char *Test_GetExceptionString();

extern "C" {

	dat_p Test_Dat_createNew();
	dat_p Test_Dat_create(uint8_t *buffer, uint32_t size);
	void Test_Dat_destroy(dat_p dat);
	int Test_Dat_entryCount(dat_p dat);
	void Test_Dat_reset(dat_p dat);
	uint8_t Test_Dat_next(dat_p dat, DatHeader_t * header);
	uint8_t Test_Dat_header(dat_p dat, uint32_t index, DatHeader_t * header);
	uint8_t Test_Dat_entry(dat_p dat, uint32_t index, uint8_t *buffer, uint32_t size);
	void Test_Dat_add(dat_p dat, char *fileName, uint8_t *buffer, uint32_t size, uint8_t compress);
	uint32_t Test_Dat_size(dat_p dat);
	uint8_t Test_Dat_buffer(dat_p dat, uint8_t *buffer, uint32_t size);

}
