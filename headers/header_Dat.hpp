#pragma once
#include <vector>
#include "header_Exceptions.hpp"

#ifndef APIENTRY
#define APIENTRY
#endif

#pragma pack(push, 1)
struct APIENTRY DatHeader {
	char FileName[13];
	uint32_t UncompressedSize;
	uint32_t CompressedSize;
	uint8_t IsNotCompressed;
};

struct APIENTRY DatEntry {
	DatHeader Header;
	uint32_t CompressedBufferOffset;
};
#pragma pack(pop)

#pragma warning(push)
#pragma warning(disable:4251)

struct APIENTRY Dat {

private:

	std::vector<uint8_t> fileBuffer;
	uint16_t entryCount = 0;
	std::vector<DatEntry> entries;
	int32_t entryIteration = -1;

	void InternalEntry(std::vector<uint8_t> &inputBuffer, uint16_t index);
	void Uncompress(std::vector<uint8_t> &inputBuffer, uint32_t uncompressedSize);
	void Compress(std::vector<uint8_t> &inputBuffer);

public:

	Dat();
	Dat(std::vector<uint8_t> &inputBuffer);
	void Add(const char *fileName, std::vector<uint8_t> &inputBuffer, bool compress);
	uint16_t EntryCount();
	void Reset();
	DatHeader *Next();
	DatHeader *Header(uint32_t index);
	bool Entry(std::vector<uint8_t> &inputBuffer);
	bool Entry(uint16_t index, std::vector<uint8_t> &inputBuffer);
	uint32_t Size();
	void Buffer(std::vector<uint8_t> &inputBuffer);
};

#pragma warning(pop)
