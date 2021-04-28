#define APIENTRY __declspec(dllexport)
#include "../../../src/Exceptions.cpp"

#include "ExceptionsExport.h"

extern "C" {
	int __cdecl GetExceptionCode() {
		return ErrorCode;
	}

	const char * __cdecl GetExceptionString() {
		return ErrorString.c_str();
	}
}

