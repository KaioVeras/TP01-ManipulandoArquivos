#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "lista.h"

// Procedimento para configurar a acentuacao
void configurarAcentuacao(void) {
    #ifdef _WIN32
        // Habilita suporte a UTF-8 no console do Windows
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        // Configura locale para UTF-8
        setlocale(LC_ALL, ".UTF8");
    #else
        // Para Linux/Mac, configura locale para UTF-8
        setlocale(LC_ALL, "");
    #endif
}