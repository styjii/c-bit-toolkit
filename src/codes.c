/*
 * codes.c -- Codes BCD et Gray (voir codes.h)
 */
#include "codes.h"

/* bcd_coder : convertit n (0 à 99 999 999) en BCD « compacté » : chaque
 * chiffre DÉCIMAL occupe un groupe de 4 bits (un quartet).
 * Exemple : 171 -> 0x171 = 0001 0111 0001.
 * Principe : on extrait les chiffres décimaux par divisions par 10 et on place
 * chacun dans un quartet (décalage de 4 bits à chaque chiffre). */
bool bcd_coder(uint32_t n, uint32_t *bcd)
{
    uint32_t resultat = 0;
    unsigned decalage = 0;

    if (n > 99999999u) {
        return false; /* 8 chiffres BCD maximum dans 32 bits */
    }
    do {
        resultat |= (n % 10u) << decalage;
        n /= 10u;
        decalage += 4;
    } while (n != 0);
    *bcd = resultat;
    return true;
}

/* bcd_decoder : opération inverse. Refuse tout quartet > 9, car les
 * combinaisons 1010 à 1111 n'existent pas en BCD. */
bool bcd_decoder(uint32_t bcd, uint32_t *n)
{
    uint32_t resultat = 0;
    uint32_t poids = 1;

    do {
        uint32_t quartet = bcd & 0xFu;
        if (quartet > 9u) {
            return false; /* quartet invalide */
        }
        resultat += quartet * poids;
        poids *= 10u;
        bcd >>= 4u;
    } while (bcd != 0);
    *n = resultat;
    return true;
}

/* gray_coder : binaire naturel -> code Gray (un seul bit change entre deux
 * nombres consécutifs). Formule : g_i = b_(i+1) XOR b_i, g_MSB = b_MSB,
 * ce qui s'écrit simplement  g = b ^ (b >> 1). */
uint32_t gray_coder(uint32_t b)
{
    return b ^ (b >> 1u);
}

/* gray_decoder : code Gray -> binaire naturel. Chaque bit du résultat est le
 * XOR de tous les bits de poids supérieur ou égal du code Gray ; on les
 * accumule en XORant g avec ses versions décalées. */
uint32_t gray_decoder(uint32_t g)
{
    uint32_t b = 0;
    for (; g != 0; g >>= 1u) {
        b ^= g;
    }
    return b;
}
