/*
 * TP01 - MANIPULANDO ARQUIVOS
 * 
 * Programa: Sistema de Processamento de Dados Eleitorais
 * Propósito: Consolidar dados de 27 Tribunais Regionais Eleitorais (TRE)
 *            em um único arquivo, gerar resumos por tribunal com cálculos
 *            de metas judiciais e filtrar dados por município.
 * 
 * Autores: [Seu Nome] - 2026
 * Disciplina: Programação C
 * 
 * Arquivos:
 *   - main.c: Arquivo principal com menu interativo
 *   - lista.h: Definições de estruturas e protótipos de funções
 *   - lista.c: Implementação das funções de processamento
 * 
 * Funcionalidades:
 *   1. Concatenar todos os arquivos CSV em um único arquivo
 *   2. Gerar resumo consolidado com cálculo de metas por tribunal
 *   3. Filtrar e extrair dados de um município específico
 */

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

/*
 * FUNÇÃO: main
 * PROPÓSITO: Controlar fluxo principal da aplicação
 * 
 * FUNCIONAMENTO:
 *   1. Limpa a tela (compatível com Windows e Unix)
 *   2. Configura suporte a acentuação (UTF-8)
 *   3. Inicializa array com 27 nomes de arquivos (um por estado)
 *   4. Exibe menu interativo em loop
 *   5. Despacha para função apropriada baseado na opção do usuário
 *   6. Valida dependências (ex: opção 2 requer saída da opção 1)
 * 
 * RETORNO: 0 (sucesso)
 */
int main(void) {
    // Limpa tela - diferencia Windows (cls) de Linux/Mac (clear)
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    // Configura encoding UTF-8 para exibir acentuação corretamente
    configurarAcentuacao();

    // Array com os 27 arquivos CSV (um para cada estado brasileiro)
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

    // Loop de menu - continua até usuário escolher opção 0 (Sair)
    do {
        // Exibe menu principal
        mostrarMenu();

        // Lê opção digitada pelo usuário
        char entradaMenu[32];
        if (fgets(entradaMenu, sizeof(entradaMenu), stdin) == NULL) {
            break;  // EOF ou erro de leitura
        }

        // Converte string para número inteiro
        opcao = (int)strtol(entradaMenu, NULL, 10);

        // Despacha para função correspondente
        switch (opcao) {
            case 1:
                // OPÇÃO 1: Concatenar todos os 27 arquivos CSV em um único arquivo
                concatenarArquivosCSV(arquivos, numArquivos);
                aguardarEnter();
                break;
            case 2:
                // OPÇÃO 2: Gerar resumo (requer baseDeDadosTotal.csv da opção 1)
                if (!arquivoBaseConcatenadoExiste()) {
                    printf("Arquivo 'baseDeDadosTotal.csv' nao encontrado.\n");
                    printf("E necessario concatenar os arquivos primeiro (opcao 1).\n");
                    printf("Nenhum arquivo de resumo foi gerado.\n");
                } else {
                    gerarResumoCSV(arquivos, numArquivos);
                }
                aguardarEnter();
                break;
            case 3:
                // OPÇÃO 3: Filtrar dados por município (requer baseDeDadosTotal.csv)
                if (!arquivoBaseConcatenadoExiste()) {
                    printf("Arquivo 'baseDeDadosTotal.csv' nao encontrado.\n");
                    printf("E necessario concatenar os arquivos primeiro (opcao 1).\n");
                    printf("Nenhum arquivo de municipio foi gerado.\n");
                } else {
                    gerarCSVPorMunicipio();
                }
                aguardarEnter();
                break;
            case 0:
                // OPÇÃO 0: Sair do programa
                printf("Saindo...\n");
                break;
            default:
                // Opção inválida
                printf("Opcao invalida. Tente novamente.\n");
                aguardarEnter();
                break;
        }

        // Limpa tela após ação (exceto ao sair)
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