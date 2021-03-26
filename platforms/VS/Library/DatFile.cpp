#include "../../../classes/header_BinaryIO.hpp"
#define APIEXPORT __declspec(dllexport)
#include "../../../classes/class_DatFile.cpp"

extern "C" {
	APIEXPORT DatFile * __cdecl DatFile_createNew() {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			return new DatFile();
		}
		catch (...) {
			Exceptions::HANDLE();
			return NULL;
		}
	}

	APIEXPORT DatFile * __cdecl DatFile_create(char *buffer, int size) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			std::vector<char> vBuffer(buffer, buffer + size);
			return new DatFile(vBuffer);
		}
		catch (...) {
			Exceptions::HANDLE();
			return NULL;
		}
	}

	APIEXPORT void __cdecl DatFile_destroy(DatFile *datFile) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			delete datFile;
		}
		catch (...) {
			Exceptions::HANDLE();
		}
	}

	APIEXPORT int __cdecl DatFile_entryCount(DatFile *datFile) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			return datFile->EntryCount();
		}
		catch (...) {
			Exceptions::HANDLE();
			return 0;
		}
	}

	APIEXPORT void __cdecl DatFile_reset(DatFile *datFile) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			datFile->Reset();
		}
		catch (...) {
			Exceptions::HANDLE();
		}
	}

	APIEXPORT bool __cdecl DatFile_next(DatFile *datFile, DatFileHeader *header) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			DatFileHeader *next = datFile->Next();
			if (next) {
				memcpy(header, next, sizeof(DatFileHeader));
				return true;
			}
		}
		catch (...) {
			Exceptions::HANDLE();
		}
		return false;
	}

	APIEXPORT bool __cdecl DatFile_header(DatFile *datFile, uint32_t index, DatFileHeader *header) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			DatFileHeader *item = datFile->Header(index);
			if (item) {
				memcpy(header, item, sizeof(DatFileHeader));
				return true;
			}
		}
		catch (...) {
			Exceptions::HANDLE();
		}
		return false;
	}

	APIEXPORT bool __cdecl DatFile_entry(DatFile *datFile, uint32_t index, char *buffer, uint32_t size) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			DatFileHeader *item = datFile->Header(index);
			if (item->UncompressedSize > size) {
				ErrorCode = Exceptions::Codes::BUFFER_OVERFLOW;
				return false;
			}
			std::vector<char> internal_buffer(item->UncompressedSize);
			datFile->Entry(index, internal_buffer);
			memcpy(buffer, internal_buffer.data(), item->UncompressedSize);
		}
		catch (...) {
			Exceptions::HANDLE();
			return false;
		}
		return true;
	}

	APIEXPORT void __cdecl DatFile_add(DatFile *datFile, char *fileName, char *buffer, uint32_t size, bool compress) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			std::vector<char> internal_buffer(size);
			memcpy(internal_buffer.data(), buffer, size);
			datFile->Add(fileName, internal_buffer, compress);
		}
		catch (...) {
			Exceptions::HANDLE();
		}
	}

	APIEXPORT uint32_t __cdecl DatFile_size(DatFile *datFile) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			return datFile->Size();
		}
		catch (...) {
			Exceptions::HANDLE();
		}
		return 0;
	}

	APIEXPORT bool __cdecl DatFile_buffer(DatFile *datFile, char *buffer, uint32_t size) {
		ErrorCode = Exceptions::Codes::NO_ERROR;
		try {
			std::vector<char> internal_buffer;
			datFile->Buffer(internal_buffer);
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
