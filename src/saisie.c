/*
 * saisie.c -- Lecture sécurisée au clavier (voir saisie.h)
 */
#include "saisie.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* lire_ligne : lit une ligne au clavier avec fgets (plus sûr que scanf) après
 * avoir affiché `invite`. Retourne false en fin de fichier (Ctrl+D). */
bool lire_ligne(const char *invite, char *tampon, size_t taille)
{
    if (tampon == NULL || taille == 0 || taille > (size_t)INT_MAX) {
        return false; /* fgets attend une taille de type int, strictement positive */
    }
    printf("%s", invite);
    fflush(stdout);
    if (fgets(tampon, (int)taille, stdin) == NULL) {
        return false;
    }
    size_t fin = strcspn(tampon, "\r\n"); /* position du retour à la ligne */
    if (fin < taille) {                    /* toujours vrai : garde explicite */
        tampon[fin] = '\0';
    }
    return true;
}

/* lire_entier : lit un entier non signé (décimal, ou hexadécimal avec 0x).
 * Redemande tant que la saisie est invalide. */
bool lire_entier(const char *invite, uint64_t *valeur)
{
    char ligne[64];
    for (;;) {
        if (!lire_ligne(invite, ligne, sizeof ligne)) {
            return false;
        }
        char *fin;
        unsigned long long v = strtoull(ligne, &fin, 0);
        if (ligne[0] != '\0' && *fin == '\0' && ligne[0] != '-') {
            *valeur = v;
            return true;
        }
        printf("Saisie invalide, recommencez.\n");
    }
}
