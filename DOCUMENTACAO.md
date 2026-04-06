# DOCUMENTAÇÃO TÉCNICA - TP01 Manipulando Arquivos

## 📚 Índice
1. [Visão Geral Técnica](#visão-geral-técnica)
2. [Estruturas de Dados](#estruturas-de-dados)
3. [Funções Principais](#funções-principais)
4. [Fluxo de Dados](#fluxo-de-dados)
5. [Algoritmos Importantes](#algoritmos-importantes)
6. [Tratamentos de Erros](#tratamentos-de-erros)
7. [Considerações de Memória](#considerações-de-memória)

---

## 🏗️ Visão Geral Técnica

### Propósito
Desenvolver um sistema de processamento de dados eleitorais que:
- Lê dados de múltiplos arquivos CSV
- Normaliza e consolida informações
- Calcula métricas de desempenho judicial
- Permite consultas filtradas por município

### Linguagem: C (ANSI C99)
### Plataformas: Windows, Linux, macOS
### Formato de dados: CSV (Comma Separated Values)

---

## 🔑 Estruturas de Dados

### 1. Struct AcumuladoTribunal
```c
typedef struct {
    char sigla[32];              // Identificador do tribunal (2-3 caracteres)
    double casos_novos_2026;     // Acumulador de casos novos
    double julgados_2026;        // Acumulador de casos julgados
    double suspensos_2026;       // Acumulador de casos suspensos
    
    // ... mais 14 campos de acumuladores ...
} AcumuladoTribunal;
```

**Objetivo**: Armazenar dados consolidados de um tribunal específico.
**Capacidade**: Máximo 64 tribunais em memória (definido por MAX_TRIBUNAIS).

### 2. Constantes de Configuração
```c
#define MAX_CAMPOS_CSV 64       // Máximo de colunas em cada linha CSV
#define TAM_CAMPO_CSV 256       // Tamanho máximo de cada célula
#define MAX_TRIBUNAIS 64        // Máximo de tribunais em memória
```

---

## ⚙️ Funções Principais

### main.c

#### `int main(void)`
**Responsabilidade**: Menu interativo e controle de fluxo
**Fluxo**:
1. Limpa tela (compatível com SO)
2. Configura codificação para acentuação
3. Inicializa array com 27 arquivos CSV (estados)
4. Loop interativo apresentando menu
5. Despacha para funções baseado em opção

**Variáveis principais**:
- `arquivos[]`: Array com nomes dos 27 arquivos
- `numArquivos`: Total de arquivos (27)
- `opcao`: Escolha do usuário

**Tratamentos**:
- Verifica existência de `baseDeDadosTotal.csv` antes de opções 2 e 3
- Avisa se arquivo pré-requisito não existe

---

### lista.c

#### Funções de I/O

##### `FILE* abrirArquivoComPrefixos(const char *arquivo, const char *modo, char *caminhoResolvido, size_t tamanhoCaminho)`

**Objetivo**: Encontrar e abrir arquivo testando múltiplos prefixos de caminho
**Lógica**:
- Tenta abrir arquivo no diretório atual
- Se falhar, tenta no diretório pai (`../`)
- Retorna NULL se todas as tentativas falharem

**Parâmetros**:
- `arquivo`: Nome do arquivo
- `modo`: Modo de abertura ("r", "w", etc)
- `caminhoResolvido`: Saída com caminho que funcionou
- `tamanhoCaminho`: Tamanho do buffer para caminho

**Retorno**: Arquivo aberto ou NULL

**Uso**: Torna o programa robusto a diferentes estruturas de diretório

---

##### `int arquivoBaseConcatenadoExiste(void)`

**Objetivo**: Verificar se arquivo consolidado já foi gerado
**Testa**: 
- `baseDeDadosTotal.csv` (diretório atual)
- `../baseDeDadosTotal.csv` (diretório pai)

**Retorno**: 1 se existe, 0 se não existe

---

#### Funções de Processamento de Dados

##### `static double converterNumero(const char *texto)`

**Objetivo**: Converter string para número (double)
**Lógica**:
- Se string NULL ou vazia → retorna 0.0
- Usar `strtod()` para conversão segura

**Vantagem**: Evita erros ao somar campos vazios

---

##### `static double calcularPercentual(double numerador, double denominador, double fator)`

**Objetivo**: Calcular percentuais com proteção contra divisão por zero
**Fórmula**: `(numerador / denominador) * fator`
**Proteção**: Se denominador = 0 → retorna 0.0

**Exemplos de uso**:
```c
// Meta 1: percentual simples (fator = 100)
meta1 = calcularPercentual(julgados, total_casos, 100.0);

// Meta 2A: com fator especial 1000/7 ≈ 142.857
meta2A = calcularPercentual(julgm2_a, distm2_a - suspm2_a, 1000.0/7.0);
```

---

#### Funções de Parsing CSV

##### `static int extrairCamposCSV(const char *linha, char campos[][TAM_CAMPO_CSV], int maxCampos)`

**Objetivo**: Parsear uma linha CSV respeitando campos entre aspas
**Algoritmo**:
1. Itera por cada caractere da linha
2. Controla estado `emAspas` (dentro ou fora de aspas duplas)
3. Quando encontra separador (`,` ou `;`) FORA de aspas:
   - Finaliza campo atual
   - Avança para próximo
4. Pula caracteres `\r` e `\n`

**Tratamento de aspas**:
```
Linha: "Campos, "com, vírgula",meio"
Resultado: ["Campos"; " com, vírgula"; "meio"]
```

**Retorno**: Número total de campos extraídos

---

##### `static void converterSeparadorLinha(const char *origem, char *destino, size_t tamanhoDestino)`

**Objetivo**: Converter separador ponto-e-vírgula para vírgula
**Preserva**: Separadores dentro de aspas (não converte)
**Uso**: Normalizar múltiplos formatos CSV

---

#### Funções de Texto

##### `static void normalizarTexto(char *texto)`

**Objetivo**: Padronizar texto para comparações
**Processamento**:
1. Remove espaços no início
2. Remove espaços no final
3. Converte para MAIÚSCULAS
4. In-place (modifica string original)

**Motivo**: Comparações case-insensitive e robustas

---

##### `static int encontrarOuCriarTribunal(AcumuladoTribunal tribunais[], int *qtdTribunais, const char *sigla)`

**Objetivo**: Localizar tribunal ou criar novo registro
**Algoritmo**:
1. Busca linear por sigla existente
2. Se encontra → retorna índice
3. Se não encontra:
   - Valida se há espaço (< MAX_TRIBUNAIS)
   - Cria novo tribunal com sigla
   - Inicializa todos os campos com 0
   - Incrementa contador

**Retorno**: Índice do tribunal ou -1 se erro

---

#### Função 1: Concatenação

##### `int concatenarArquivosCSV(const char *const *arquivos, int numArquivos)`

**Objetivo**: Mesclar 27 arquivos em um único arquivo
**Algoritmo**:
```
1. Abre arquivo de saída "baseDeDadosTotal.csv" para escrita
2. Para cada arquivo de entrada:
   a. Abre arquivo
   b. Se NÃO é primeiro arquivo, pula o cabeçalho (fgets)
   c. Copia todas as linhas normalizado o separador
   d. Fecha arquivo
3. Fecha arquivo de saída
```

**Processamento de separadores**:
- Cada linha passa por `converterSeparadorLinha`
- Converte `;` para `,` (exceto dentro de aspas)

**Resultado**: Arquivo com todos os dados consolidados

---

#### Função 2: Gerar Resumo

##### `void gerarResumoCSV(const char *const *arquivos, int numArquivos)`

**Objetivo**: Calcular estatísticas por tribunal e determinar cumprimento de metas

**Índices de Colunas**:
```c
IDX_SIGLA_TRIBUNAL = 0        // Coluna do estado
IDX_CASOS_NOVOS_2026 = 10     // Coluna de casos novos
IDX_JULGADOS_2026 = 11        // Coluna de julgados
// ... (15 índices no total) ...
```

**Algoritmo em 3 fases**:

**Fase 1: Leitura e Acumulação**
```
1. Abre baseDeDadosTotal.csv
2. Para cada linha (exceto cabeçalho):
   a. Extrai campos da linha CSV
   b. Encontra ou cria tribunal pela sigla
   c. Adiciona valores aos acumuladores do tribunal
```

**Fase 2: Cálculo de Metas**
Para cada tribunal, calcula:
- **Meta 1**: `(julgados_2026 / (casos_novos + dessobrestados - suspensos)) × 100`
- **Meta 2A**: `(julgm2_a / (distm2_a - suspm2_a)) × (1000/7)`
- **Meta 2Ant**: `(julgm2_ant / (distm2_ant - suspm2_ant - desom2_ant)) × 100`
- **Meta 4A**: `(julgm4_a / (distm4_a - suspm4_a)) × 100`
- **Meta 4B**: `(julgm4_b / (distm4_b - suspm4_b)) × 100`

**Fase 3: Escrita**
```
Escreve em resumo.csv:
sigla_tribunal;total_julgados_2026;Meta1;Meta2A;Meta2Ant;Meta4A;Meta4B
SP;50000;95.621533;88.235714;72.145689;91.342857;85.632145
RJ;45000;92.354231;85.124578;68.987231;88.234231;82.123456
```

**Significado das Metas**:
- Meta 1: Capacidade geral de julgamento
- Meta 2A: Eficiência em processos novos
- Meta 2Ant: Eficiência em processos antigos (backlogs)
- Meta 4A: Análise preliminar
- Meta 4B: Análise profunda/especializada

---

#### Função 3: Filtrar por Município

##### `void gerarCSVPorMunicipio(void)`

**Objetivo**: Extrair registros de um município específico

**Algoritmo**:
1. Solicita nome do município ao usuário
2. Normaliza texto digitado (maiúsculas, trim)
3. Abre `baseDeDadosTotal.csv` para leitura
4. Lê cabeçalho
5. Para cada linha:
   a. Extrai campo de município (coluna 5)
   b. Normaliza campo lido
   c. Se matches nome digitado:
      - Cria arquivo se primeira ocorrência
      - Escreve cabeçalho
      - Escreve linha
      - Incrementa contador
6. Reporta resultado ao usuário

**Saída**: Arquivo `[MUNICIPIO].csv` com apenas registros daquele município

---

#### Função Menu

##### `void mostrarMenu(void)`
Exibe interface de linha de comando

---

#### Configuração de Acentuação

##### `void configurarAcentuacao(void)`

**Windows**:
- Configura code page para UTF-8 (CP_UTF8)
- Define locale para UTF-8

**Linux/macOS**:
- Define locale padrão do sistema

**Propósito**: Permitir caracteres acentuados na saída

---

#### Utilitários

##### `void aguardarEnter(void)`
Pausa execução até usuário pressionar ENTER

---

## 📊 Fluxo de Dados

```
┌─────────────────────────────────────────────┐
│   27 Arquivos CSV (teste_TRE-*.csv)         │
│   (um por estado)                           │
└────────────────────┬────────────────────────┘
                     │ Opção 1: Concatenar
                     ↓
┌─────────────────────────────────────────────┐
│   baseDeDadosTotal.csv                      │
│   (todos os dados consolidados)             │
└─────────────────────────────────────────────┘
         ↓                                  ↓
    Opção 2                            Opção 3
    Resumo                          Filtro por
                                    Município
         ↓                                  ↓
┌──────────────────┐          ┌──────────────────────┐
│   resumo.csv     │          │  [MUNICIPIO].csv     │
│   (metas por TRE)│          │  (dados filtrados)   │
└──────────────────┘          └──────────────────────┘
```

---

## 🧮 Algoritmos Importantes

### Parsing CSV Robusto
O parser respeita campos entre aspas, essencial quando dados contêm separadores:

```
"Silva, José";"São Paulo";"Rua, Av"
→ Detecta 3 campos, não 5
```

### Acumulação com Struct
Uso de array de structs para consolidar dados:
- Máximo 64 tribunais
- Busca por sigla O(n) - aceitável pois n ≤ 64
- Inicialização de novos com zeros

### Proteção contra Divisão por Zero
Todas as divisões verificam denominador:
```c
if (denominador == 0.0) return 0.0;
```

---

## ⚠️ Tratamentos de Erros

| Cenário | Tratamento |
|---------|-----------|
| Arquivo não encontrado | `perror()` + mensagem custom |
| Falha ao criar arquivo | Avisos informativos |
| `baseDeDadosTotal.csv` não existe | Avisos na opção 2 e 3 |
| Campo vazio em número | Converte para 0.0 |
| Separador fora de aspas | Não converte ponto-e-vírgula |
| Linha com poucos campos | Pula linha (continue) |
| Municipio não encontrado | Cria arquivo vazio, informa 0 registros |

---

## 💾 Considerações de Memória

### Alocação Estática
- Array de 64 tribunais: `64 × ~300 bytes = ~19.2 KB`
- Buffers de linha: ~80 KB
- **Total aproximado: <200 KB**

### Alternativas Não Implementadas (Mas Considerações)
- Alocação dinâmica com `malloc/realloc` - complexidade extra
- Banco de dados - overhead desnecessário para TP
- Arquivos temporários - uso de memória está adequado

### Limites Teóricos
- Tamanho máximo de linha CSV: 40.000 caracteres
- Máximo de colunas por linha: 64
- Máximo de tribunais em memória: 64

---

## 🔍 Verificação de Qualidade

### Validações Implementadas
- ✓ Verificação de aberturas de arquivo
- ✓ Controle de separadores dentro de aspas
- ✓ Normalização de texto para buscas
- ✓ Proteção contra divisão por zero
- ✓ Suporte multiplataforma
- ✓ Encoding UTF-8 para acentuação

### Casos Extremos Testáveis
1. Arquivo CSV vazio
2. Linha com campos vazios
3. Dados com vírgulas dentro de aspas
4. Município não existente
5. Arquivo já processado (rodar opção 1 duas vezes)

---

## 📋 Conclusão

O sistema implementa uma solução robusta e eficiente para:
- Consolidação de múltiplas fontes de dados
- Cálculos de performance em tempo real
- Consultas orientadas a usuário
- Portabilidade entre plataformas

Todas as funções incluem tratamento de erros apropriado e o design prioriza clareza e manutenibilidade do código.

---

# EXEMPLOS DE SAÍDA ESPERADA

Referência prática de como o programa deve se comportar em diferentes cenários.

---

## 1. Execução Normal Completa

### Entrada do Usuário
```
1    [Enter para concatenar]
2    [Enter para gerar resumo]
3    [Enter para filtrar por MACAPA]
0    [Enter para sair]
```

### Saída Esperada no Console

```
========================================
   TP01 - Manipulando Arquivos
========================================

1 - Concatenar todos os arquivos CSV
2 - Gerar resumo dos arquivos CSV
3 - Gerar arquivo por municipio
0 - Sair

Escolha uma opcao: 1
Arquivos concatenados com sucesso em 'baseDeDadosTotal.csv'.

Pressione ENTER para continuar...

========================================
   TP01 - Manipulando Arquivos
========================================

1 - Concatenar todos os arquivos CSV
2 - Gerar resumo dos arquivos CSV
3 - Gerar arquivo por municipio
0 - Sair

Escolha uma opcao: 2
Resumo por sigla_tribunal gerado com sucesso em 'resumo.csv' a partir de 'baseDeDadosTotal.csv'.

Pressione ENTER para continuar...

========================================
   TP01 - Manipulando Arquivos
========================================

1 - Concatenar todos os arquivos CSV
2 - Gerar resumo dos arquivos CSV
3 - Gerar arquivo por municipio
0 - Sair

Escolha uma opcao: 3
Digite o nome do municipio (ex: MACAPA): MACAPA
Arquivo 'MACAPA.csv' gerado com sucesso.
Total de ocorrencias encontradas para 'MACAPA': 457

Pressione ENTER para continuar...

========================================
   TP01 - Manipulando Arquivos
========================================

1 - Concatenar todos os arquivos CSV
2 - Gerar resumo dos arquivos CSV
3 - Gerar arquivo por municipio
0 - Sair

Escolha uma opcao: 0
Saindo...
```

---

## 2. Arquivos Gerados

### Arquivo: baseDeDadosTotal.csv (amostra das primeiras linhas)

```
sigla_tribunal,mes_ano,municipio_eleitoral,tipo_de_acervo,data_ini_acc,data_fim_acc,casos_novos_2026,julgados_2026,suspensos_2026,dessobrestados_2026,distm2_a,julgm2_a,suspm2_a,distm2_ant,julgm2_ant,suspm2_ant,desom2_ant,distm4_a,julgm4_a,suspm4_a,distm4_b,julgm4_b,suspm4_b
AP,2026-01,MACAPA,GERAL,2024-01-01,2026-01-31,124,98,15,8,45,38,5,28,22,3,2,12,10,1,15,12,2
AP,2026-01,SANTANA,GERAL,2024-01-01,2026-01-31,87,72,10,5,32,28,2,18,14,2,1,8,6,0,10,8,1
AP,2026-01,LARANJAL,GERAL,2024-01-01,2026-01-31,65,54,8,3,24,21,2,15,12,1,0,6,5,0,8,7,1
```

### Arquivo: resumo.csv (exemplo de saída)

```
sigla_tribunal;total_julgados_2026;Meta1;Meta2A;Meta2Ant;Meta4A;Meta4B
AC;12450;85.234231;125.456789;98.765432;91.234567;87.654321
AL;15680;88.456789;132.123456;102.345678;94.567890;89.876543
AM;11230;82.345678;118.765432;95.432109;88.123456;85.234567
AP;9845;79.876543;115.234567;92.345678;85.678901;82.456789
BA;34561;92.567890;145.678901;108.456789;98.765432;94.567890
... (27 linhas total, uma por estado)
```

### Arquivo: MACAPA.csv (amostra do filtro)

```
sigla_tribunal,mes_ano,municipio_eleitoral,tipo_de_acervo,data_ini_acc,data_fim_acc,casos_novos_2026,julgados_2026,suspensos_2026,dessobrestados_2026,distm2_a,julgm2_a,suspm2_a,distm2_ant,julgm2_ant,suspm2_ant,desom2_ant,distm4_a,julgm4_a,suspm4_a,distm4_b,julgm4_b,suspm4_b
AP,2026-01,MACAPA,GERAL,2024-01-01,2026-01-31,124,98,15,8,45,38,5,28,22,3,2,12,10,1,15,12,2
AP,2026-02,MACAPA,GERAL,2024-02-01,2026-02-28,128,102,14,7,46,39,4,29,23,2,1,13,11,1,16,13,2
AP,2026-03,MACAPA,GERAL,2024-03-01,2026-03-31,132,105,12,6,48,41,3,30,25,2,0,14,12,1,17,14,2
... (457 linhas total para MACAPA)
```

---

## 3. Casos de Erro Esperados

### Caso: Opção 2 sem executar Opção 1

```
Escolha uma opcao: 2
Arquivo 'baseDeDadosTotal.csv' nao encontrado.
E necessario concatenar os arquivos primeiro (opcao 1).
Nenhum arquivo de resumo foi gerado.

Pressione ENTER para continuar...
```

### Caso: Opção 3 sem executar Opção 1

```
Escolha uma opcao: 3
Arquivo 'baseDeDadosTotal.csv' nao encontrado.
E necessario concatenar os arquivos primeiro (opcao 1).
Nenhum arquivo de municipio foi gerado.

Pressione ENTER para continuar...
```

### Caso: Opção 3 com município inexistente

```
Escolha uma opcao: 3
Digite o nome do municipio (ex: MACAPA): CIDADE_QUE_NAO_EXISTE
Nenhum registro encontrado para o municipio informado.
O arquivo 'CIDADE_QUE_NAO_EXISTE.csv' nao foi criado.

Pressione ENTER para continuar...
```

### Caso: Opção 3 com entrada vazia

```
Escolha uma opcao: 3
Digite o nome do municipio (ex: MACAPA): [Enter sem digitar]
Municipio invalido.

Pressione ENTER para continuar...
```

### Caso: Opção inválida

```
Escolha uma opcao: 99
Opcao invalida. Tente novamente.

Pressione ENTER para continuar...
```

### Caso: Arquivo corrompido (perda de arquivo de entrada)

```
Escolha uma opcao: 1
Erro ao abrir arquivo de entrada: No such file or directory
Arquivos concatenados com sucesso em 'baseDeDadosTotal.csv'.
```

(Continua mesmo que um arquivo individual falhe)

---

## 4. Comportamentos com Normalização de Texto

### Entrada do Usuário → Resultado

```
Usuário digita: "macapa"        → Busca por "MACAPA"    → ENCONTRADO ✓
Usuário digita: "MACAPA"        → Busca por "MACAPA"    → ENCONTRADO ✓
Usuário digita: "MaCaPa"        → Busca por "MACAPA"    → ENCONTRADO ✓
Usuário digita: "  macapa  "    → Busca por "MACAPA"    → ENCONTRADO ✓
Usuário digita: "são paulo"     → Busca por "SÃO PAULO" → ENCONTRADO ✓
```

---

## 5. Validação de Dados em resumo.csv

### Interpretação dos Valores

```
sigla_tribunal;total_julgados;Meta1;Meta2A;Meta2Ant;Meta4A;Meta4B
SP;50000;95.621538;142.857143;88.234567;91.234567;85.678901

Interpretação:
- SP teve 50.000 casos julgados em 2026
- Meta 1 (geral): 95.62% = ACIMA DA META
- Meta 2A (até 2 anos): 142.86 = MUITO ACIMA DA META
- Meta 2Ant (antigos): 88.23% = ABAIXO DA META (atenção!)
- Meta 4A (análise 1): 91.23% = PRÓXIMO DA META
- Meta 4B (análise 2): 85.68% = ABAIXO DA META
```

---

## 6. Tamanho Esperado dos Arquivos

Assumindo ~10.000 linhas de dados por estado:

```
baseDeDadosTotal.csv  ~  150-200 MB  (27 estados × ~6MB cada)
resumo.csv            ~  1-2 KB      (27 linhas, 6 colunas)
MACAPA.csv            ~  100-500 KB  (tipicamente 300-500 registros)
```

---

## 7. Performance Esperada

Comandos de teste no terminal:

```bash
# Opção 1: Tempo esperado
$ time ./main
> 1
> 0
real    0m1.245s
user    0m0.234s
sys     0m0.987s
# Esperado: 1-3 segundos (dependendo velocidade disco)

# Opção 2: Tempo esperado
$ time ./main
> 2
> 0
real    0m0.567s
user    0m0.456s
sys     0m0.087s
# Esperado: <1 segundo (processamento em memória)

# Opção 3: Tempo esperado
$ time ./main
> 3
> MACAPA
> 0
real    0m0.234s
user    0m0.187s
sys     0m0.043s
# Esperado: <1 segundo (busca linear)
```

---

## 8. Inspeção dos Arquivos Gerados

### Quantas linhas?

```bash
$ wc -l baseDeDadosTotal.csv
270001  baseDeDadosTotal.csv
# Esperado: ~27×10.000 = 270.000 linhas + 1 cabeçalho

$ wc -l resumo.csv
28  resumo.csv
# Esperado: 27 estados + 1 cabeçalho

$ wc -l MACAPA.csv
458  MACAPA.csv
# Esperado: ~450-500 registros + 1 cabeçalho
```

### Primeira linha (cabeçalhos)?

```bash
$ head -1 baseDeDadosTotal.csv
sigla_tribunal,mes_ano,municipio_eleitoral,...

$ head -1 resumo.csv
sigla_tribunal;total_julgados_2026;Meta1;Meta2A;Meta2Ant;Meta4A;Meta4B

$ head -1 MACAPA.csv
sigla_tribunal,mes_ano,municipio_eleitoral,...
```

---

## 9. Verificação de Integridade

### Resumo: Todos os 27 estados presentes?

```bash
$ cut -d';' -f1 resumo.csv | sort | uniq | wc -l
28  # 27 estados + 1 "sigla_tribunal" (cabeçalho)
```

### MACAPA: Todos os registros são de MACAPA?

```bash
$ cut -d',' -f3 MACAPA.csv | sort | uniq
municipio_eleitoral
MACAPA
# ✓ Apenas MACAPA (além do cabeçalho)
```

### Valores de Meta são válidos?

```bash
# Meta deve ser número positivo
$ awk -F';' 'NR>1 {print $3}' resumo.csv | head -5
95.621538
88.456789
82.345678
79.876543
92.567890
# ✓ Todos números decimais válidos
```

---

## 10. Teste de Robustez

### Podemos rodar opção 1 duas vezes?

```bash
$ ./main          # Primeira vez
Escolha uma opcao: 1
Arquivos concatenados com sucesso...

$ ./main          # Segunda vez
Escolha uma opcao: 1
Arquivos concatenados com sucesso...
# ✓ Funciona, sobrescreve arquivo anterior
```

### Podemos rodar opção 2 várias vezes?

```bash
# Primeira execução
$ ./main
Escolha uma opcao: 2
Resumo... gerado com sucesso...

# Segunda execução (mesmo arquivo de entrada)
$ ./main
Escolha uma opcao: 2
Resumo... gerado com sucesso...
# ✓ Funciona, resumo é atualizado (ou idêntico)
```

---

## 11. Comparação de Meta por Tribunal

### Qual tribunal melhor em Meta 1?

```bash
$ sort -t';' -k3 -nr resumo.csv | head -3
SP;50000;95.621538;142.857143;88.234567;91.234567;85.678901
RJ;45000;94.234567;139.876543;87.123456;90.567890;84.876543
MG;42000;93.876543;138.765432;86.765432;89.234567;83.765432
# SP é o melhor em Meta 1
```

---

## 12. Diagrama de Fluxo com Saídas

```
┌─ Iniciar
│
├─ Limpar tela → "========================================..."
│
├─ Exibir menu
│
├─ Ler opção
│
├─ Opção = 1?
│  ├─ SIM → Concatenar → "Arquivos concatenados com sucesso..."
│  │                  → baseDeDadosTotal.csv criado
│  │
│  └─ NÃO
│     ├─ Opção = 2?
│     │  ├─ SIM, arquivo existe? 
│     │  │  ├─ SIM → Gerar resumo → "Resumo por sigla... gerado com sucesso..."
│     │  │  │                     → resumo.csv criado
│     │  │  └─ NÃO → Aviso de pré-requisito
│     │  │
│     │  └─ NÃO
│     │     ├─ Opção = 3?
│     │     │  ├─ SIM, arquivo existe?
│     │     │  │  ├─ SIM → Filtrar município → "Arquivo '[...]' gerado com sucesso..."
│     │     │  │  │                          → [MUNICIPIO].csv criado
│     │     │  │  └─ NÃO → Aviso de pré-requisito
│     │     │  │
│     │     │  └─ NÃO
│     │     │     ├─ Opção = 0?
│     │     │     │  ├─ SIM → "Saindo..." → FIM
│     │     │     │  └─ NÃO → "Opção inválida..."
│     │     │     │
│     │     │     └─ Voltar ao menu
```

---

Estes são os comportamentos **esperados** do seu programa quando implementado corretamente!

Compare suas saídas com estes exemplos para validar que tudo está funcionando apropriadamente. 🎯

================================================================================
                         Desenvolvido em 2026
                    Disciplina: Programação C (TP01)
================================================================================