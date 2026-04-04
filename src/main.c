#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "lista.h"

int main(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    configurarAcentuacao();

    if (concatenarCsvs("data", "data/teste_TRE_concatenado.csv") != 0) {
        return 1;
    }

    printf("Concatenacao concluida em: data/teste_TRE_concatenado.csv\n");

    return 0;
}