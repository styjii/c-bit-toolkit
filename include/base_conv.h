/*
 * base_conv.h -- Conversions entre bases (2 à 36), entiers et fractionnaires
 */
#ifndef BASE_CONV_H
#define BASE_CONV_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Taille minimale d'un tampon recevant un entier 64 bits en base 2
 * (64 chiffres + 1 caractère de fin de chaîne). */
#define TAILLE_MIN_ENTIER 65

/* Convertit l'entier n en chaîne dans la base `base` (2 à 36).
 * Retourne true si la conversion a réussi (base valide, tampon assez grand). */
bool en_base(uint64_t n, unsigned base, char *sortie, size_t taille);

/* Convertit la chaîne s écrite en base `base` en entier (méthode de Horner).
 * Retourne false si un chiffre est interdit dans cette base ou en cas de
 * dépassement de 64 bits. */
bool depuis_base(const char *s, unsigned base, uint64_t *resultat);

/* Convertit x >= 0 en base `base`, avec au plus `max_chiffres` chiffres après
 * la virgule. */
bool reel_en_base(double x, unsigned base, unsigned max_chiffres,
                  char *sortie, size_t taille);

/* Convertit une chaîne comme « 110001.001 » (écrite en base `base`, séparateur
 * '.' ou ',') en nombre décimal. */
bool base_en_reel(const char *s, unsigned base, double *resultat);

#endif /* BASE_CONV_H */
