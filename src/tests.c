/*
 * tests.c -- Tests automatiques (voir tests.h)
 */
#include "tests.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "base_conv.h"
#include "bits.h"
#include "capacite.h"
#include "codes.h"

static int tests_echoues = 0;

/* verifier : affiche OK/ECHEC pour une condition de test. */
static void verifier(bool condition, const char *description)
{
    printf("  [%s] %s\n", condition ? "OK" : "ECHEC", description);
    if (!condition) {
        tests_echoues++;
    }
}

int executer_tests(void)
{
    char buf[100];
    uint64_t v;
    uint32_t u;
    double d;

    tests_echoues = 0; /* compteur remis à zéro à chaque exécution */
    printf("=== Tests automatiques ===\n");

    en_base(49, 2, buf, sizeof buf);
    verifier(strcmp(buf, "110001") == 0, "49 en binaire = 110001");

    reel_en_base(49.125, 2, 16, buf, sizeof buf);
    verifier(strcmp(buf, "110001.001") == 0, "49,125 en binaire = 110001,001");

    verifier(depuis_base("AB", 16, &v) && v == 171, "AB(16) = 171");
    verifier(bcd_coder(171, &u) && u == 0x171u, "171 en BCD = 0001 0111 0001");

    verifier(depuis_base("47", 8, &v) && v == 39, "47(8) = 39");
    en_base(39, 2, buf, sizeof buf);
    verifier(strcmp(buf, "100111") == 0, "47(8) en binaire = 100111");

    depuis_base("101", 3, &v);
    en_base(v, 8, buf, sizeof buf);
    verifier(strcmp(buf, "12") == 0, "101(3) en octal = 12");

    depuis_base("11101", 2, &v);
    en_base(v, 5, buf, sizeof buf);
    verifier(strcmp(buf, "104") == 0, "11101(2) en base 5 = 104");

    verifier(base_en_reel("1011.01", 2, &d) && d == 11.25, "1011,01(2) = 11,25");
    verifier(!depuis_base("1021", 2, &v), "1021 refuse en base 2 (chiffre 2 interdit)");
    verifier(!bcd_decoder(0xAB, &u), "BCD 1010 1011 refuse (quartets > 9)");

    verifier(bits_necessaires(100) == 7, "100 valeurs -> 7 bits");
    verifier(bits_necessaires(256) == 8, "256 valeurs -> 8 bits");
    verifier(bits_necessaires(1500ull << 20u) == 31, "1500 Mo (adressage a l'octet) -> 31 bits");

    verifier(gray_coder(9) == 0xD, "Gray(9) = 1101");
    verifier(gray_decoder(0x6) == 4, "Gray 0110 -> 4");
    verifier(bit_compter(0xFF) == 8 && est_puissance_de_2(64) && !est_puissance_de_2(65),
             "popcount et puissance de 2");
    verifier(rotation_gauche(0x80000001u, 1) == 0x00000003u, "rotation a gauche");
    verifier(bit_inverser(bit_mettre_a_1(0, 3), 3) == 0, "set puis toggle d'un bit");

    printf("%s\n", tests_echoues == 0 ? "Tous les tests sont reussis."
                                       : "Des tests ont echoue !");
    return tests_echoues;
}
