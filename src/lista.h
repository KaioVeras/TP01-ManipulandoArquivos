#ifndef LISTA_H
#define LISTA_H

// Inclusao de bibliotecas especificas para Windows
#ifdef _WIN32
#	include <fcntl.h>
#	include <io.h>
#	include <windows.h>
#endif

typedef struct {
    char sigla_tribunal[10];
    char procedimento[50];
    char ramo_justica[20];
    char sigla_grau[10];
    char uf_oj[5];
    char municipio_oj[50];
    char id_ultimo_oj[20];
    char nome[100];
    char mesano_cnm1[10];
    char mesano_sent[10];
    int casos_novos_2026;
    int julgados_2026;
    int prim_sent2026;
    int suspensos_2026;
    int dessobrestados_2026;
    float cumprimento_meta1;
    float distm2_a;
    float julgm2_a;
    float suspm2_a;
    float cumprimento_meta2a;
    float distm2_ant;
    float julgm2_ant;
    float suspm2_ant;
    float desom2_ant;
    float cumprimento_meta2ant;
    float distm4_a;
    float julgm4_a;
    float suspm4_a;
    float cumprimento_meta4a;
    float distm4_b;
    float julgm4_b;
    float suspm4_b;
    float cumprimento_meta4b;
} Processo;

void configurarAcentuacao(void);
int concatenarCsvs(const char *diretorioEntrada, const char *arquivoSaida);

#endif