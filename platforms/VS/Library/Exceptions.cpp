#include "../../../src/Exceptions.cpp"
#define APIEXPORT __declspec(dllexport)

extern "C" {
	APIEXPORT int __cdecl GetExceptionCode() {
		return ErrorCode;
	}

	APIEXPORT const char * __cdecl GetExceptionString() {
		return ErrorString.c_str();
	}
}

