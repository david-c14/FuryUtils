#pragma once

// This ugly piece of code means that the enum is defined once in c# and then the same list of values is used here

#define public
#define carbon14 Exceptions
#define FuryUtils Codes
#include "../platforms/VS/carbon14.FuryUtils/ErrorCodes.cs"
#undef FuryUtils
#undef carbon14
#undef public

namespace Exceptions
{
	void ERROR(int errorCode);
	void HANDLE();
}

extern int ErrorCode;