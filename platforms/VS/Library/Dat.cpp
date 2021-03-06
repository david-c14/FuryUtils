#define APIENTRY __declspec(dllexport)
#include "../../../headers/BinaryIO.hpp"
#include "../../../src/Dat.cpp"

#ifndef DAT_P
#define DAT_P
typedef Dat* dat_p;
#endif

#include "DatExport.h"

extern "C" {
	Dat * __cdecl Dat_createNew() {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			return new Dat();
		}
		catch (...) {
			Exceptions::HANDLE();
			return NULL;
		}
	}

	Dat * __cdecl Dat_create(uint8_t *buffer, uint32_t size) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			std::vector<uint8_t> vBuffer(buffer, buffer + size);
			return new Dat(vBuffer);
		}
		catch (...) {
			Exceptions::HANDLE();
			return NULL;
		}
	}

	void __cdecl Dat_destroy(Dat *dat) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			delete dat;
		}
		catch (...) {
			Exceptions::HANDLE();
		}
	}

	int __cdecl Dat_entryCount(Dat *dat) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			return dat->EntryCount();
		}
		catch (...) {
			Exceptions::HANDLE();
			return -1;
		}
	}

	void __cdecl Dat_reset(Dat *dat) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			dat->Reset();
		}
		catch (...) {
			Exceptions::HANDLE();
		}
	}

	uint8_t __cdecl Dat_next(Dat *dat, DatHeader *header) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
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

	uint8_t __cdecl Dat_header(Dat *dat, uint32_t index, DatHeader *header) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
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

	uint8_t __cdecl Dat_entry(Dat *dat, uint32_t index, uint8_t *buffer, uint32_t size) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			DatHeader *item = dat->Header(index);
			if (item->UncompressedSize > size) {
				ErrorCode = Exceptions::BUFFER_OVERFLOW;
				ErrorString = Exceptions::ERROR_DAT_BUFFER_TOO_SMALL;
				return false;
			}
			std::vector<uint8_t> internal_buffer(item->UncompressedSize);
			dat->Entry(index, internal_buffer);
			memcpy(buffer, internal_buffer.data(), item->UncompressedSize);
		}
		catch (...) {
			Exceptions::HANDLE();
			return false;
		}
		return true;
	}

	void __cdecl Dat_add(Dat *dat, char *fileName, uint8_t *buffer, uint32_t size, uint8_t compress) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			std::vector<uint8_t> internal_buffer(size);
			memcpy(internal_buffer.data(), buffer, size);
			dat->Add(fileName, internal_buffer, compress);
		}
		catch (...) {
			Exceptions::HANDLE();
		}
	}

	uint32_t __cdecl Dat_size(Dat *dat) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			return dat->Size();
		}
		catch (...) {
			Exceptions::HANDLE();
		}
		return 0;
	}

	uint8_t __cdecl Dat_buffer(Dat *dat, uint8_t *buffer, uint32_t size) {
		ErrorCode = Exceptions::NO_ERROR;
		ErrorString = "";
		try {
			std::vector<uint8_t> internal_buffer;
			dat->Buffer(internal_buffer);
			if (internal_buffer.size() > size) {
				ErrorCode = Exceptions::BUFFER_OVERFLOW;
				ErrorString = Exceptions::ERROR_DAT_BUFFER_TOO_SMALL;
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
