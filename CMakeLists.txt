cmake_minimum_required(VERSION 3.10)

project(c-bit-toolkit
        VERSION 1.0.0
        DESCRIPTION "Manipulation de bits et conversions de bases en C"
        LANGUAGES C)

# --- Standard du langage ------------------------------------------------------
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

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
