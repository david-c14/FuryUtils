#pragma once

#ifndef APIENTRY
#define APIENTRY
#endif

extern "C" {
	APIENTRY int __cdecl GetExceptionCode();

	APIENTRY const char * __cdecl GetExceptionString();
}

