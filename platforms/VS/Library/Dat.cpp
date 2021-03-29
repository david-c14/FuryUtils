#include "../../../headers/header_BinaryIO.hpp"
#define APIEXPORT __declspec(dllexport)
#include "../../../classes/class_Dat.cpp"

extern "C" {
	APIEXPORT Dat * __cdecl Dat_createNew() {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			return new Dat();
		}
		catch (...) {
			Exceptions::HANDLE();
			return NULL;
		}
	}

	APIEXPORT Dat * __cdecl Dat_create(char *buffer, int size) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			std::vector<char> vBuffer(buffer, buffer + size);
			return new Dat(vBuffer);
		}
		catch (...) {
			Exceptions::HANDLE();
			return NULL;
		}
	}

	APIEXPORT void __cdecl Dat_destroy(Dat *dat) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			delete dat;
		}
		catch (...) {
			Exceptions::HANDLE();
		}
	}

	APIEXPORT int __cdecl Dat_entryCount(Dat *dat) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			return dat->EntryCount();
		}
		catch (...) {
			Exceptions::HANDLE();
			return 0;
		}
	}

	APIEXPORT void __cdecl Dat_reset(Dat *dat) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			dat->Reset();
		}
		catch (...) {
			Exceptions::HANDLE();
		}
	}

	APIEXPORT bool __cdecl Dat_next(Dat *dat, DatHeader *header) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			DatHeader *next = dat->Next();
			if (next) {
				memcpy(header, next, sizeof(DatHeader));
				return true;
			}
		}
		catch (...) {
			Exceptions::HANDLE();
		}
		return false;
	}

	APIEXPORT bool __cdecl Dat_header(Dat *dat, uint32_t index, DatHeader *header) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			DatHeader *item = dat->Header(index);
			if (item) {
				memcpy(header, item, sizeof(DatHeader));
				return true;
			}
		}
		catch (...) {
			Exceptions::HANDLE();
		}
		return false;
	}

	APIEXPORT bool __cdecl Dat_entry(Dat *dat, uint32_t index, char *buffer, uint32_t size) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			DatHeader *item = dat->Header(index);
			if (item->UncompressedSize > size) {
				ErrorCode = Exceptions::Codes::BUFFER_OVERFLOW;
				return false;
			}
			std::vector<char> internal_buffer(item->UncompressedSize);
			dat->Entry(index, internal_buffer);
			memcpy(buffer, internal_buffer.data(), item->UncompressedSize);
		}
		catch (...) {
			Exceptions::HANDLE();
			return false;
		}
		return true;
	}

	APIEXPORT void __cdecl Dat_add(Dat *dat, char *fileName, char *buffer, uint32_t size, bool compress) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			std::vector<char> internal_buffer(size);
			memcpy(internal_buffer.data(), buffer, size);
			dat->Add(fileName, internal_buffer, compress);
		}
		catch (...) {
			Exceptions::HANDLE();
		}
	}

	APIEXPORT uint32_t __cdecl Dat_size(Dat *dat) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			return dat->Size();
		}
		catch (...) {
			Exceptions::HANDLE();
		}
		return 0;
	}

	APIEXPORT bool __cdecl Dat_buffer(Dat *dat, char *buffer, uint32_t size) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			std::vector<char> internal_buffer;
			dat->Buffer(internal_buffer);
			if (internal_buffer.size() > size) {
				ErrorCode = Exceptions::Codes::BUFFER_OVERFLOW;
				return false;
			}
			memcpy(buffer, internal_buffer.data(), internal_buffer.size());
		}
		catch (...) {
			Exceptions::HANDLE();
			return false;
		}
		return true;
	}
}
