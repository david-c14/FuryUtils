#pragma once
#include <vector>
#include "header_BinaryIO.hpp"
#include "header_Exceptions.hpp"

#pragma pack(push, 1)
struct RGBTriple {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};
#pragma pack(pop)

struct Imm {

protected: 
	std::vector<RGBTriple> _palette;
	std::vector<uint8_t> _pixels;
	std::vector<uint8_t> _outputBuffer;
	uint16_t _width;
	uint16_t _height;
	uint16_t _depth;
	Imm();

public:
	Imm(std::vector<uint8_t> &inputPalette, std::vector<uint8_t> &inputPixels);
	uint32_t Size();
	void Buffer(std::vector<uint8_t> &inputBuffer);
	uint32_t ImmSize();
	void ImmBuffer(std::vector<uint8_t> &inputBuffer);
	uint32_t PamSize();
	void PamBuffer(std::vector<uint8_t> &inputBuffer);

};