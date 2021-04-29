#pragma pack(push, 1)
struct APIENTRY DatHeader {
	char FileName[13];
	uint32_t UncompressedSize;
	uint32_t CompressedSize;
	uint8_t IsNotCompressed;
};
#pragma pack(pop)
