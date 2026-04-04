#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

int main(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    configurarAcentuacao();

    const char *arquivos[] = {
        "teste_TRE-AC.csv",
        "teste_TRE-AL.csv",
        "teste_TRE-AM.csv",
        "teste_TRE-AP.csv",
        "teste_TRE-BA.csv",
        "teste_TRE-CE.csv",
        "teste_TRE-DF.csv",
        "teste_TRE-ES.csv",
        "teste_TRE-GO.csv",
        "teste_TRE-MA.csv",
        "teste_TRE-MG.csv",
        "teste_TRE-MS.csv",
        "teste_TRE-MT.csv",
        "teste_TRE-PA.csv",
        "teste_TRE-PB.csv",
        "teste_TRE-PE.csv",
        "teste_TRE-PI.csv",
        "teste_TRE-PR.csv",
        "teste_TRE-RJ.csv",
        "teste_TRE-RN.csv",
        "teste_TRE-RO.csv",
        "teste_TRE-RR.csv",
        "teste_TRE-RS.csv",
        "teste_TRE-SC.csv",
        "teste_TRE-SE.csv",
        "teste_TRE-SP.csv",
        "teste_TRE-TO.csv"
    };
    int numArquivos = sizeof(arquivos) / sizeof(arquivos[0]);
    int opcao;

    do {
        mostrarMenu();

        char entradaMenu[32];
        if (fgets(entradaMenu, sizeof(entradaMenu), stdin) == NULL) {
            break;
        }

        opcao = (int)strtol(entradaMenu, NULL, 10);

        switch (opcao) {
            case 1:
                concatenarArquivosCSV(arquivos, numArquivos);
                aguardarEnter();
                break;
            case 2:
                gerarResumoCSV(arquivos, numArquivos);
                aguardarEnter();
                break;
            case 3:
                gerarCSVPorMunicipio();
                aguardarEnter();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                aguardarEnter();
                break;
        }

        if (opcao != 0) {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
        }
    } while (opcao != 0);

    return 0;
}
