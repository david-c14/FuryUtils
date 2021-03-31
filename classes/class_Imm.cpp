#include "../headers/header_Imm.hpp"

Imm::Imm(std::vector<uint8_t> &inputPalette, std::vector<uint8_t> &inputPixels) {
	uint32_t inputSize = inputPixels.size();
	if (inputSize < 9) {
		Exceptions::ERROR(Exceptions::Codes::INVALID_FORMAT);
	}
	if (inputPalette.size() != 768) {
		Exceptions::ERROR(Exceptions::Codes::BUFFER_OVERFLOW);
	}
	uint8_t *inputArray = inputPixels.data();
	if (*inputArray++ != 'L') {
		Exceptions::ERROR(Exceptions::Codes::INVALID_FORMAT);
	}
	if (*inputArray++ != 'I') {
		Exceptions::ERROR(Exceptions::Codes::INVALID_FORMAT);
	}
	if (*inputArray++ != 'B') {
		Exceptions::ERROR(Exceptions::Codes::INVALID_FORMAT);
	}
	if (*inputArray++ != 'N') {
		Exceptions::ERROR(Exceptions::Codes::INVALID_FORMAT);
	}
	_width = *inputArray++;
	_width += 256 * (*inputArray++);
	_height = *inputArray++;
	_height += 256 * (*inputArray++);
	inputArray++;
	std::vector<uint8_t> pixelVector(_width * _height);
	if ((pixelVector.size() + 9) != inputSize) {
		Exceptions::ERROR(Exceptions::Codes::BUFFER_OVERFLOW);
	}
	memcpy(pixelVector.data(), inputArray, pixelVector.size());
	std::vector<RGBTriple> paletteVector(256);
	uint8_t *paletteArray = (uint8_t *)(paletteVector.data());
	memcpy(paletteArray, inputPalette.data(), 768);
	for (unsigned int i = 0; i < 768; i++) {
		*paletteArray = (*paletteArray) << 2;
		paletteArray++;
	}
	_depth = 8;
	_palette.swap(paletteVector);
	_pixels.swap(pixelVector);
}

uint32_t Imm::Size() {
	return _outputBuffer.size();
}

void Imm::Buffer(std::vector<uint8_t> &inputBuffer) {
	if (Size()) {
		std::vector<uint8_t> outputBuffer(_outputBuffer);
		inputBuffer.swap(outputBuffer);
		return;
	}
	Exceptions::ERROR(Exceptions::Codes::NOT_IMPLEMENTED);
}