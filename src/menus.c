/*
 * menus.c -- Interface utilisateur : menu interactif (voir menus.h)
 */
#include "menus.h"

#include <stdint.h>
#include <stdio.h>

#include "base_conv.h"
#include "bits.h"
#include "capacite.h"
#include "codes.h"
#include "saisie.h"
#include "tests.h"

/* menu_conversion : nombre (entier ou fractionnaire) d'une base vers une autre,
 * en passant par la base 10 comme « pivot ». */
static void menu_conversion(void)
{
    char saisie[80], sortie[160];
    uint64_t b1, b2;
    double valeur;

    if (!lire_entier("Base de depart (2-36) : ", &b1) ||
        !lire_entier("Base d'arrivee (2-36) : ", &b2)) {
        return;
    }
    if (b1 < 2 || b1 > 36 || b2 < 2 || b2 > 36) {
        printf("Les bases doivent etre comprises entre 2 et 36.\n");
        return;
    }
    if (!lire_ligne("Nombre (ex. 110001.001) : ", saisie, sizeof saisie)) {
        return;
    }
    if (!base_en_reel(saisie, (unsigned)b1, &valeur)) {
        printf("Nombre invalide dans la base %u.\n", (unsigned)b1);
        return;
    }
    if (!reel_en_base(valeur, (unsigned)b2, 20, sortie, sizeof sortie)) {
        printf("Conversion impossible (valeur trop grande).\n");
        return;
    }
    printf("(%s)_%u = %.10g en decimal = (%s)_%u\n", saisie, (unsigned)b1,
           valeur, sortie, (unsigned)b2);
}

/* menu_bcd_gray : codage BCD et Gray d'un entier. */
static void menu_bcd_gray(void)
{
    uint64_t n;
    uint32_t bcd;

    if (!lire_entier("Entier decimal : ", &n)) {
        return;
    }
    if (n > UINT32_MAX) {
        printf("Valeur trop grande pour ce menu (max %lu).\n", (unsigned long)UINT32_MAX);
        return;
    }
    /* Largeur d'affichage : nombre de bits strictement nécessaires (au moins 1). */
    unsigned largeur = bits_necessaires(n + 1);
    if (largeur == 0) {
        largeur = 1;
    }
    printf("Binaire naturel : ");
    afficher_bits((uint32_t)n, largeur);
    printf("\nCode Gray       : ");
    afficher_bits(gray_coder((uint32_t)n), largeur);
    printf("\n");

    if (bcd_coder((uint32_t)n, &bcd)) {
        unsigned chiffres = 1; /* nombre de chiffres décimaux de n */
        for (uint32_t t = (uint32_t)n; t >= 10; t /= 10) {
            chiffres++;
        }
        printf("Code BCD        : ");
        afficher_bits(bcd, 4 * chiffres);
        printf("\n");
    } else {
        printf("Code BCD        : valeur trop grande (max 99 999 999).\n");
    }
}

/* menu_operations : opérations bit à bit sur deux entiers de 32 bits. */
static void menu_operations(void)
{
    uint64_t a, b, i;

    if (!lire_entier("Valeur A (decimal ou 0x..) : ", &a) ||
        !lire_entier("Valeur B (decimal ou 0x..) : ", &b)) {
        return;
    }
    uint32_t x = (uint32_t)a, y = (uint32_t)b;

    printf("\nA          = "); afficher_bits(x, 32);
    printf("\nB          = "); afficher_bits(y, 32);
    printf("\nA ET B     = "); afficher_bits(x & y, 32);
    printf("\nA OU B     = "); afficher_bits(x | y, 32);
    printf("\nA XOR B    = "); afficher_bits(x ^ y, 32);
    printf("\nNON A      = "); afficher_bits(~x, 32);
    printf("\nA << 1     = "); afficher_bits(x << 1u, 32);
    printf("\nA >> 1     = "); afficher_bits(x >> 1u, 32);
    printf("\nRotG(A,1)  = "); afficher_bits(rotation_gauche(x, 1), 32);
    printf("\nRotD(A,1)  = "); afficher_bits(rotation_droite(x, 1), 32);
    printf("\nBits a 1 dans A : %u ; A est une puissance de 2 : %s\n",
           bit_compter(x), est_puissance_de_2(x) ? "oui" : "non");

    if (lire_entier("\nRang i du bit a manipuler dans A (0-31) : ", &i) && i < 32) {
        unsigned r = (unsigned)i;
        printf("bit %u de A     = %u\n", r, bit_lire(x, r));
        printf("mise a 1         = "); afficher_bits(bit_mettre_a_1(x, r), 32);
        printf("\nmise a 0         = "); afficher_bits(bit_mettre_a_0(x, r), 32);
        printf("\ninversion        = "); afficher_bits(bit_inverser(x, r), 32);
        printf("\n");
    } else {
        printf("Rang invalide.\n");
    }
}

/* menu_capacite : nombre de bits pour M valeurs, ou adresse d'une mémoire. */
static void menu_capacite(void)
{
    uint64_t choix, v;

    if (!lire_entier("1 = coder M valeurs, 2 = adresser X Mo (a l'octet) : ", &choix)) {
        return;
    }
    if (choix == 1) {
        if (lire_entier("M (nombre de valeurs distinctes, >= 1) : ", &v) && v >= 1) {
            printf("Il faut %u bit(s) : 2^%u = %llu >= %llu\n", bits_necessaires(v),
                   bits_necessaires(v),
                   (unsigned long long)(UINT64_C(1) << bits_necessaires(v)),
                   (unsigned long long)v);
        }
    } else if (choix == 2) {
        if (lire_entier("Capacite en Mo (1 Mo = 2^20 octets) : ", &v) && v >= 1 && v < (UINT64_C(1) << 40u)) {
            uint64_t octets = v << 20u;
            unsigned long long bits_stockage = (unsigned long long)octets * 8u;

            printf("%llu Mo = %llu octets = %llu bits de stockage\n",
                   (unsigned long long)v, (unsigned long long)octets, bits_stockage);
            printf("Bits d'adresse (1 adresse par octet) : %u\n", bits_necessaires(octets));
        } else {
            printf("Valeur invalide.\n");
        }
    }
}

void menu_principal(void)
{
    for (;;) {
        uint64_t choix, n;

        printf("\n=========== MANIPULATION DE BITS ===========\n"
               " 1. Conversion entre bases (entier ou fractionnaire)\n"
               " 2. Codes BN, BCD et Gray d'un entier\n"
               " 3. Operations bit a bit (ET, OU, XOR, NON, decalages...)\n"
               " 4. Table du binaire naturel sur n bits\n"
               " 5. Capacite de codage / adressage memoire\n"
               " 6. Tests automatiques\n"
               " 0. Quitter\n");
        if (!lire_entier("Votre choix : ", &choix)) {
            return; /* fin de fichier */
        }
        switch (choix) {
        case 1: menu_conversion(); break;
        case 2: menu_bcd_gray(); break;
        case 3: menu_operations(); break;
        case 4:
            if (lire_entier("Nombre de bits n (1-8) : ", &n)) {
                afficher_table_bn((unsigned)n);
            }
            break;
        case 5: menu_capacite(); break;
        case 6: executer_tests(); break;
        case 0: printf("Au revoir !\n"); return;
        default: printf("Choix inconnu.\n");
        }
    }
}
