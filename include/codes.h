/*
 * codes.h -- Codes BCD et Gray
 */
#ifndef CODES_H
#define CODES_H

#include <stdbool.h>
#include <stdint.h>

/* Convertit n (0 à 99 999 999) en BCD compacté : un quartet par chiffre décimal.
 * Exemple : 171 -> 0x171 = 0001 0111 0001. Retourne false si n est trop grand. */
bool bcd_coder(uint32_t n, uint32_t *bcd);

/* Opération inverse. Retourne false si un quartet est > 9 (invalide en BCD). */
bool bcd_decoder(uint32_t bcd, uint32_t *n);

/* Binaire naturel -> code Gray (un seul bit change entre deux nombres consécutifs). */
uint32_t gray_coder(uint32_t b);

/* Code Gray -> binaire naturel. */
uint32_t gray_decoder(uint32_t g);

#endif /* CODES_H */
