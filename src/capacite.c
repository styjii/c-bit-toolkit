/*
 * capacite.c -- Capacité de codage et table du binaire naturel (voir capacite.h)
 */
#include "capacite.h"

#include <stdio.h>

#include "bits.h"

/* bits_necessaires : plus petit entier n tel que 2^n >= M, c'est-à-dire le
 * nombre de bits pour coder M valeurs distinctes (n = ceil(log2 M)).
 * Exemples : M = 100 -> 7 ; M = 256 -> 8 ; M = 257 -> 9 ; M = 1 -> 0. */
unsigned bits_necessaires(uint64_t M)
{
    unsigned n = 0;
    while (n < 64 && ((UINT64_C(1) << n) < M)) {
        n++;
    }
    return n;
}

/* afficher_table_bn : table des entiers 0 à 2^n - 1 en binaire naturel sur
 * n bits (1 à 8), avec les colonnes b(n-1) ... b0 (voir exercice 4 du cours). */
void afficher_table_bn(unsigned n)
{
    if (n < 1 || n > 8) {
        printf("Nombre de bits non supporte (1 a 8).\n");
        return;
    }
    printf("\n  N  |");
    for (unsigned i = n; i-- > 0;) {
        printf(" b%u", i);
    }
    printf("\n-----+");
    for (unsigned i = 0; i < n; i++) {
        printf("----");
    }
    printf("\n");
    for (uint32_t v = 0; v < (UINT32_C(1) << n); v++) {
        printf(" %3u |", (unsigned)v);
        for (unsigned i = n; i-- > 0;) {
            printf("  %u", bit_lire(v, i));
        }
        printf("\n");
    }
}
