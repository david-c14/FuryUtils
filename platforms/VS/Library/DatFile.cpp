#include "../../../classes/header_BinaryIO.hpp"
#define APIEXPORT __declspec(dllexport)
#include "../../../classes/class_DatFile.cpp"

extern "C" {
	APIEXPORT DatFile * __cdecl DatFile_createNew() {
		return new DatFile();
	}

	APIEXPORT DatFile * __cdecl DatFile_create(char *buffer, int size) {
		std::vector<char> vBuffer(buffer, buffer + size);
		return new DatFile(vBuffer);
	}

	APIEXPORT void __cdecl DatFile_destroy(DatFile *datFile) {
		delete datFile;
	}

	APIEXPORT int __cdecl DatFile_entryCount(DatFile *datFile) {
		return datFile->EntryCount();
	}

	APIEXPORT void __cdecl DatFile_reset(DatFile *datFile) {
		datFile->Reset();
	}

	APIEXPORT bool __cdecl DatFile_next(DatFile *datFile, DatFileHeader *header) {
		DatFileHeader *next = datFile->Next();
		if (next) {
			memcpy(header, next, sizeof(DatFileHeader));
			return true;
		}
		return false;
	}

	APIEXPORT bool __cdecl DatFile_header(DatFile *datFile, uint32_t index, DatFileHeader *header) {
		DatFileHeader *item = datFile->Header(index);
		if (item) {
			memcpy(header, item, sizeof(DatFileHeader));
			return true;
		}
		return false;
	}

	APIEXPORT bool __cdecl DatFile_entry(DatFile *datFile, uint32_t index, char *buffer, uint32_t size) {
		DatFileHeader *item = datFile->Header(index);
		if (item->UncompressedSize > size) {
			return false;
		}
		std::vector<char> internal_buffer(item->UncompressedSize);
		datFile->Entry(index, internal_buffer);
		memcpy(buffer, internal_buffer.data(), item->UncompressedSize);
		return true;
	}

	APIEXPORT void __cdecl DatFile_add(DatFile *datFile, char *fileName, char *buffer, uint32_t size, bool compress) {
		std::vector<char> internal_buffer(size);
		memcpy(internal_buffer.data(), buffer, size);
		datFile->Add(fileName, internal_buffer, compress);
	}

	APIEXPORT uint32_t __cdecl DatFile_size(DatFile *datFile) {
		return datFile->Size();
	}

	APIEXPORT bool __cdecl DatFile_buffer(DatFile *datFile, char *buffer, uint32_t size) {
		std::vector<char> internal_buffer;
		datFile->Buffer(internal_buffer);
		if (internal_buffer.size() > size) {
			return false;
		}
		memcpy(buffer, internal_buffer.data(), internal_buffer.size());
		return true;
	}
}
