#pragma once
#include <vector>
#include "header_Exceptions.hpp"
#include "header_Imm.hpp"

#ifndef APIEXPORT
#define APIEXPORT
#endif

#pragma pack(push, 1)
struct RGBAQuad {
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
};
#pragma pack(pop)

struct APIEXPORT Bmp : Imm {

private:
	void MakeBmp();

public:

	Bmp(std::vector<uint8_t> &inputPalette, std::vector<uint8_t> &inputPixels);
	Bmp(std::vector<uint8_t> &bmpBuffer);
};