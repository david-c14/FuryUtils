#pragma once

#ifndef APIENTRY
#define APIENTRY
#endif

#ifndef IMM_P
typedef void* imm_p;
#endif

extern "C" {
	APIENTRY uint32_t __cdecl Imm_size(imm_p imm);

	APIENTRY uint8_t __cdecl Imm_buffer(imm_p imm, uint8_t *buffer, uint32_t size);

	APIENTRY uint32_t __cdecl Imm_pixelSize(imm_p imm);

	APIENTRY uint8_t __cdecl Imm_pixelBuffer(imm_p imm, uint8_t *buffer, uint32_t size);

	APIENTRY uint32_t __cdecl Imm_paletteSize(imm_p imm);

	APIENTRY uint8_t __cdecl Imm_paletteBuffer(imm_p imm, uint8_t *buffer, uint32_t size);
}