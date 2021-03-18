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
#ifdef LOGGING
	uint8_t frame[17];
	uint8_t frameLength = 0;
	uint32_t frameOffset = 0;
	uint32_t originalOffset = 0;
#endif
	while (outputOffset < uncompressedSize) {
		if (!compressionBitCount) {
			BinaryIO::CheckSpace(inputBuffer, inputOffset, 1);
			compressionBits = BinaryIO::ReadUInt8(inputBuffer, inputOffset);
			compressionBitCount = 8;
#ifdef LOGGING
			if (inputOffset) {
				printf("    ");
				for (unsigned int i = 0; i < frameLength; i++) {
					printf("%02x-", (int)(frame[i]));
				}
				printf("  ");
				for (unsigned int i = 1; i < frameLength; i++) {
					if (frame[i] < 32) {
						printf("_");
					}
					else if (frame[i] > 126) {
						printf("_");
					}
					else {
						printf("%c", (frame[i]));
					}
				}
				printf("\n");
			}
			frameLength = 0;
			frameOffset = inputOffset;
			frame[frameLength++] = compressionBits;
#endif
		}
		bool uncompressed = compressionBits & 0x01;
		compressionBitCount--;
		compressionBits >>= 1;
		if (uncompressed) {
			BinaryIO::CheckSpace(inputBuffer, inputOffset, 1);
#ifdef LOGGING
			frame[frameLength++] = inputBuffer[inputOffset];
			printf("  %08x  %08x\n", inputOffset, outputOffset);
#endif
			outputBuffer[outputOffset++] = inputBuffer[inputOffset++];
		}
		else {
			BinaryIO::CheckSpace(inputBuffer, inputOffset, 2);
			uint8_t byte1 = BinaryIO::ReadUInt8(inputBuffer, inputOffset);
			uint8_t byte2 = BinaryIO::ReadUInt8(inputBuffer, inputOffset);
			uint8_t length = (byte2 & 0x0f) + 3;
			if ((outputOffset + length) > uncompressedSize) {
				throw "Compression overflow";
			}
			int32_t offset = ((byte2 >> 4) << 8) + byte1;
#ifdef LOGGING
			originalOffset = offset;
			frame[frameLength++] = byte1;
			frame[frameLength++] = byte2;
#endif
			offset += 0x1012;
			offset += (outputOffset & 0xfffff000);
			while (offset >= (int32_t)outputOffset)
				offset -= 0x1000;
#ifdef LOGGING
			printf("  %08x  %08x  %02d  %08x  %08x  ", inputOffset - 2, outputOffset, (int)length, (int)originalOffset, (int)offset);
#endif
			while (length--) {
				if (offset < 0) {
#ifdef LOGGING
					printf("20-");
#endif
					outputBuffer[outputOffset++] = ' ';
				}
				else {
#ifdef LOGGING
					printf("%02x-", (int)(outputBuffer[offset]));
#endif
					outputBuffer[outputOffset++] = outputBuffer[offset];
				}
				offset++;
			}
#ifdef LOGGING
			printf("\n");
#endif
		}
	}
#ifdef LOGGING
	printf("    ");
	for (unsigned int i = 0; i < frameLength; i++) {
		printf("%02x-", (int)(frame[i]));
	}
	printf("  ");
	for (unsigned int i = 1; i < frameLength; i++) {
		if (frame[i] < 32) {
			printf("_");
		}
		else if (frame[i] > 126) {
			printf("_");
		}
		else {
			printf("%c", (frame[i]));
		}
	}
	printf("\n");
#endif
	outputBuffer.swap(inputBuffer);
}

void DatFile::Compress(std::vector<char> &originalBuffer) {
	uint32_t inputLength = originalBuffer.size();
	std::vector<char> inputBuffer(inputLength + 18);
	std::vector<char> workingBuffer(inputLength);
	for (unsigned int i = 0; i < 18; i++) {
		inputBuffer[i] = ' ';
	}
	memcpy(inputBuffer.data() + 18, originalBuffer.data(), inputLength);
	char frame[17];
	int frameLength = 1;
	int bitCount = 0;
	uint32_t inputOffset = 18;
	inputLength += 18;
	uint32_t outputOffset = 0;
	printf("inputLength: %d\n", inputLength);
	while (inputOffset < inputLength) {
		// Search for matching sequence
		uint32_t searchStart = 0;
		uint32_t searchEnd = inputOffset - 1;
		uint32_t searchLength = 18;
		if (inputBuffer[inputOffset] != ' ') {
			searchStart = 18;
		}
		if ((inputOffset - searchStart) > 4096) {
			searchStart = inputOffset - 4096;
		}
		if ((searchEnd + searchLength + 1) > inputLength) {
			searchLength = inputLength - searchEnd - 1;
		}
		printf("%d %d %d\n", searchStart, searchEnd, searchLength);
		uint8_t foundLength = 2;
		uint32_t foundOffset = 0;
		while (searchStart < searchEnd) {
			uint8_t thisLength = 0;
			while (thisLength < searchLength) {
				if (inputBuffer[searchStart + thisLength] != inputBuffer[inputOffset + thisLength])
					break;
				thisLength++;
			}
			if (thisLength > foundLength) {
				foundLength = thisLength;
				foundOffset = searchStart;
			}
			searchStart++;
		}
		frame[0] >>= 1;
		frame[0] &= 0x7f;
		if (foundLength > 2) {
			inputOffset += foundLength;
			foundLength -= 3;
			foundOffset -= 36;
			foundOffset &= 0xfff;
			uint8_t byte1 = foundOffset & 0xff;
			uint8_t byte2 = ((foundOffset >> 8) << 4) + foundLength;
			frame[frameLength++] = byte1;
			frame[frameLength++] = byte2;
		}
		else {
			frame[0] |= 0x80;
			frame[frameLength++] = inputBuffer[inputOffset++];
		}
		bitCount++;
		if (bitCount == 8) {
			memcpy(workingBuffer.data() + outputOffset, frame, frameLength);
			outputOffset += frameLength;
			bitCount = 0;
			frameLength = 1;
		}
	}
	printf("End\n");
	if (bitCount > 0) {
		while (bitCount < 8) {
			frame[0] >>= 1;
			bitCount++;
		}
		memcpy(workingBuffer.data() + outputOffset, frame, frameLength);
		outputOffset += frameLength;
	}
	std::vector<char> outputBuffer(outputOffset);
	memcpy(outputBuffer.data(), workingBuffer.data(), outputOffset);
	originalBuffer.swap(outputBuffer);
	return;
}

DatFile::DatFile(std::vector<char> &inputBuffer) {
	fileBuffer.clear();
	fileBuffer.swap(inputBuffer);
	uint32_t bufferOffset = 0;
	BinaryIO::CheckSpace(fileBuffer, bufferOffset, 2);
	entryCount = BinaryIO::ReadUInt16(fileBuffer, bufferOffset);
	entries.resize(entryCount);
	for (unsigned int i = 0; i < entryCount; i++) {
		BinaryIO::CheckSpace(fileBuffer, bufferOffset, sizeof(DatFileHeader));
		strncpy_s(entries[i].Header.FileName, 13, &(fileBuffer[bufferOffset]), 13);
		entries[i].Header.FileName[12] = '\0';
		bufferOffset += 13;
		entries[i].Header.UncompressedSize = BinaryIO::ReadUInt32(fileBuffer, bufferOffset);
		entries[i].Header.CompressedSize = BinaryIO::ReadUInt32(fileBuffer, bufferOffset);
		entries[i].Header.IsNotCompressed = BinaryIO::ReadUInt8(fileBuffer, bufferOffset);
		entries[i].CompressedBufferOffset = bufferOffset;
		BinaryIO::CheckSpace(fileBuffer, bufferOffset, entries[i].Header.CompressedSize);
		bufferOffset += entries[i].Header.CompressedSize;
	}
}

DatFile::DatFile() {
	fileBuffer.resize(2);
	uint32_t bufferOffset = 0;
	BinaryIO::WriteUInt16(fileBuffer, bufferOffset, entryCount);
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
	BinaryIO::WriteUInt16(fileBuffer, bufferOffset, ++entryCount);
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

