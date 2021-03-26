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
			if (inputOffset > 1) {
				printf("    ");
				for (unsigned int i = 0; i < frameLength; i++) {
					printf("%02hhx-", frame[i]);
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
			printf("  %08x  %08x                          %02hhx\n", inputOffset, outputOffset, inputBuffer[inputOffset]);
#endif
			outputBuffer[outputOffset++] = inputBuffer[inputOffset++];
		}
		else {
			BinaryIO::CheckSpace(inputBuffer, inputOffset, 2);
			uint8_t byte1 = BinaryIO::ReadUInt8(inputBuffer, inputOffset);
			uint8_t byte2 = BinaryIO::ReadUInt8(inputBuffer, inputOffset);
			uint8_t length = (byte2 & 0x0f) + 3;
			if ((outputOffset + length) > uncompressedSize) {
				Exceptions::ERROR(Exceptions::Codes::BUFFER_OVERFLOW);
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
			printf("  %08x  %08x  %02d  %08x  %08x  ", inputOffset - 2, outputOffset, length, originalOffset, offset);
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
					printf("%02hhx-", outputBuffer[offset]);
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
		printf("%02hhx-", frame[i]);
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
			printf("%c", frame[i]);
		}
	}
	printf("\n");
#endif
	outputBuffer.swap(inputBuffer);
}

struct Frame {
	uint8_t frame[17];
	uint8_t frameLength = 1;
	uint8_t bitCount = 0;
	char *workingArray;
	uint32_t *outputOffset;
	uint32_t originalLength;
public:

	Frame(char *workingArray, uint32_t *outputOffset, uint32_t originalLength) {
		this->workingArray = workingArray;
		this->outputOffset = outputOffset;
		this->originalLength = originalLength;
	}

	uint8_t Length() {
		return frameLength;
	}

	bool Write() {
		bitCount++;
		if (bitCount < 8) {
			return true;
		}
		if ((*outputOffset + frameLength) > originalLength) {
			return false;
		}
#ifdef LOGGING
		printf("    ");
		for (unsigned int i = 0; i < frameLength; i++) {
			printf("%02hhx-", frame[i]);
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
		memcpy(workingArray + *outputOffset, frame, frameLength);
		*outputOffset += frameLength;
		bitCount = 0;
		frameLength = 1;
		return true;
	}

	bool Add(char c) {
		frame[0] >>= 1;
		frame[0] |= 0x80;
		frame[frameLength++] = c;
		return Write();
	}

	bool Add(uint32_t offset, uint32_t length) {
		frame[0] >>= 1;
		frame[0] &= 0x7f;
		uint8_t byte1 = offset & 0xff;
		uint8_t byte2 = ((offset >> 8) << 4) + length;
		frame[frameLength++] = byte1;
		frame[frameLength++] = byte2;
		return Write();
	}

	bool Close() {
		if (bitCount == 0) {
			return true;
		}
		while (bitCount < 8) {
			frame[0] >>= 1;
			bitCount++;
		}
		return Write();
	}
};


void DatFile::Compress(std::vector<char> &originalBuffer) {
	uint32_t originalLength = (uint32_t)(originalBuffer.size());
	uint32_t inputLength = originalLength + 18;
	std::vector<char> inputBuffer(inputLength);
	std::vector<char> workingBuffer(originalLength);
	char *inputArray = inputBuffer.data();
	char *workingArray = workingBuffer.data();
	for (unsigned int i = 0; i < 18; i++) {
		inputArray[i] = ' ';
	}
	memcpy(inputArray + 18, originalBuffer.data(), originalLength);
	uint8_t frame[17];
	uint8_t frameLength = 1;
	uint8_t bitCount = 0;
	uint32_t inputOffset = 18;
	uint32_t outputOffset = 0;
	while (inputOffset < inputLength) {
		// Search for matching sequence
		uint32_t searchStart = 0;
		uint32_t searchEnd = inputOffset;
		uint32_t searchLength = 18;
		if (inputArray[inputOffset] != ' ') {
			searchStart = 18;
		}
		if ((inputOffset - searchStart) > 4096) {
			searchStart = inputOffset - 4096;
		}
		if ((searchEnd + searchLength) > inputLength) {
			searchLength = inputLength - searchEnd;
		}
		uint8_t foundLength = 2;
		uint32_t foundOffset = 0;
		while (searchStart < searchEnd) {
			searchEnd--;
			uint8_t thisLength = 0;
			while (thisLength < searchLength) {
				if (inputArray[searchEnd + thisLength] != inputArray[inputOffset + thisLength])
					break;
				thisLength++;
			}
			if (thisLength > foundLength) {
				foundLength = thisLength;
				foundOffset = searchEnd;
			}
		}
		frame[0] >>= 1;
		frame[0] &= 0x7f;
		if (foundLength > 2) {
#ifdef LOGGING
			printf("  %08x  %08x  %02d  %08x  %08x  ", outputOffset + frameLength, inputOffset - 18, foundLength, (foundOffset - 36) & 0xfff, (foundOffset - 18));
			for (unsigned int i = 0; i < foundLength; i++) {
				printf("%02hhx-", inputArray[foundOffset + i]);
			}
			printf("\n");
#endif
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
#ifdef LOGGING
			printf("  %08x  %08x                          %02hhx\n", outputOffset + frameLength, inputOffset - 18, inputArray[inputOffset]);
#endif
			frame[0] |= 0x80;
			frame[frameLength++] = inputArray[inputOffset++];
		}
		bitCount++;
		if (bitCount == 8) {
			if ((outputOffset + frameLength) > originalLength) {
				return;
			}
#ifdef LOGGING
			printf("    ");
			for (unsigned int i = 0; i < frameLength; i++) {
				printf("%02hhx-", frame[i]);
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
			memcpy(workingArray + outputOffset, frame, frameLength);
			outputOffset += frameLength;
			bitCount = 0;
			frameLength = 1;
		}
	}
	if (bitCount > 0) {
		if ((outputOffset + frameLength) > originalLength) {
			return;
		}
		while (bitCount < 8) {
			frame[0] >>= 1;
			bitCount++;
		}
#ifdef LOGGING
		printf("    ");
		for (unsigned int i = 0; i < frameLength; i++) {
			printf("%02hhx-", frame[i]);
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
				printf("%c", frame[i]);
			}
		}
		printf("\n");
#endif
		memcpy(workingArray + outputOffset, frame, frameLength);
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
	entry.Header.UncompressedSize = (uint32_t)(inputBuffer.size());
	if (compress) {
		Compress(inputBuffer);
	}
	entry.Header.CompressedSize = (uint32_t)(inputBuffer.size());
	entry.Header.IsNotCompressed = (entry.Header.CompressedSize >= entry.Header.UncompressedSize);

	uint32_t bufferOffset = 0;
	BinaryIO::WriteUInt16(fileBuffer, bufferOffset, ++entryCount);
	bufferOffset = (uint32_t)(fileBuffer.size());
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
	ErrorCode = Exceptions::Codes::INDEX_OUT_OF_RANGE;
	return NULL;
}

DatFileHeader *DatFile::Header(uint32_t index) {
	if (index < entryCount) {
		return &entries[index].Header;
	}
	ErrorCode = Exceptions::Codes::INDEX_OUT_OF_RANGE;
	return NULL;
}

bool DatFile::Entry(std::vector<char> &inputBuffer) {
	if (entryIteration < entryCount) {
		InternalEntry(inputBuffer, entryIteration);
		return true;
	}
	ErrorCode = Exceptions::Codes::INDEX_OUT_OF_RANGE;
	return false;
}

bool DatFile::Entry(uint16_t index, std::vector<char> &inputBuffer) {
	if (index < entryCount) {
		InternalEntry(inputBuffer, entryIteration);
		return true;
	}
	ErrorCode = Exceptions::Codes::INDEX_OUT_OF_RANGE;
	return false;
}

uint32_t DatFile::Size() {
	return (uint32_t)(fileBuffer.size());
}

void DatFile::Buffer(std::vector<char> &inputBuffer) {
	std::vector<char> copyBuffer(fileBuffer);
	copyBuffer.swap(inputBuffer);
}

