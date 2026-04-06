#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "lista.h"

/*
 * FUNÇÃO: abrirArquivoComPrefixos
 * PROPÓSITO: Encontrar e abrir um arquivo testando múltiplos caminhos
 * 
 * MOTIVAÇÃO: Torna o programa robusto a diferentes estruturas de diretório
 *            (arquivo no diretório atual ou no diretório pai)
 * 
 * PARÂMETROS:
 *   arquivo            - Nome do arquivo a abrir (ex: "teste_TRE-SP.csv")
 *   modo               - Modo de abertura ("r", "w", "a", etc)
 *   caminhoResolvido   - OUT: Caminho que funcionou
 *   tamanhoCaminho     - Tamanho do buffer caminhoResolvido
 * 
 * RETORNO: Arquivo aberto ou NULL se não encontrado
 * 
 * ALGORITMO:
 *   1. Testa prefixos em ordem: "" (atual), "../" (pai)
 *   2. Para cada prefixo, tenta fopen()
 *   3. Se sucesso, copia caminho e retorna FILE*
 *   4. Se nenhum funciona, retorna NULL
 */
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

/*
 * FUNÇÃO: arquivoBaseConcatenadoExiste
 * PROPÓSITO: Verificar se arquivo consolidado já foi gerado
 * 
 * RETORNO: 1 se existe, 0 se não existe
 * 
 * NOTA: Testa ambos os caminhos (atual e pai) pois o usuário pode
 *       executar o programa de diretórios diferentes
 */
int arquivoBaseConcatenadoExiste(void) {
    const char *caminhos[] = {"baseDeDadosTotal.csv", "../baseDeDadosTotal.csv"};

    for (size_t i = 0; i < sizeof(caminhos) / sizeof(caminhos[0]); i++) {
        FILE *arquivo = fopen(caminhos[i], "r");
        if (arquivo != NULL) {
            fclose(arquivo);
            return 1;
        }
    }

    return 0;
}

/*
 * FUNÇÃO: converterNumero
 * PROPÓSITO: Converter string para double com proteção
 * 
 * PARÂMETRO: texto - String com número (ex: "123.45", "", NULL)
 * RETORNO: Número convertido ou 0.0 se inválido
 * 
 * MOTIVAÇÃO: Quando um campo CSV está vazio, não queremos um erro
 *            Na agregação, 0 contribui corretamente (elemento neutro)
 */
static double converterNumero(const char *texto) {
    if (texto == NULL || texto[0] == '\0') {
        return 0.0;  // Campo vazio → contribui 0 no total
    }

    return strtod(texto, NULL);
}

/*
 * FUNÇÃO: calcularPercentual
 * PROPÓSITO: Calcular percentuais com proteção contra divisão por zero
 * 
 * FÓRMULA: (numerador / denominador) * fator
 * 
 * PARÂMETROS:
 *   numerador    - Número de casos positivos
 *   denominador  - Base para cálculo
 *   fator        - Multiplicador (100 para %, 1000/7 para metas especiais)
 * 
 * RETORNO: Percentual calculado ou 0.0 se denominador = 0
 * 
 * EXEMPLOS:
 *   Meta 1: calcularPercentual(50000, 52000, 100.0)      → ~96.15%
 *   Meta 2A: calcularPercentual(40000, 42000, 1000/7)    → ~170.74...
 */
static double calcularPercentual(double numerador, double denominador, double fator) {
    if (denominador == 0.0) {
        return 0.0;  // Proteção: evita divisão por zero
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

/*
 * FUNÇÃO: extrairCamposCSV
 * PROPÓSITO: Fazer parsing de uma linha CSV respeitando campos entre aspas
 * 
 * MOTIVAÇÃO: Dados CSV podem conter separadores dentro de campos entre aspas
 *            Exemplo: "Silva, José" deve ser 1 campo, não 2
 * 
 * PARÂMETROS:
 *   linha        - String com a linha CSV completa
 *   campos       - OUT: Array bidimensional para armazenar campos extraídos
 *   maxCampos    - Máximo de campos a extrair
 * 
 * RETORNO: Número de campos extraídos
 * 
 * ALGORITMO:
 *   1. Estado máquina: controla se estamos dentro/fora de aspas
 *   2. Lê caractere por caractere:
 *      - Troca estado quando encontra " (aspas duplas)
 *      - Ignora separadores dentro de aspas
 *      - Finaliza campo quando encontra separador FORA de aspas
 *   3. Pula caracteres ESC (\\r, \\n)
 * 
 * EXEMPLO:
 *   Input:  "Silva, José","SP","Rua, Av",123
 *   Output: campos[0] = "Silva, José"
 *           campos[1] = "SP"
 *           campos[2] = "Rua, Av"
 *           campos[3] = "123"
 *           retorno = 4
 */
static int extrairCamposCSV(const char *linha, char campos[][TAM_CAMPO_CSV], int maxCampos) {
    int indiceCampo = 0;      // Qual campo estamos preenchendo
    int indiceChar = 0;       // Qual posição dentro do campo atual
    int emAspas = 0;          // Estado: dentro de aspas (1) ou não (0)

    for (int i = 0; linha[i] != '\0'; i++) {
        char c = linha[i];

        // Para na quebra de linha
        if (c == '\r' || c == '\n') {
            break;
        }

        // Controla entrada/saída de aspas (não adiciona aspas ao campo)
        if (c == '"') {
            emAspas = !emAspas;
            continue;
        }

        // Separa campos: vírgula OU ponto-e-vírgula (se fora de aspas)
        if ((c == ',' || c == ';') && !emAspas) {
            campos[indiceCampo][indiceChar] = '\0';  // Finaliza string
            indiceCampo++;
            indiceChar = 0;

            if (indiceCampo >= maxCampos) {
                return maxCampos;  // Atingiu limite
            }

            continue;
        }

        // Adiciona caractere ao campo atual (com proteção de buffer)
        if (indiceChar < TAM_CAMPO_CSV - 1) {
            campos[indiceCampo][indiceChar++] = c;
        }
    }

    // Finaliza último campo
    if (indiceCampo < maxCampos) {
        campos[indiceCampo][indiceChar] = '\0';
        indiceCampo++;
    }

    return indiceCampo;
}

/*
 * FUNÇÃO: converterSeparadorLinha
 * PROPÓSITO: Normalizar linha CSV convertendo ponto-e-vírgula para vírgula
 * 
 * PRESERVA: Separadores dentro de aspas não são convertidos
 * 
 * MOTIVAÇÃO: Diferentes sistemas usam separadores diferentes
 *            Excel português/europeu usa ponto-e-vírgula
 *            Este programa espera vírgulas para consistência
 * 
 * PARÂMETROS:
 *   origem          - Linha original
 *   destino         - Linha convertida
 *   tamanhoDestino  - Tamanho máximo de destino
 */
static void converterSeparadorLinha(const char *origem, char *destino, size_t tamanhoDestino) {
    size_t j = 0;
    int emAspas = 0;

    for (size_t i = 0; origem[i] != '\0' && j < tamanhoDestino - 1; i++) {
        char c = origem[i];

        if (c == '"') {
            emAspas = !emAspas;
        }

        // Converte ';' para ',' apenas se NÃO estamos dentro de aspas
        if (c == ';' && !emAspas) {
            c = ',';
        }

        destino[j++] = c;
    }

    destino[j] = '\0';
}

/*
 * FUNÇÃO: normalizarTexto
 * PROPÓSITO: Padronizar texto para comparações consistentes
 * 
 * PROCESSAMENTO:
 *   1. Remove espaços em branco do início
 *   2. Remove espaços em branco do final
 *   3. Converte para MAIÚSCULAS
 * 
 * MOTIVAÇÃO: Permite buscar "macapa", "MACAPA", "Macapá" de forma uniforme
 *            e ignorar espaçamento inconsistente
 * 
 * NOTA: Modifica string IN-PLACE
 * 
 * EXEMPLO:
 *   Input:  "  são paulo  "
 *   Output: "SÃO PAULO"
 */
static void normalizarTexto(char *texto) {
    size_t inicio = 0;
    size_t fim = strlen(texto);

    // Remove espaços do início
    while (texto[inicio] != '\0' && isspace((unsigned char)texto[inicio])) {
        inicio++;
    }

    // Remove espaços do final
    while (fim > inicio && isspace((unsigned char)texto[fim - 1])) {
        fim--;
    }

    // Move string normalizada para o início
    if (inicio > 0) {
        memmove(texto, texto + inicio, fim - inicio);
    }

    // Finaliza string no novo comprimento
    texto[fim - inicio] = '\0';

    // Converte para maiúsculas
    for (size_t i = 0; texto[i] != '\0'; i++) {
        texto[i] = (char)toupper((unsigned char)texto[i]);
    }
}

/*
 * FUNÇÃO: encontrarOuCriarTribunal
 * PROPÓSITO: Localizar tribunal por sigla ou criar novo registro
 * 
 * PARÂMETROS:
 *   tribunais     - Array de AcumuladoTribunal
 *   qtdTribunais  - IN/OUT: quantidade atual (incrementa se novo criado)
 *   sigla         - Sigla do tribunal (ex: "SP", "RJ")
 * 
 * RETORNO: Índice do tribunal ou -1 se erro
 * 
 * LÓGICA:
 *   1. Busca linear por sigla existente (n ≤ 64, aceitável)
 *   2. Se encontra, retorna índice
 *   3. Se não encontra:
 *      - Valida se há espaço (< MAX_TRIBUNAIS)
 *      - Cria novo dengan inicialização com zeros
 *      - Copia sigla
 *      - Incrementa contador
 *      - Retorna índice novo
 * 
 * MOTIVAÇÃO: Consolidação automática - programa descobre novos tribunais
 */
static int encontrarOuCriarTribunal(AcumuladoTribunal tribunais[], int *qtdTribunais, const char *sigla) {
    // Busca linear: tribunal já existe?
    for (int i = 0; i < *qtdTribunais; i++) {
        if (strcmp(tribunais[i].sigla, sigla) == 0) {
            return i;  // Encontrado
        }
    }

    // Não encontrado - criar novo?
    if (*qtdTribunais >= MAX_TRIBUNAIS) {
        return -1;  // Erro: limite atingido
    }

    int novoIndice = *qtdTribunais;
    memset(&tribunais[novoIndice], 0, sizeof(AcumuladoTribunal));  // Zera tudo
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

int concatenarArquivosCSV(const char *const *arquivos, int numArquivos) {
    FILE *entrada, *saida;
    char buffer[40000];
    char linhaConvertida[40000];
    int primeiroArquivo = 1;

    // OPÇÃO 1: Concatenar todos os 27 arquivos CSV
    // 
    // PROPÓSITO: Consolidar dados de todos os estados em um único arquivo
    // 
    // ALGORITMO:
    //   1. Abre arquivo de saída para escrita
    //   2. Para cada arquivo de entrada:
    //      a. Abre arquivo (tenta caminho atual e pai)
    //      b. Se não é o primeiro, pula cabeçalho para evitar duplicação
    //      c. Copia todas as linhas normalizando separador
    //      d. Fecha arquivo
    //   3. Fecha arquivo de saída
    // 
    // SAÍDA: baseDeDadosTotal.csv (consolidado)
    // 
    // NOTA: Este é um pré-requisito para as opções 2 e 3

    saida = fopen("baseDeDadosTotal.csv", "w");
    if (saida == NULL) {
        perror("Erro ao criar arquivo de saída");
        return 0;
    }

    // Loop por cada arquivo de entrada (27 arquivos)
    for (int i = 0; i < numArquivos; i++) {
        char caminhoArquivo[256];
        entrada = abrirArquivoComPrefixos(arquivos[i], "r", caminhoArquivo, sizeof(caminhoArquivo));
        if (entrada == NULL) {
            perror("Erro ao abrir arquivo de entrada");
            continue;  // Continua com próximo arquivo
        }

        // Se não é o primeiro arquivo, pula o cabeçalho
        // (para evitar múltiplas linhas de cabeçalho no arquivo final)
        if (!primeiroArquivo) {
            fgets(buffer, sizeof(buffer), entrada);  // Descarta cabeçalho
        }

        // Copia linha por linha, convertendo separadores
        while (fgets(buffer, sizeof(buffer), entrada)) {
            converterSeparadorLinha(buffer, linhaConvertida, sizeof(linhaConvertida));
            fprintf(saida, "%s", linhaConvertida);
        }

        primeiroArquivo = 0;
        fclose(entrada);
    }

    fclose(saida);
    printf("Arquivos concatenados com sucesso em 'baseDeDadosTotal.csv'.\n");
    return 1;
}

void gerarResumoCSV(const char *const *arquivos, int numArquivos) {
    FILE *entrada, *saida;
    char linha[40000];
    AcumuladoTribunal tribunais[MAX_TRIBUNAIS];
    int qtdTribunais = 0;
    char caminhoArquivo[256];

    // OPÇÃO 2: Gerar Resumo de Metas Judiciais
    // 
    // PROPÓSITO: Consolidar estatísticas por tribunal e calcular metas de performance
    //            (Meta 1: Taxa geral de julgamento)
    //            (Meta 2A: Eficiência em processos com até 2 anos)
    //            (Meta 2Ant: Eficiência em processos antigos)
    //            (Meta 4A: Análise preliminar resolução)
    //            (Meta 4B: Análise profunda resolução)
    // 
    // PRÉ-REQUISITO: Arquivo baseDeDadosTotal.csv (gerado da opção 1)
    // SAÍDA: resumo.csv com cálculos de metas por tribunal
    // 
    // FASES:
    //   1. LEITURA: Acumula dados por tribunal
    //   2. CÁLCULO: Computa 5 metas judiciais para cada tribunal
    //   3. ESCRITA: Salva resumo em resumo.csv

    // MAPEAMENTO DE COLUNAS DO ARQUIVO CSV
    // Indices hardcoded que indicam qual coluna tem qual informação
    enum {
        IDX_SIGLA_TRIBUNAL = 0,          // Coluna 0: Sigla do estado (SP, RJ, etc)
        IDX_CASOS_NOVOS_2026 = 10,       // Coluna 10: Casos novos em 2026
        IDX_JULGADOS_2026 = 11,          // Coluna 11: Casos julgados em 2026
        IDX_SUSPENSOS_2026 = 13,         // Coluna 13: Casos suspensos
        IDX_DESSOBRESTADOS_2026 = 14,    // Coluna 14: Casos dessobrestados
        IDX_DISTM2_A = 16,               // Coluna 16: C. com até 2 anos distribuídos
        IDX_JULGM2_A = 17,               // Coluna 17: C. com até 2 anos julgados
        IDX_SUSPM2_A = 18,               // Coluna 18: C. com até 2 anos suspensos
        IDX_DISTM2_ANT = 20,             // Coluna 20: Casos antigos distribuídos
        IDX_JULGM2_ANT = 21,             // Coluna 21: Casos antigos julgados
        IDX_SUSPM2_ANT = 22,             // Coluna 22: Casos antigos suspensos
        IDX_DESOM2_ANT = 23,             // Coluna 23: Casos antigos dessobrestados
        IDX_DISTM4_A = 25,               // Coluna 25: Análise preliminar distribuída
        IDX_JULGM4_A = 26,               // Coluna 26: Análise preliminar resolvida
        IDX_SUSPM4_A = 27,               // Coluna 27: Análise preliminar suspensa
        IDX_DISTM4_B = 29,               // Coluna 29: Análise profunda distribuída
        IDX_JULGM4_B = 30,               // Coluna 30: Análise profunda resolvida
        IDX_SUSPM4_B = 31                // Coluna 31: Análise profunda suspensa
    };

    (void)arquivos;      // Parâmetro não usado (lê de baseDeDadosTotal.csv)
    (void)numArquivos;   // Parâmetro não usado

    // Inicializa array de tribunais com zeros
    memset(tribunais, 0, sizeof(tribunais));

    // === FASE 1: LEITURA E ACUMULAÇÃO ===
    
    // Abre arquivo consolidado
    entrada = abrirArquivoComPrefixos("baseDeDadosTotal.csv", "r", caminhoArquivo, sizeof(caminhoArquivo));
    if (entrada == NULL) {
        perror("Erro ao abrir 'baseDeDadosTotal.csv'. Gere o arquivo na opcao 1 antes do resumo");
        return;
    }

    // Abre arquivo de saída (resumo)
    saida = fopen("resumo.csv", "w");
    if (saida == NULL) {
        perror("Erro ao criar arquivo de resumo");
        fclose(entrada);
        return;
    }

    // Pula cabeçalho do arquivo de entrada
    if (fgets(linha, sizeof(linha), entrada) != NULL) {
        // Processa cada linha de dados
        while (fgets(linha, sizeof(linha), entrada)) {
            // Extrai campos CSV
            char campos[MAX_CAMPOS_CSV][TAM_CAMPO_CSV];
            int qtdCampos = extrairCamposCSV(linha, campos, MAX_CAMPOS_CSV);
            
            // Valida se tem campos suficientes e se sigla não está vazia
            if (qtdCampos <= IDX_SUSPM4_B || campos[IDX_SIGLA_TRIBUNAL][0] == '\0') {
                continue;  // Pula linha inválida
            }

            // Encontra ou cria tribunal pela sigla
            int indiceTribunal = encontrarOuCriarTribunal(tribunais, &qtdTribunais, campos[IDX_SIGLA_TRIBUNAL]);
            if (indiceTribunal < 0) {
                continue;  // Erro ao criar tribunal (limite atingido)
            }

            AcumuladoTribunal *t = &tribunais[indiceTribunal];

            // Acumula estatísticas gerais
            t->casos_novos_2026 += converterNumero(campos[IDX_CASOS_NOVOS_2026]);
            t->julgados_2026 += converterNumero(campos[IDX_JULGADOS_2026]);
            t->suspensos_2026 += converterNumero(campos[IDX_SUSPENSOS_2026]);
            t->dessobrestados_2026 += converterNumero(campos[IDX_DESSOBRESTADOS_2026]);

            // Acumula estatísticas de processados (até 2 anos)
            t->distm2_a += converterNumero(campos[IDX_DISTM2_A]);
            t->julgm2_a += converterNumero(campos[IDX_JULGM2_A]);
            t->suspm2_a += converterNumero(campos[IDX_SUSPM2_A]);

            // Acumula estatísticas de antigos (>2 anos)
            t->distm2_ant += converterNumero(campos[IDX_DISTM2_ANT]);
            t->julgm2_ant += converterNumero(campos[IDX_JULGM2_ANT]);
            t->suspm2_ant += converterNumero(campos[IDX_SUSPM2_ANT]);
            t->desom2_ant += converterNumero(campos[IDX_DESOM2_ANT]);

            // Acumula análises preliminares
            t->distm4_a += converterNumero(campos[IDX_DISTM4_A]);
            t->julgm4_a += converterNumero(campos[IDX_JULGM4_A]);
            t->suspm4_a += converterNumero(campos[IDX_SUSPM4_A]);

            // Acumula análises profundas
            t->distm4_b += converterNumero(campos[IDX_DISTM4_B]);
            t->julgm4_b += converterNumero(campos[IDX_JULGM4_B]);
            t->suspm4_b += converterNumero(campos[IDX_SUSPM4_B]);
        }
    }

    fclose(entrada);

    // === FASE 2: CÁLCULO DE METAS ===

    // Escreve cabeçalho do arquivo de resumo
    fprintf(saida, "sigla_tribunal;total_julgados_2026;Meta1;Meta2A;Meta2Ant;Meta4A;Meta4B\n");

    // Para cada tribunal consolidado
    for (int i = 0; i < qtdTribunais; i++) {
        AcumuladoTribunal *t = &tribunais[i];

        // CÁLCULO DAS 5 METAS

        // Meta 1: Eficiência geral
        // Percentual de casos julgados em relação aos casos processáveis
        double meta1 = calcularPercentual(
            t->julgados_2026,
            (t->casos_novos_2026 + t->dessobrestados_2026 - t->suspensos_2026),
            100.0
        );

        // Meta 2A: Eficiência em processos até 2 anos
        // Fator 1000/7 (≈142.857) é especial para esta meta judicial
        double meta2A = calcularPercentual(
            t->julgm2_a,
            (t->distm2_a - t->suspm2_a),
            (1000.0 / 7.0)
        );

        // Meta 2Ant: Eficiência em processos antigos (>2 anos)
        // Importante indicador de eliminação de backlogs
        double meta2Ant = calcularPercentual(
            t->julgm2_ant,
            (t->distm2_ant - t->suspm2_ant - t->desom2_ant),
            100.0
        );

        // Meta 4A: Análise preliminar
        // Casos resolvidos em primeira análise
        double meta4A = calcularPercentual(
            t->julgm4_a,
            (t->distm4_a - t->suspm4_a),
            100.0
        );

        // Meta 4B: Análise profunda
        // Casos que precisam análise más complexa
        double meta4B = calcularPercentual(
            t->julgm4_b,
            (t->distm4_b - t->suspm4_b),
            100.0
        );

        // === FASE 3: ESCRITA ===

        // Escreve linha com: sigla ; total_julgados ; meta1 ; meta2A ; meta2Ant ; meta4A ; meta4B
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
    // Exibe interface interativa do programa
    printf("\n========================================\n");
    printf("       TP01 - Manipulando Arquivos\n");
    printf("========================================\n\n");
    printf("1 - Concatenar todos os arquivos CSV\n");
    printf("2 - Gerar resumo dos arquivos CSV\n");
    printf("3 - Gerar arquivo por municipio\n");
    printf("0 - Sair\n\n");
    printf("Escolha uma opcao: ");
}

/*
 * FUNÇÃO: configurarAcentuacao
 * PROPÓSITO: Configurar encoding UTF-8 para suportar caracteres acentuados
 * 
 * WINDOWS: Usa API Windows para ativar UTF-8 no console
 * LINUX/MAC: Configura locale do sistema para UTF-8
 * 
 * MOTIVAÇÃO: Tribunal Eleitoral tem nomes com acentuação (São Paulo, etc)
 */
void configurarAcentuacao() {
    #ifdef _WIN32
        // Habilita suporte a UTF-8 no console do Windows
        SetConsoleOutputCP(CP_UTF8);   // Code page saída
        SetConsoleCP(CP_UTF8);         // Code page entrada

        // Configura locale para UTF-8
        setlocale(LC_ALL, ".UTF8");
    #else
        // Para Linux/Mac, configura locale padrão do sistema
        // (geralmente já é UTF-8)
        setlocale(LC_ALL, "");
    #endif
}

void gerarCSVPorMunicipio(void) {
    // OPÇÃO 3: Filtrar arquivo por município
    // 
    // PROPÓSITO: Extrair registros de um município específico
    //            permitindo análises focadas por localidade
    // 
    // PRÉ-REQUISITO: Arquivo baseDeDadosTotal.csv (gerado da opção 1)
    // 
    // ALGORITMO:
    //   1. Solicita nome do município ao usuário
    //   2. Normaliza (maiúsculas, trim espaços)
    //   3. Lê baseDeDadosTotal.csv
    //   4. Para cada linha:
    //      - Extrai município (coluna 5)
    //      - Se match com busca, escreve em arquivo novo
    //   5. Reporta quantos registros foram encontrados
    // 
    // SAÍDA: [MUNICIPIO].csv (ex: MACAPA.csv)

    char municipio[64];
    int totalOcorrencias = 0;
    
    // Solicita nome do município
    printf("Digite o nome do municipio (ex: MACAPA): ");
    if (fgets(municipio, sizeof(municipio), stdin) == NULL) {
        return;
    }
    
    // Remove quebra de linha
    municipio[strcspn(municipio, "\r\n")] = 0;
    
    // Normaliza para maiúsculas e remove espaços
    normalizarTexto(municipio);

    if (municipio[0] == '\0') {
        printf("Municipio invalido.\n");
        return;
    }

    // Abre arquivo consolidado para leitura
    FILE *entrada = fopen("baseDeDadosTotal.csv", "r");
    if (entrada == NULL) {
        perror("Erro ao abrir baseDeDadosTotal.csv");
        return;
    }

    // Monta nome do arquivo de saída
    char nomeArquivo[128];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.csv", municipio);
    FILE *saida = NULL;

    char linha[40000];
    char cabecalho[40000];

    // Lê cabeçalho (será copiado quando encontrar primeiro registro)
    if (fgets(cabecalho, sizeof(cabecalho), entrada) == NULL) {
        printf("Arquivo baseDeDadosTotal.csv vazio.\n");
        fclose(entrada);
        return;
    }

    // Processa cada linha do arquivo
    while (fgets(linha, sizeof(linha), entrada)) {
        // Extrai campos da linha CSV
        char campos[MAX_CAMPOS_CSV][TAM_CAMPO_CSV];
        int qtdCampos = extrairCamposCSV(linha, campos, MAX_CAMPOS_CSV);

        // Coluna 5 contém o nome do município
        int municipio_idx = 5;

        // Normaliza texto do município na linha (maiúsculas, trim)
        if (qtdCampos > municipio_idx) {
            normalizarTexto(campos[municipio_idx]);
        }

        // Compara com busca do usuário
        if (qtdCampos > municipio_idx && strcmp(campos[municipio_idx], municipio) == 0) {
            // Match encontrado!
            
            if (saida == NULL) {
                // Primeira ocorrência - criar arquivo e escrever cabeçalho
                saida = fopen(nomeArquivo, "w");
                if (saida == NULL) {
                    perror("Erro ao criar arquivo de municipio");
                    fclose(entrada);
                    return;
                }

                fprintf(saida, "%s", cabecalho);
            }

            // Escreve linha no arquivo de saída
            fprintf(saida, "%s", linha);
            totalOcorrencias++;
        }
    }

    fclose(entrada);

    // Reporta resultado
    if (saida != NULL) {
        fclose(saida);
        printf("Arquivo '%s' gerado com sucesso.\n", nomeArquivo);
        printf("Total de ocorrencias encontradas para '%s': %d\n", municipio, totalOcorrencias);
    } else {
        printf("Nenhum registro encontrado para o municipio informado.\n");
        printf("O arquivo '%s' nao foi criado.\n", nomeArquivo);
    }
}