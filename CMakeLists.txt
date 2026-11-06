cmake_minimum_required(VERSION 3.14)

project(c-bit-toolkit
        VERSION 1.0.0
        DESCRIPTION "Manipulation de bits et conversions de bases en C"
        LANGUAGES C)

# --- Standard du langage ------------------------------------------------------
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# --- Support de l'éditeur (LSP : clangd, ccls, YouCompleteMe...) --------------
# Génère build/compile_commands.json : la liste exacte des options de chaque
# fichier (dont -I include). C'est ce qui permet à l'éditeur de retrouver
# base_conv.h, bits.h, etc. dans include/, comme le fait le compilateur.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Lien symbolique à la racine du projet, là où l'éditeur le cherche.
# Un échec (par exemple sous Windows sans droit de créer des liens) est ignoré :
# l'éditeur utilise alors compile_flags.txt, fourni à la racine du projet.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    file(CREATE_LINK "${CMAKE_BINARY_DIR}/compile_commands.json"
         "${CMAKE_SOURCE_DIR}/compile_commands.json"
         RESULT COMPILE_COMMANDS_LINK_RESULT SYMBOLIC)
endif()

# --- Type de build par défaut -------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Type de build" FORCE)
endif()

# --- Bibliothèque : logique métier (sans entrées/sorties interactives) --------
add_library(bitslib STATIC
    src/bits.c
    src/base_conv.c
    src/codes.c
    src/capacite.c
)
target_include_directories(bitslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- Exécutable : interface, tests et point d'entrée --------------------------
add_executable(c-bit-toolkit
    src/main.c
    src/menus.c
    src/saisie.c
    src/tests.c
)
target_link_libraries(c-bit-toolkit PRIVATE bitslib)

# --- Avertissements du compilateur --------------------------------------------
if(MSVC)
    set(PROJECT_WARNINGS /W4)
else()
    set(PROJECT_WARNINGS -Wall -Wextra -pedantic)
endif()
target_compile_options(bitslib PRIVATE ${PROJECT_WARNINGS})
target_compile_options(c-bit-toolkit PRIVATE ${PROJECT_WARNINGS})

# --- Tests (ctest) ------------------------------------------------------------
enable_testing()
add_test(NAME tests_automatiques COMMAND c-bit-toolkit --test)
