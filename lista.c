#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "lista.h"

#define MAX_CAMPOS_CSV 64
#define TAM_CAMPO_CSV 256
#define MAX_TRIBUNAIS 64

typedef struct {
    char sigla[32];
    double casos_novos_2026;
    double julgados_2026;
    double suspensos_2026;
    double dessobrestados_2026;
    double distm2_a;
    double julgm2_a;
    double suspm2_a;
    double distm2_ant;
    double julgm2_ant;
    double suspm2_ant;
    double desom2_ant;
    double distm4_a;
    double julgm4_a;
    double suspm4_a;
    double distm4_b;
    double julgm4_b;
    double suspm4_b;
} AcumuladoTribunal;

static FILE *abrirArquivoComPrefixos(const char *arquivo, const char *modo, char *caminhoResolvido, size_t tamanhoCaminho) {
    const char *prefixos[] = {"", "../"};

    for (size_t i = 0; i < sizeof(prefixos) / sizeof(prefixos[0]); i++) {
        snprintf(caminhoResolvido, tamanhoCaminho, "%s%s", prefixos[i], arquivo);
        FILE *arquivoAberto = fopen(caminhoResolvido, modo);
        if (arquivoAberto != NULL) {
            return arquivoAberto;
        }
    }

    caminhoResolvido[0] = '\0';
    return NULL;
}

static double converterNumero(const char *texto) {
    if (texto == NULL || texto[0] == '\0') {
        return 0.0;
    }

    return strtod(texto, NULL);
}

static double calcularPercentual(double numerador, double denominador, double fator) {
    if (denominador == 0.0) {
        return 0.0;
    }

    return (numerador / denominador) * fator;
}

static int extrairCamposCSV(const char *linha, char campos[][TAM_CAMPO_CSV], int maxCampos) {
    int indiceCampo = 0;
    int indiceChar = 0;
    int emAspas = 0;

    for (int i = 0; linha[i] != '\0'; i++) {
        char c = linha[i];

        if (c == '\r' || c == '\n') {
            break;
        }

        if (c == '"') {
            emAspas = !emAspas;
            continue;
        }

        if ((c == ',' || c == ';') && !emAspas) {
            campos[indiceCampo][indiceChar] = '\0';
            indiceCampo++;
            indiceChar = 0;

            if (indiceCampo >= maxCampos) {
                return maxCampos;
            }

            continue;
        }

        if (indiceChar < TAM_CAMPO_CSV - 1) {
            campos[indiceCampo][indiceChar++] = c;
        }
    }

    if (indiceCampo < maxCampos) {
        campos[indiceCampo][indiceChar] = '\0';
        indiceCampo++;
    }

    return indiceCampo;
}

static void converterSeparadorLinha(const char *origem, char *destino, size_t tamanhoDestino) {
    size_t j = 0;
    int emAspas = 0;

    for (size_t i = 0; origem[i] != '\0' && j < tamanhoDestino - 1; i++) {
        char c = origem[i];

        if (c == '"') {
            emAspas = !emAspas;
        }

        if (c == ',' && !emAspas) {
            c = ';';
        }

        destino[j++] = c;
    }

    destino[j] = '\0';
}

static void normalizarTexto(char *texto) {
    size_t inicio = 0;
    size_t fim = strlen(texto);

    while (texto[inicio] != '\0' && isspace((unsigned char)texto[inicio])) {
        inicio++;
    }

    while (fim > inicio && isspace((unsigned char)texto[fim - 1])) {
        fim--;
    }

    if (inicio > 0) {
        memmove(texto, texto + inicio, fim - inicio);
    }

    texto[fim - inicio] = '\0';

    for (size_t i = 0; texto[i] != '\0'; i++) {
        texto[i] = (char)toupper((unsigned char)texto[i]);
    }
}

static int encontrarOuCriarTribunal(AcumuladoTribunal tribunais[], int *qtdTribunais, const char *sigla) {
    for (int i = 0; i < *qtdTribunais; i++) {
        if (strcmp(tribunais[i].sigla, sigla) == 0) {
            return i;
        }
    }

    if (*qtdTribunais >= MAX_TRIBUNAIS) {
        return -1;
    }

    int novoIndice = *qtdTribunais;
    memset(&tribunais[novoIndice], 0, sizeof(AcumuladoTribunal));
    strncpy(tribunais[novoIndice].sigla, sigla, sizeof(tribunais[novoIndice].sigla) - 1);
    tribunais[novoIndice].sigla[sizeof(tribunais[novoIndice].sigla) - 1] = '\0';
    (*qtdTribunais)++;

    return novoIndice;
}

void aguardarEnter(void) {
    char bufferEspera[8];
    printf("\nPressione ENTER para continuar...");
    fgets(bufferEspera, sizeof(bufferEspera), stdin);
}

void concatenarArquivosCSV(const char *const *arquivos, int numArquivos) {
    FILE *entrada, *saida;
    char buffer[40000];
    char linhaConvertida[40000];
    int primeiroArquivo = 1;

    saida = fopen("baseDeDadosTotal.csv", "w");
    if (saida == NULL) {
        perror("Erro ao criar arquivo de saída");
        return;
    }

    for (int i = 0; i < numArquivos; i++) {
        char caminhoArquivo[256];
        entrada = abrirArquivoComPrefixos(arquivos[i], "r", caminhoArquivo, sizeof(caminhoArquivo));
        if (entrada == NULL) {
            perror("Erro ao abrir arquivo de entrada");
            continue;
        }

        if (!primeiroArquivo) {
            fgets(buffer, sizeof(buffer), entrada);
        }

        while (fgets(buffer, sizeof(buffer), entrada)) {
            converterSeparadorLinha(buffer, linhaConvertida, sizeof(linhaConvertida));
            fprintf(saida, "%s", linhaConvertida);
        }

        primeiroArquivo = 0;
        fclose(entrada);
    }

    fclose(saida);
    printf("Arquivos concatenados com sucesso em 'baseDeDadosTotal.csv'.\n");
}

void gerarResumoCSV(const char *const *arquivos, int numArquivos) {
    FILE *entrada, *saida;
    char linha[40000];
    AcumuladoTribunal tribunais[MAX_TRIBUNAIS];
    int qtdTribunais = 0;
    char caminhoArquivo[256];

    enum {
        IDX_SIGLA_TRIBUNAL = 0,
        IDX_CASOS_NOVOS_2026 = 10,
        IDX_JULGADOS_2026 = 11,
        IDX_SUSPENSOS_2026 = 13,
        IDX_DESSOBRESTADOS_2026 = 14,
        IDX_DISTM2_A = 16,
        IDX_JULGM2_A = 17,
        IDX_SUSPM2_A = 18,
        IDX_DISTM2_ANT = 20,
        IDX_JULGM2_ANT = 21,
        IDX_SUSPM2_ANT = 22,
        IDX_DESOM2_ANT = 23,
        IDX_DISTM4_A = 25,
        IDX_JULGM4_A = 26,
        IDX_SUSPM4_A = 27,
        IDX_DISTM4_B = 29,
        IDX_JULGM4_B = 30,
        IDX_SUSPM4_B = 31
    };

    (void)arquivos;
    (void)numArquivos;

    memset(tribunais, 0, sizeof(tribunais));

    entrada = abrirArquivoComPrefixos("baseDeDadosTotal.csv", "r", caminhoArquivo, sizeof(caminhoArquivo));
    if (entrada == NULL) {
        perror("Erro ao abrir 'baseDeDadosTotal.csv'. Gere o arquivo na opcao 1 antes do resumo");
        return;
    }

    saida = fopen("resumo.csv", "w");
    if (saida == NULL) {
        perror("Erro ao criar arquivo de resumo");
        fclose(entrada);
        return;
    }

    if (fgets(linha, sizeof(linha), entrada) != NULL) {
        while (fgets(linha, sizeof(linha), entrada)) {
            char campos[MAX_CAMPOS_CSV][TAM_CAMPO_CSV];
            int qtdCampos = extrairCamposCSV(linha, campos, MAX_CAMPOS_CSV);
            if (qtdCampos <= IDX_SUSPM4_B || campos[IDX_SIGLA_TRIBUNAL][0] == '\0') {
                continue;
            }

            int indiceTribunal = encontrarOuCriarTribunal(tribunais, &qtdTribunais, campos[IDX_SIGLA_TRIBUNAL]);
            if (indiceTribunal < 0) {
                continue;
            }

            AcumuladoTribunal *t = &tribunais[indiceTribunal];

            t->casos_novos_2026 += converterNumero(campos[IDX_CASOS_NOVOS_2026]);
            t->julgados_2026 += converterNumero(campos[IDX_JULGADOS_2026]);
            t->suspensos_2026 += converterNumero(campos[IDX_SUSPENSOS_2026]);
            t->dessobrestados_2026 += converterNumero(campos[IDX_DESSOBRESTADOS_2026]);

            t->distm2_a += converterNumero(campos[IDX_DISTM2_A]);
            t->julgm2_a += converterNumero(campos[IDX_JULGM2_A]);
            t->suspm2_a += converterNumero(campos[IDX_SUSPM2_A]);

            t->distm2_ant += converterNumero(campos[IDX_DISTM2_ANT]);
            t->julgm2_ant += converterNumero(campos[IDX_JULGM2_ANT]);
            t->suspm2_ant += converterNumero(campos[IDX_SUSPM2_ANT]);
            t->desom2_ant += converterNumero(campos[IDX_DESOM2_ANT]);

            t->distm4_a += converterNumero(campos[IDX_DISTM4_A]);
            t->julgm4_a += converterNumero(campos[IDX_JULGM4_A]);
            t->suspm4_a += converterNumero(campos[IDX_SUSPM4_A]);

            t->distm4_b += converterNumero(campos[IDX_DISTM4_B]);
            t->julgm4_b += converterNumero(campos[IDX_JULGM4_B]);
            t->suspm4_b += converterNumero(campos[IDX_SUSPM4_B]);
        }
    }

    fclose(entrada);

    fprintf(saida, "sigla_tribunal;total_julgados_2026;Meta1;Meta2A;Meta2Ant;Meta4A;Meta4B\n");

    for (int i = 0; i < qtdTribunais; i++) {
        AcumuladoTribunal *t = &tribunais[i];

        double meta1 = calcularPercentual(
            t->julgados_2026,
            (t->casos_novos_2026 + t->dessobrestados_2026 - t->suspensos_2026),
            100.0
        );

        double meta2A = calcularPercentual(
            t->julgm2_a,
            (t->distm2_a - t->suspm2_a),
            (1000.0 / 7.0)
        );

        double meta2Ant = calcularPercentual(
            t->julgm2_ant,
            (t->distm2_ant - t->suspm2_ant - t->desom2_ant),
            100.0
        );

        double meta4A = calcularPercentual(
            t->julgm4_a,
            (t->distm4_a - t->suspm4_a),
            100.0
        );

        double meta4B = calcularPercentual(
            t->julgm4_b,
            (t->distm4_b - t->suspm4_b),
            100.0
        );

        fprintf(
            saida,
            "%s;%.0f;%.6f;%.6f;%.6f;%.6f;%.6f\n",
            t->sigla,
            t->julgados_2026,
            meta1,
            meta2A,
            meta2Ant,
            meta4A,
            meta4B
        );
    }

    fclose(saida);

    printf("Resumo por sigla_tribunal gerado com sucesso em 'resumo.csv' a partir de 'baseDeDadosTotal.csv'.\n");
}

void mostrarMenu(void) {
    printf("\n========================================\n");
    printf("       TP01 - Manipulando Arquivos\n");
    printf("========================================\n\n");
    printf("1 - Concatenar todos os arquivos CSV\n");
    printf("2 - Gerar resumo dos arquivos CSV\n");
    printf("3 - Gerar arquivo por municipio\n");
    printf("0 - Sair\n\n");
    printf("Escolha uma opcao: ");
}

// Procedimento para configurar a acentuacao
void configurarAcentuacao() {
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

// função 3

void gerarCSVPorMunicipio(void) {
    char municipio[64];
    int totalOcorrencias = 0;
    printf("Digite o nome do municipio (ex: MACAPA): ");
    if (fgets(municipio, sizeof(municipio), stdin) == NULL) {
        return;
    }
    municipio[strcspn(municipio, "\r\n")] = 0;
    normalizarTexto(municipio);

    if (municipio[0] == '\0') {
        printf("Municipio invalido.\n");
        return;
    }

    FILE *entrada = fopen("baseDeDadosTotal.csv", "r");
    if (entrada == NULL) {
        perror("Erro ao abrir baseDeDadosTotal.csv");
        return;
    }

    char nomeArquivo[128];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.csv", municipio);
    FILE *saida = fopen(nomeArquivo, "w");
    if (saida == NULL) {
        perror("Erro ao criar arquivo de municipio");
        fclose(entrada);
        return;
    }

    char linha[40000];
    // Copia cabeçalho
    if (fgets(linha, sizeof(linha), entrada)) {
        fprintf(saida, "%s", linha);
    }

    while (fgets(linha, sizeof(linha), entrada)) {
        char campos[MAX_CAMPOS_CSV][TAM_CAMPO_CSV];
        int qtdCampos = extrairCamposCSV(linha, campos, MAX_CAMPOS_CSV);

        int municipio_oj = 5;

        if (qtdCampos > municipio_oj) {
            normalizarTexto(campos[municipio_oj]);
        }

        if (qtdCampos > municipio_oj && strcmp(campos[municipio_oj], municipio) == 0) {
            fprintf(saida, "%s", linha);
            totalOcorrencias++;
        }
    }

    fclose(entrada);
    fclose(saida);

    printf("Arquivo '%s' gerado com sucesso.\n", nomeArquivo);
    printf("Total de ocorrencias encontradas para '%s': %d\n", municipio, totalOcorrencias);

    if (totalOcorrencias == 0) {
        printf("Nenhum registro encontrado para o municipio informado.\n");
    }
}