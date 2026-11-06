/*
 * base_conv.c -- Conversions entre bases : entiers et nombres fractionnaires
 * (voir base_conv.h)
 */
#include "base_conv.h"

#include <ctype.h>
#include <string.h>

/* Chiffres utilisables pour écrire un nombre dans une base jusqu'à 36 :
 * 0-9 puis A-Z (A = 10, B = 11, ..., F = 15, ..., Z = 35). */
static const char CHIFFRES[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* ============================================================================
 *  CONVERSIONS ENTRE BASES (ENTIERS)
 * ============================================================================
 */

/* en_base : convertit l'entier n en chaîne dans la base `base` (2 à 36).
 * Méthode des DIVISIONS SUCCESSIVES : on divise par la base, le reste est le
 * chiffre suivant (en partant du poids faible), on continue avec le quotient.
 * Les restes sont obtenus de droite à gauche, donc on remplit le tampon à
 * l'envers ; c'est l'équivalent de « lire les restes de bas en haut ».
 * Retourne true si la conversion a réussi (base valide, tampon assez grand). */
bool en_base(uint64_t n, unsigned base, char *sortie, size_t taille)
{
    char tmp[TAILLE_MIN_ENTIER];
    size_t k = 0;

    if (base < 2 || base > 36 || sortie == NULL || taille == 0) {
        return false;
    }
    do {
        tmp[k++] = CHIFFRES[n % base]; /* reste = chiffre courant */
        n /= base;                     /* quotient = reste à convertir */
    } while (n != 0);

    if (k + 1 > taille) {
        return false; /* tampon de sortie trop petit */
    }
    for (size_t j = 0; j < k; j++) {
        sortie[j] = tmp[k - 1 - j]; /* on inverse l'ordre des restes */
    }
    sortie[k] = '\0';
    return true;
}

/* valeur_chiffre : valeur d'un caractère ('7' -> 7, 'F' ou 'f' -> 15), ou -1
 * si le caractère n'est pas un chiffre alphanumérique. */
static int valeur_chiffre(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    c = (char)toupper((unsigned char)c);
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    return -1;
}

/* depuis_base : convertit la chaîne s écrite en base `base` en entier.
 * Méthode de HORNER, équivalente à la forme polynomiale :
 *     valeur = valeur * base + chiffre     pour chaque chiffre, de gauche à droite.
 * Vérifie que chaque chiffre est < base (ex. « 12 » est refusé en base 2) et
 * détecte les dépassements de capacité. Retourne true si tout est valide. */
bool depuis_base(const char *s, unsigned base, uint64_t *resultat)
{
    uint64_t v = 0;

    if (s == NULL || *s == '\0' || base < 2 || base > 36) {
        return false;
    }
    for (; *s != '\0'; s++) {
        int d = valeur_chiffre(*s);
        if (d < 0 || (unsigned)d >= base) {
            return false; /* caractère interdit dans cette base */
        }
        if (v > (UINT64_MAX - (uint64_t)d) / base) {
            return false; /* dépassement de 64 bits */
        }
        v = v * base + (uint64_t)d;
    }
    *resultat = v;
    return true;
}

/* ============================================================================
 *  CONVERSIONS DES NOMBRES FRACTIONNAIRES
 * ============================================================================
 */

/* reel_en_base : convertit x >= 0 en base `base`, avec au plus `max_chiffres`
 * chiffres après la virgule.
 *  - partie entière      : divisions successives (fonction en_base) ;
 *  - partie fractionnaire : MULTIPLICATIONS SUCCESSIVES par la base ; la partie
 *    entière de chaque produit est le chiffre suivant (lu de haut en bas).
 * Certaines fractions (comme 0,1 en binaire) ne se terminent jamais : on
 * s'arrête alors à `max_chiffres`. */
bool reel_en_base(double x, unsigned base, unsigned max_chiffres,
                  char *sortie, size_t taille)
{
    if (x < 0.0 || x >= 1.8e19 || base < 2 || base > 36) {
        return false;
    }
    uint64_t partie_entiere = (uint64_t)x;
    double frac = x - (double)partie_entiere;

    if (!en_base(partie_entiere, base, sortie, taille)) {
        return false;
    }
    size_t pos = strlen(sortie);
    if (frac > 0.0 && max_chiffres > 0) {
        if (pos + 2 > taille) {
            return false;
        }
        sortie[pos++] = '.';
        for (unsigned k = 0; k < max_chiffres && frac > 0.0; k++) {
            frac *= base;
            unsigned chiffre = (unsigned)frac; /* partie entière du produit */
            frac -= chiffre;
            if (pos + 2 > taille) {
                return false;
            }
            sortie[pos++] = CHIFFRES[chiffre];
        }
        sortie[pos] = '\0';
    }
    return true;
}

/* base_en_reel : convertit une chaîne comme « 110001.001 » (écrite en base
 * `base`, séparateur '.' ou ',') en nombre décimal.
 *  - partie entière      : méthode de Horner (fonction depuis_base) ;
 *  - partie fractionnaire : chaque chiffre de rang -k pèse base^(-k).
 * Cela revient à calculer la forme polynomiale du nombre. */
bool base_en_reel(const char *s, unsigned base, double *resultat)
{
    char entier[80];
    const char *virgule = strpbrk(s, ".,");
    size_t len = virgule ? (size_t)(virgule - s) : strlen(s);
    uint64_t pe = 0;

    if (len >= sizeof entier) {
        return false;
    }
    memcpy(entier, s, len);
    entier[len] = '\0';
    if (len > 0 && !depuis_base(entier, base, &pe)) {
        return false;
    }
    double valeur = (double)pe;

    if (virgule != NULL) {
        /* On accumule la partie fractionnaire sous forme d'une FRACTION EXACTE
         * num / den (den = base^k) puis on divise une seule fois. Additionner
         * des poids comme 0,1 ou 0,01 introduirait des erreurs d'arrondi
         * (0,1 n'est pas exactement représentable en binaire). On cesse
         * d'accumuler quand den dépasse 2^53 : les chiffres suivants sont
         * au-delà de la précision d'un double (ils restent contrôlés). */
        uint64_t num = 0, den = 1;
        const uint64_t LIMITE = UINT64_C(1) << 53;

        for (const char *p = virgule + 1; *p != '\0'; p++) {
            int d = valeur_chiffre(*p);
            if (d < 0 || (unsigned)d >= base) {
                return false;
            }
            if (den <= LIMITE / base) {
                num = num * base + (uint64_t)d; /* chiffre de rang -k */
                den *= base;                    /* poids = 1 / base^k */
            }
        }
        valeur += (double)num / (double)den;
    } else if (len == 0) {
        return false; /* chaîne vide */
    }
    *resultat = valeur;
    return true;
}
