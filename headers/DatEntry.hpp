#pragma pack(push, 1)
struct APIENTRY DatEntry {
	DatHeader Header;
	uint32_t CompressedBufferOffset;
};
#pragma pack(pop)
