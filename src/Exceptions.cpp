#include "../headers/header_Exceptions.hpp"

int ErrorCode = Exceptions::NO_ERROR;

namespace Exceptions {
	
	void ERROR(int error) {
		ErrorCode = error;
		throw error;
	}

	void HANDLE() {
		if (ErrorCode == Exceptions::NO_ERROR) {
			ErrorCode = Exceptions::UNKNOWN_ERROR;
		}
	}
}