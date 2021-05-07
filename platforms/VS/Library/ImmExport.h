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

	APIENTRY uint32_t __cdecl Imm_immSize(imm_p imm);

	APIENTRY uint8_t __cdecl Imm_immBuffer(imm_p imm, uint8_t *buffer, uint32_t size);

	APIENTRY uint32_t __cdecl Imm_pamSize(imm_p imm);

	APIENTRY uint8_t __cdecl Imm_pamBuffer(imm_p imm, uint8_t *buffer, uint32_t size);
}