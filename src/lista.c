#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "lista.h"

const char *Arquivos[] = {
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

const int QuantidadeArquivos = sizeof(Arquivos) / sizeof(Arquivos[0]);

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

// #include <stdio.h>
// #include <stdlib.h>

// int main() {
//     FILE *entrada, *saida;
//     char buffer[1024];
//     char *arquivos[] = {"teste_TRE-AC.csv", "teste_TRE-.csv", "arquivo3.txt"}; 
//     int numArquivos = 27; // Número de arquivos a serem concatenados

//     // Cria o arquivo CSV de saída
//     saida = fopen("saida.csv", "w");
//     if (saida == NULL) {
//         printf("Erro ao criar arquivo de saída");
//         return 1;
//     }

//     // Percorre todos os arquivos da lista
//     for (int i = 0; i < numArquivos; i++) {  - Loop pelos arquivos: percorre todos os nomes listados em arquivos[].
//         entrada = fopen(arquivos[i], "r");  - Abre cada arquivo da lista em modo leitura (r).
//         if (entrada == NULL) {
//             perror("Erro ao abrir arquivo de entrada");
//             continue;
//         }            verificação abertura arquivo, Se não conseguiu abrir, mostra erro e continua para o próximo arquivo sem parar o programa.

//         // Lê cada linha e escreve no CSV
//         while (fgets(buffer, sizeof(buffer), entrada)) {
//             fprintf(saida, "%s", buffer);    
//         }

//         fclose(entrada);
//     }

//     fclose(saida);

//     printf("Arquivos concatenados com sucesso em 'saida.csv'.\n");

//     return 0;
// }
