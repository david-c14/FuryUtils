#pragma once
#include <vector>
#include "header_Exceptions.hpp"

#ifndef APIEXPORT
#define APIEXPORT
#endif

#pragma pack(push, 1)
struct APIEXPORT DatHeader {
	char FileName[13];
	uint32_t UncompressedSize;
	uint32_t CompressedSize;
	uint8_t IsNotCompressed;
};

struct DatEntry {
	DatHeader Header;
	uint32_t CompressedBufferOffset;
};
#pragma pack(pop)

struct Dat {

private:

	std::vector<char> fileBuffer;
	uint16_t entryCount = 0;
	std::vector<DatEntry> entries;
	int32_t entryIteration = -1;

	void InternalEntry(std::vector<char> &inputBuffer, uint16_t index);
	void Uncompress(std::vector<char> &inputBuffer, uint32_t uncompressedSize);
	void Compress(std::vector<char> &inputBuffer);

public:

	Dat();
	Dat(std::vector<char> &inputBuffer);
	void Add(const char *fileName, std::vector<char> &inputBuffer, bool compress);
	uint16_t EntryCount();
	void Reset();
	DatHeader *Next();
	DatHeader *Header(uint32_t index);
	bool Entry(std::vector<char> &inputBuffer);
	bool Entry(uint16_t index, std::vector<char> &inputBuffer);
	uint32_t Size();
	void Buffer(std::vector<char> &inputBuffer);
};
