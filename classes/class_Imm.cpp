#include "../headers/header_Imm.hpp"

Imm::Imm() {

}

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

uint32_t Imm::ImmSize() {
	return _width * _height + 9;
}

void Imm::ImmBuffer(std::vector<uint8_t> &inputBuffer) {
	std::vector<uint8_t> outputBuffer(_pixels.size() + 9);
	uint32_t offset = 0;
	// Fix when BinaryIO works with UInt8_t instead of char
	/*
	BinaryIO::WriteUInt8(outputBuffer, offset, 'L');
	BinaryIO::WriteUInt8(outputBuffer, offset, 'I');
	BinaryIO::WriteUInt8(outputBuffer, offset, 'B');
	BinaryIO::WriteUInt8(outputBuffer, offset, 'N');
	BinaryIO::WriteUInt16(outputBuffer, offset, _width);
	BinaryIO::WriteUInt16(outputBuffer, offset, _height);
	BinaryIO::WriteUInt8(outputBuffer, offset, 0);
	*/
	outputBuffer[0] = 'L';
	outputBuffer[1] = 'I';
	outputBuffer[2] = 'B';
	outputBuffer[3] = 'N';
	outputBuffer[4] = _width & 0xff;
	outputBuffer[5] = _width >> 8;;
	outputBuffer[6] = _height & 0xff;
	outputBuffer[7] = _height >> 8;
	outputBuffer[8] = ' ';
	memcpy(outputBuffer.data() + 9, _pixels.data(), _width * _height);
	inputBuffer.swap(outputBuffer);
	return;
}

uint32_t Imm::PamSize() {
	return (1 << _depth) * sizeof(RGBTriple);
}

void Imm::PamBuffer(std::vector<uint8_t> &inputBuffer) {
	std::vector<uint8_t> outputBuffer(_palette.size() * sizeof(RGBTriple));
	uint8_t *outputArray = outputBuffer.data();
	memcpy(outputArray, _palette.data(), _palette.size() * sizeof(RGBTriple));
	for (unsigned int i = 0; i < outputBuffer.size(); i++) {
		outputArray[i] = (outputArray[i] >> 2);
	}

	inputBuffer.swap(outputBuffer);
	return;
}

