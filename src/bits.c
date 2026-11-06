/*
 * bits.c -- Opérations élémentaires sur les bits (voir bits.h)
 */
#include "bits.h"

#include <stdio.h>

/* bit_lire : retourne la valeur (0 ou 1) du bit de rang i dans x.
 * Principe : on décale x de i positions vers la droite, le bit voulu arrive
 * en position 0, puis le masque 1 ne conserve que ce bit. */
unsigned bit_lire(uint32_t x, unsigned i)
{
    return (x >> i) & 1u;
}

/* bit_mettre_a_1 : force le bit de rang i à 1 (opération OU avec un masque).
 * Masque = 1 << i  : un seul 1, à la position i.  x | masque met ce bit à 1
 * sans toucher aux autres. */
uint32_t bit_mettre_a_1(uint32_t x, unsigned i)
{
    return x | (UINT32_C(1) << i);
}

/* bit_mettre_a_0 : force le bit de rang i à 0 (opération ET avec le masque
 * inversé). ~masque contient des 1 partout sauf en position i. */
uint32_t bit_mettre_a_0(uint32_t x, unsigned i)
{
    return x & ~(UINT32_C(1) << i);
}

/* bit_inverser : inverse le bit de rang i (OU exclusif avec le masque).
 * 0 XOR 1 = 1 et 1 XOR 1 = 0 : le bit change, les autres sont inchangés. */
uint32_t bit_inverser(uint32_t x, unsigned i)
{
    return x ^ (UINT32_C(1) << i);
}

/* bit_compter : nombre de bits à 1 dans x (« poids de Hamming »).
 * Méthode de Kernighan : x & (x - 1) supprime le bit à 1 de plus faible rang.
 * On compte combien de suppressions sont nécessaires pour arriver à 0. */
unsigned bit_compter(uint32_t x)
{
    unsigned n = 0;
    while (x != 0) {
        x &= x - 1;
        n++;
    }
    return n;
}

/* est_puissance_de_2 : vrai si x = 2^k (un seul bit à 1).
 * Un tel nombre vérifie x & (x - 1) == 0 ; il faut exclure x = 0. */
bool est_puissance_de_2(uint32_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

/* rotation_gauche : décale x de n positions vers la gauche sur 32 bits, les
 * bits sortant à gauche réapparaissent à droite (contrairement à <<). */
uint32_t rotation_gauche(uint32_t x, unsigned n)
{
    n %= 32;
    if (n == 0) {
        return x; /* évite un décalage de 32, qui est indéfini en C */
    }
    return (x << n) | (x >> (32 - n));
}

/* rotation_droite : même principe vers la droite. */
uint32_t rotation_droite(uint32_t x, unsigned n)
{
    n %= 32;
    if (n == 0) {
        return x;
    }
    return (x >> n) | (x << (32 - n));
}

/* afficher_bits : écrit x sur `largeur` bits (1 à 32), du MSB au LSB, en
 * regroupant les bits par 4 pour la lisibilité. Exemple : 0010 1011. */
void afficher_bits(uint32_t x, unsigned largeur)
{
    if (largeur < 1 || largeur > 32) {
        largeur = 32;
    }
    for (unsigned i = largeur; i > 0; i--) {
        unsigned rang = i - 1; /* rang du bit affiché : largeur - 1, ..., 0 */

        putchar(bit_lire(x, rang) ? '1' : '0');
        if (rang % 4 == 0 && rang != 0) {
            putchar(' ');
        }
    }
}
