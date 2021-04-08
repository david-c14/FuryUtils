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

	enum Error
#include "../src/errorcodes.txt"

}