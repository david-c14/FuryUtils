#pragma once
#include <vector>
#include "Exceptions.hpp"
#include "Imm.hpp"

#ifndef APIENTRY
#define APIENTRY
#endif

#pragma pack(push, 1)
struct RGBAQuad {
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
};
#pragma pack(pop)

struct APIENTRY Bmp : Imm {

private:
	void MakeBmp();

public:

	Bmp(std::vector<uint8_t> &inputPalette, std::vector<uint8_t> &inputPixels);
	Bmp(std::vector<uint8_t> &bmpBuffer);
};