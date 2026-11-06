/*
 * capacite.h -- Capacité de codage et table du binaire naturel
 */
#ifndef CAPACITE_H
#define CAPACITE_H

#include <stdint.h>

/* Plus petit entier n tel que 2^n >= M : nombre de bits pour coder M valeurs
 * distinctes (n = ceil(log2 M)). Exemples : M = 100 -> 7 ; M = 1 -> 0. */
unsigned bits_necessaires(uint64_t M);

/* Affiche la table des entiers 0 à 2^n - 1 en binaire naturel sur n bits
 * (n de 1 à 8), avec les colonnes b(n-1) ... b0. */
void afficher_table_bn(unsigned n);

#endif /* CAPACITE_H */
