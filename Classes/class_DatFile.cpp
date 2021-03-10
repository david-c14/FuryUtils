#include "header_DatFile.hpp"

void DatFile::InternalEntry(std::vector<char> &inputBuffer, uint16_t index) {
	uint32_t start = entries[index].CompressedBufferOffset;
	uint32_t end = start + entries[index].Header.CompressedSize;
	std::vector<char> copyBuffer(fileBuffer.begin() + start, fileBuffer.begin() + end);
	if (!entries[index].Header.IsNotCompressed) {
		Uncompress(copyBuffer, entries[index].Header.UncompressedSize);
	}
	copyBuffer.swap(inputBuffer);
}

void DatFile::Uncompress(std::vector<char> &inputBuffer, uint32_t uncompressedSize) {
	std::vector<char> outputBuffer(uncompressedSize);
	uint32_t inputOffset = 0;
	uint32_t outputOffset = 0;
	uint8_t compressionBits;
	uint8_t compressionBitCount = 0;
	while (outputOffset < uncompressedSize) {
		if (!compressionBitCount) {
			BinaryRead::CheckSpace(inputBuffer, inputOffset, 1);
			compressionBits = BinaryRead::ReadUInt8(inputBuffer, inputOffset);
			compressionBitCount = 8;
		}
		bool uncompressed = compressionBits & 0x01;
		compressionBitCount--;
		compressionBits >>= 1;
		if (uncompressed) {
			BinaryRead::CheckSpace(inputBuffer, inputOffset, 1);
			outputBuffer[outputOffset++] = inputBuffer[inputOffset++];
		}
		else {
			BinaryRead::CheckSpace(inputBuffer, inputOffset, 2);
			uint8_t byte1 = BinaryRead::ReadUInt8(inputBuffer, inputOffset);
			uint8_t byte2 = BinaryRead::ReadUInt8(inputBuffer, inputOffset);
			uint8_t length = (byte2 & 0x0f) + 3;
			if ((outputOffset + length) > uncompressedSize) {
				throw "Compression overflow";
			}
			int32_t offset = ((byte2 >> 4) << 8) + byte1;
			offset += 0x1012;
			offset += (outputOffset & 0xfffff000);
			while (offset >= (int32_t)outputOffset)
				offset -= 0x1000;
			while (length--) {
				if (offset < 0) {
					outputBuffer[outputOffset++] = ' ';
				}
				else {
					outputBuffer[outputOffset++] = outputBuffer[offset];
				}
				offset++;
			}
		}
	}
	outputBuffer.swap(inputBuffer);
}

void DatFile::Compress(std::vector<char> &inputBuffer) {
	return;
}

DatFile::DatFile(std::vector<char> &inputBuffer) {
	fileBuffer.clear();
	fileBuffer.swap(inputBuffer);
	uint32_t bufferOffset = 0;
	BinaryRead::CheckSpace(fileBuffer, bufferOffset, 2);
	entryCount = BinaryRead::ReadUInt16(fileBuffer, bufferOffset);
	entries.resize(entryCount);
	for (unsigned int i = 0; i < entryCount; i++) {
		BinaryRead::CheckSpace(fileBuffer, bufferOffset, sizeof(DatFileHeader));
		strncpy_s(entries[i].Header.FileName, 13, &(fileBuffer[bufferOffset]), 13);
		entries[i].Header.FileName[12] = '\0';
		bufferOffset += 13;
		entries[i].Header.UncompressedSize = BinaryRead::ReadUInt32(fileBuffer, bufferOffset);
		entries[i].Header.CompressedSize = BinaryRead::ReadUInt32(fileBuffer, bufferOffset);
		entries[i].Header.IsNotCompressed = BinaryRead::ReadUInt8(fileBuffer, bufferOffset);
		entries[i].CompressedBufferOffset = bufferOffset;
		BinaryRead::CheckSpace(fileBuffer, bufferOffset, entries[i].Header.CompressedSize);
		bufferOffset += entries[i].Header.CompressedSize;
	}
}

DatFile::DatFile() {
	fileBuffer.resize(2);
	uint32_t bufferOffset = 0;
	BinaryRead::WriteUInt16(fileBuffer, bufferOffset, entryCount);
}

void DatFile::Add(const char *fileName, std::vector<char> &inputBuffer, bool compress) {
	DatFileEntry entry;
	strncpy_s(entry.Header.FileName, 13, fileName, 12);
	entry.Header.FileName[12] = '\0';
	entry.Header.UncompressedSize = inputBuffer.size();
	if (compress) {
		Compress(inputBuffer);
	}
	entry.Header.CompressedSize = inputBuffer.size();
	entry.Header.IsNotCompressed = (entry.Header.CompressedSize >= entry.Header.UncompressedSize);

	uint32_t bufferOffset = 0;
	BinaryRead::WriteUInt16(fileBuffer, bufferOffset, ++entryCount);
	bufferOffset = fileBuffer.size();
	fileBuffer.resize(bufferOffset + sizeof(DatFileHeader) + entry.Header.CompressedSize);
	entry.CompressedBufferOffset = bufferOffset;
	memcpy(fileBuffer.data() + bufferOffset, &(entry.Header), sizeof(DatFileHeader));
	bufferOffset += sizeof(DatFileHeader);
	memcpy(fileBuffer.data() + bufferOffset, inputBuffer.data(), entry.Header.CompressedSize);

	entries.push_back(entry);
}

uint16_t DatFile::EntryCount() {
	return entryCount;
}

void DatFile::Reset() {
	entryIteration = -1;
}

DatFileHeader *DatFile::Next() {
	if (++entryIteration < entryCount) {
		return &entries[entryIteration].Header;
	}
	return NULL;
}

DatFileHeader *DatFile::Header(uint32_t index) {
	if (index < entryCount) {
		return &entries[index].Header;
	}
	return NULL;
}

bool DatFile::Entry(std::vector<char> &inputBuffer) {
	if (entryIteration < entryCount) {
		InternalEntry(inputBuffer, entryIteration);
		return true;
	}
	return false;
}

bool DatFile::Entry(uint16_t index, std::vector<char> &inputBuffer) {
	if (index < entryCount) {
		InternalEntry(inputBuffer, entryIteration);
		return true;
	}
	return false;
}

uint32_t DatFile::Size() {
	return fileBuffer.size();
}

void DatFile::Buffer(std::vector<char> &inputBuffer) {
	std::vector<char> copyBuffer(fileBuffer);
	copyBuffer.swap(inputBuffer);
}

