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

    FILE *entrada, *saida;
    char buffer[1024];
    const char *arquivos[] = {
        "data/teste_TRE-AC.csv",
        "data/teste_TRE-AL.csv",
        "data/teste_TRE-AM.csv",
        "data/teste_TRE-AP.csv",
        "data/teste_TRE-BA.csv",
        "data/teste_TRE-CE.csv",
        "data/teste_TRE-DF.csv",
        "data/teste_TRE-ES.csv",
        "data/teste_TRE-GO.csv",
        "data/teste_TRE-MA.csv",
        "data/teste_TRE-MG.csv",
        "data/teste_TRE-MS.csv",
        "data/teste_TRE-MT.csv",
        "data/teste_TRE-PA.csv",
        "data/teste_TRE-PB.csv",
        "data/teste_TRE-PE.csv",
        "data/teste_TRE-PI.csv",
        "data/teste_TRE-PR.csv",
        "data/teste_TRE-RJ.csv",
        "data/teste_TRE-RN.csv",
        "data/teste_TRE-RO.csv",
        "data/teste_TRE-RR.csv",
        "data/teste_TRE-RS.csv",
        "data/teste_TRE-SC.csv",
        "data/teste_TRE-SE.csv",
        "data/teste_TRE-SP.csv",
        "data/teste_TRE-TO.csv"
    };
    int numArquivos = sizeof(arquivos) / sizeof(arquivos[0]);

    // Cria o arquivo CSV de saída
    saida = fopen("baseDeDadosTotal.csv", "w");
    if (saida == NULL) {
        perror("Erro ao criar arquivo de saída");
        return 1;
    }

    // Percorre todos os arquivos da lista
    for (int i = 0; i < numArquivos; i++) {
        entrada = fopen(arquivos[i], "r");
        if (entrada == NULL) {
            perror("Erro ao abrir arquivo de entrada");
            continue;
        }

        // Lê cada linha e escreve no CSV
        while (fgets(buffer, sizeof(buffer), entrada)) {
            fprintf(saida, "%s", buffer);
        }

        fclose(entrada);
    }

    fclose(saida);
    printf("Arquivos concatenados com sucesso em 'baseDeDadosTotal.csv'.\n");

    return 0;
}