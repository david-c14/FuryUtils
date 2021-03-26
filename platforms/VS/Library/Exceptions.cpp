#include "../../../classes/module_Exceptions.cpp"
#define APIEXPORT __declspec(dllexport)

extern "C" {
	APIEXPORT int __cdecl GetExceptionCode() {
		return ErrorCode;
	}
}

