#ifndef __IMM_H__
#define __IMM_H__

typedef void* imm_p;

uint32_t Imm_size(imm_p imm);
uint8_t Imm_buffer(imm_p imm, uint8_t *buffer, uint32_t size);
uint32_t Imm_immSize(imm_p imm);
uint8_t Imm_immBuffer(imm_p imm, uint8_t *buffer, uint32_t size);
uint32_t Imm_pamSize(imm_p imm);
uint8_t Imm_pamBuffer(imm_p imm, uint8_t *buffer, uint32_t size);

#endif