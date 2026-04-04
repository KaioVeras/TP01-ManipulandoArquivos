#ifndef LISTA_H
#define LISTA_H

// Inclusao de bibliotecas especificas para Windows
#ifdef _WIN32
#	include <fcntl.h>
#	include <io.h>
#	include <windows.h>
#endif

void configurarAcentuacao();
void aguardarEnter(void);
void concatenarArquivosCSV(const char *const *arquivos, int numArquivos);
void gerarResumoCSV(const char *const *arquivos, int numArquivos);
void mostrarMenu(void);
void gerarCSVPorMunicipio(void);

typedef struct {
    char sigla[10];
    long casos_novos_2026;
    long julgados_2026;
    long prim_sent2026;
    long suspensos_2026;
    long dessobrestados_2026;
    long cumprimento_meta1;
    
    long distm2_a;
    long julgm2_a;
    long suspm2_a;
    long cumprimento_meta2a;
    
    long distm2_ant;
    long julgm2_ant;
    long suspm2_ant;
    long desom2_ant;
    long cumprimento_meta2ant;
    
    long distm4_a;
    long julgm4_a;
    long suspm4_a;
    long cumprimento_meta4a;
    
    long distm4_b;
    long julgm4_b;
    long suspm4_b;
    long cumprimento_meta4b;

} ResumoGeral;



#endif