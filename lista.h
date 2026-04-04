#ifndef LISTA_H
#define LISTA_H

// Inclusao de bibliotecas especificas para Windows
#ifdef _WIN32
#	include <fcntl.h>
#	include <io.h>
#	include <windows.h>
#endif

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

void configurarAcentuacao();
void aguardarEnter(void);
int concatenarArquivosCSV(const char *const *arquivos, int numArquivos);
int arquivoBaseConcatenadoExiste(void);
void gerarResumoCSV(const char *const *arquivos, int numArquivos);
void mostrarMenu(void);
void gerarCSVPorMunicipio(void);

#endif