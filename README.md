# c-bit-toolkit

Manipulation de bits et conversions de bases en C (norme C99), avec un menu interactif et des tests automatiques.

Ce projet est issu du cours d'Automatisme (LTP Alarobia, 2nde FTG). Il illustre :

- les conversions entre bases (2 à 36), pour les entiers et les nombres fractionnaires ;
- les codes BCD et Gray ;
- les opérations bit à bit (ET, OU, OU exclusif, NON, décalages, rotations) ;
- la manipulation d'un bit précis (lire, mettre à 1, mettre à 0, inverser) ;
- la table de codage en binaire naturel sur n bits ;
- la capacité de codage : nombre de bits pour M valeurs ou pour adresser une mémoire de X Mo.

## Arborescence

```
c-bit-toolkit/
├── CMakeLists.txt
├── README.md
├── compile_flags.txt
├── include/
│   ├── base_conv.h
│   ├── bits.h
│   ├── capacite.h
│   ├── codes.h
│   ├── menus.h
│   ├── saisie.h
│   └── tests.h
└── src/
    ├── base_conv.c
    ├── bits.c
    ├── capacite.c
    ├── codes.c
    ├── main.c
    ├── menus.c
    ├── saisie.c
    └── tests.c
```

## Modules

| Module | Rôle |
|--------|------|
| `bits` | Lire, mettre à 1, mettre à 0 et inverser un bit ; poids de Hamming ; puissance de 2 ; rotations ; affichage binaire |
| `base_conv` | Conversions entiers et fractionnaires entre les bases 2 à 36 (divisions et multiplications successives, méthode de Horner) |
| `codes` | Codage et décodage BCD compacté et Gray |
| `capacite` | Nombre de bits nécessaires pour M valeurs ; table du binaire naturel sur n bits |
| `saisie` | Lecture sécurisée au clavier (`fgets`, `strtoull`) |
| `menus` | Menu interactif |
| `tests` | Tests automatiques issus de la fiche d'exercices |
| `main` | Point d'entrée |

Les quatre premiers modules forment la bibliothèque statique `bitslib` (sans entrées/sorties interactives). Les autres composent l'exécutable `c-bit-toolkit`.

## Prérequis

- Un compilateur C compatible C99 (GCC, Clang ou MSVC)
- CMake 3.14 ou plus récent

## Compilation

```bash
cmake -S . -B build
cmake --build build
```

L'exécutable est généré dans `build/` (`build/c-bit-toolkit`). Avec un générateur multi-configuration (Visual Studio, par exemple), il se trouve dans `build/Release/` ou `build/Debug/`.

## Utilisation

Menu interactif :

```bash
./build/c-bit-toolkit
```

Tests automatiques uniquement (code de retour 0 si tout réussit) :

```bash
./build/c-bit-toolkit --test
```

Tests via CTest :

```bash
cd build
ctest --output-on-failure
```

### Options du menu

| Choix | Fonction |
|-------|----------|
| 1 | Conversion entre bases (entier ou fractionnaire) |
| 2 | Codes binaire naturel, BCD et Gray d'un entier |
| 3 | Opérations bit à bit et manipulation d'un bit |
| 4 | Table du binaire naturel sur n bits (1 à 8) |
| 5 | Capacité de codage et adressage mémoire |
| 6 | Tests automatiques |
| 0 | Quitter |

### Exemple

```
Votre choix : 1
Base de depart (2-36) : 10
Base d'arrivee (2-36) : 2
Nombre (ex. 110001.001) : 49.125
(49.125)_10 = 49.125 en decimal = (110001.001)_2
```

## Configuration de l'éditeur (Vim, LSP)

Les en-têtes sont dans `include/` et les sources les incluent par leur nom (`#include "bits.h"`). Le compilateur les trouve grâce à l'option `-Iinclude` ajoutée par CMake. Un serveur LSP comme **clangd** ne connaît pas cette option : sans configuration, il signale `'bits.h' file not found` alors que le projet compile très bien.

Le projet fournit deux sources d'information pour l'éditeur :

| Fichier | Origine | Rôle |
|---------|---------|------|
| `compile_commands.json` | généré par CMake (lien créé à la racine) | options exactes de chaque fichier ; prioritaire |
| `compile_flags.txt` | versionné dans le dépôt | repli : `-Iinclude`, C99 et avertissements, utilisé même sans compilation préalable |

Mise en route :

```bash
cmake -S . -B build        # génère build/compile_commands.json et le lien à la racine
```

Puis redémarrer le serveur LSP dans Vim (`:LspRestart` avec vim-lsp, `:CocRestart` avec coc.nvim). Les plugins qui s'appuient sur clangd (vim-lsp, coc-clangd, ALE, YouCompleteMe) utilisent ces fichiers automatiquement.

Sous Windows, si le lien symbolique ne peut pas être créé, copier le fichier à la main :

```bash
cmake -E copy build/compile_commands.json .
```

## Limites

- Entiers de 64 bits au maximum pour les conversions de bases.
- Les opérations sur les bits portent sur des entiers de 32 bits.
- BCD : valeurs de 0 à 99 999 999 (8 chiffres dans 32 bits).
- Partie fractionnaire : au plus 20 chiffres après la virgule à l'affichage ; certaines fractions (0,1 en binaire) ne se terminent jamais.
- La table du binaire naturel accepte 1 à 8 bits.
