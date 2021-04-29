#pragma once
#include <string>

extern thread_local int ErrorCode;
extern thread_local std::string ErrorString;

namespace Exceptions
{
	struct Exception {
		int _errorCode;
		std::string _errorString;
		Exception(int errorCode, std::string message);
	};

	void ERROR(int errorCode);
	void ERROR(int errorCode, std::string message);
	void HANDLE();

	const char ERROR_IO_READ_BEYOND_BUFFER[] = "Attempt to read beyond end of buffer";

	const char ERROR_IMM_SHORT_HEADER[] = "image buffer size is too short for valid Imm";
	const char ERROR_IMM_PALETTE_SIZE[] = "palette buffer is the wrong size";
	const char ERROR_IMM_HEADER_MAGIC[] = "First 4 characters of image buffer should be 'LIBN'";
	const char ERROR_IMM_SIZE_MISMATCH[] = "Image buffer size does not match declared size";
	const char ERROR_IMM_NOT_IMPLEMENTED[] = "Image format not implemented";

	const char ERROR_BMP_SHORT_HEADER[] = "Buffer is too short to contain a valid Bmp";
	const char ERROR_BMP_HEADER_MAGIC[] = "First 2 characters of buffer should be 'BM'";
	const char ERROR_BMP_SIZE_MISMATCH[] = "Buffer size does not agree with declared size in header";
	const char ERROR_BMP_INFO_SIZE_MISMATCH[] = "Buffer size is too small to hold info structure";
	const char ERROR_BMP_IMAGE_SIZE_MISMATCH[] = "Buffer size is too small to hold specified image";
	const char ERROR_BMP_UNSUPPORTED_VERSION[] = "Info header version is not supported";
	const char ERROR_BMP_UNSUPPORTED_DEPTH[] = "Only 16-color and 256-color Bmp are supported";
	const char ERROR_BMP_UNSUPPORTED_COMPRESSION[] = "Only uncompressed and RLE compressed Bmp are supported";
	const char ERROR_BMP_PALETTE_SIZE_MISMATCH[] = "Declared palette size is too large";
	const char ERROR_BMP_TOP_TO_BOTTOM_RLE[] = "Top to bottom RLE encoded Bmp are not valid";
	const char ERROR_BMP_COMPRESSION_ERROR[] = "Compressed data contains an error";

	const char ERROR_DAT_COMPRESSION_SIZE_MISMATCH[] = "Uncompressed data is larger than declared size";
	const char ERROR_DAT_BUFFER_TOO_SMALL[] = "Buffer too small";

	enum Error
#include "../src/errorcodes.txt"


}