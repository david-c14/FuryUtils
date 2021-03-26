#include "header_Exceptions.hpp"

int ErrorCode = Exceptions::Codes::NO_ERROR;

namespace Exceptions {
	
	void ERROR(int error) {
		ErrorCode = error;
		throw error;
	}

	void HANDLE() {
		if (ErrorCode == Exceptions::Codes::NO_ERROR) {
			ErrorCode = Exceptions::Codes::UNKNOWN_ERROR;
		}
	}
}