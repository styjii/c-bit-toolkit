/*
 * main.c -- Point d'entrée de c-bit-toolkit
 *
 *  Exécution :  ./c-bit-toolkit          (menu interactif)
 *               ./c-bit-toolkit --test   (tests automatiques uniquement)
 */
#include <stdlib.h>
#include <string.h>

#include "menus.h"
#include "tests.h"

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        return executer_tests() == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    menu_principal();
    return EXIT_SUCCESS;
}
