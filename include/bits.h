/*
 * bits.h -- Opérations élémentaires sur les bits (32 bits)
 *
 * Le bit de rang i a pour poids 2^i. Le bit 0 est le bit de poids faible (LSB).
 */
#ifndef BITS_H
#define BITS_H

#include <stdbool.h>
#include <stdint.h>

/* Retourne la valeur (0 ou 1) du bit de rang i dans x. */
unsigned bit_lire(uint32_t x, unsigned i);

/* Force le bit de rang i à 1 (OU avec un masque). */
uint32_t bit_mettre_a_1(uint32_t x, unsigned i);

/* Force le bit de rang i à 0 (ET avec le masque inversé). */
uint32_t bit_mettre_a_0(uint32_t x, unsigned i);

/* Inverse le bit de rang i (OU exclusif avec le masque). */
uint32_t bit_inverser(uint32_t x, unsigned i);

/* Nombre de bits à 1 dans x (« poids de Hamming »). */
unsigned bit_compter(uint32_t x);

/* Vrai si x = 2^k (un seul bit à 1). */
bool est_puissance_de_2(uint32_t x);

/* Rotations de x sur 32 bits : les bits sortants réapparaissent de l'autre côté. */
uint32_t rotation_gauche(uint32_t x, unsigned n);
uint32_t rotation_droite(uint32_t x, unsigned n);

/* Écrit x sur `largeur` bits (1 à 32), du MSB au LSB, groupés par 4. */
void afficher_bits(uint32_t x, unsigned largeur);

#endif /* BITS_H */
