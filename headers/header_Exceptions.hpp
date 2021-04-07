#pragma once

extern int ErrorCode;

namespace Exceptions
{
	void ERROR(int errorCode);
	void HANDLE();

	enum Error
#include "../src/errorcodes.txt"

}