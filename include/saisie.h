/*
 * saisie.h -- Lecture sécurisée au clavier
 */
#ifndef SAISIE_H
#define SAISIE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Affiche `invite` puis lit une ligne avec fgets (plus sûr que scanf).
 * Retourne false en fin de fichier (Ctrl+D). */
bool lire_ligne(const char *invite, char *tampon, size_t taille);

/* Lit un entier non signé (décimal, ou hexadécimal avec 0x).
 * Redemande tant que la saisie est invalide ; retourne false en fin de fichier. */
bool lire_entier(const char *invite, uint64_t *valeur);

#endif /* SAISIE_H */
