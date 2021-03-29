#pragma once
#include <vector>

namespace BinaryIO {
	void CheckSpace(std::vector<char> &buffer, uint32_t bufferOffset, uint32_t required);

	uint8_t ReadUInt8(std::vector<char> &buffer, uint32_t &bufferOffset);

	uint16_t ReadUInt16(std::vector<char> &buffer, uint32_t &bufferOffset);

	uint32_t ReadUInt32(std::vector<char> &buffer, uint32_t &bufferOffset);

	void WriteUInt8(std::vector<char> &buffer, uint32_t &bufferOffset, uint8_t data);

	void WriteUInt16(std::vector<char> &buffer, uint32_t &bufferOffset, uint16_t data);

	void WriteUInt32(std::vector<char> &buffer, uint32_t &bufferOffset, uint32_t data);

}