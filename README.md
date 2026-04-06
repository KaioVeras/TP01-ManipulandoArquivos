# TP01 - Manipulando Arquivos

## 📋 Visão Geral

Sistema em linguagem C para processamento e análise de dados eleitorais em formato CSV. O programa manipula dados de 27 Tribunais Regionais Eleitorais (TRE) do Brasil, consolidando informações e gerando relatórios com análises comparativas entre tribunais.

## 🎯 Objetivos

Este trabalho prático (TP) tem como objetivo explorar e consolidar conhecimentos em:
- Manipulação de arquivos (leitura e escrita)
- Processamento de dados em formato CSV
- Estruturas de dados em C
- Armazenamento e cálculo de informações em memória
- Tratamento de separadores diferentes (vírgula e ponto-e-vírgula)

## ✨ Funcionalidades

### 1. **Concatenar Arquivos CSV** (Opção 1)
Consolida os 27 arquivos CSV de entrada (um para cada estado) em um único arquivo chamado `baseDeDadosTotal.csv`.

**Características:**
- Combina dados de todos os estados brasileiros
- Normaliza separadores (converte ponto-e-vírgula para vírgula)
- Evita duplicação de cabeçalhos
- Tratamento multiplataforma (Windows/Linux/Mac)

### 2. **Gerar Resumo** (Opção 2)
Cria um relatório consolidado com estatísticas por tribunal (`resumo.csv`).

**Cálculos realizados:**
- **Meta 1**: Percentual de casos julgados em relação aos novos (com ajustes)
- **Meta 2A**: Casos julgados de processos com até 2 anos (com fator 1000/7)
- **Meta 2Ant**: Casos julgados de processos antigos (>2 anos)
- **Meta 4A**: Processos de análise preliminar resolvidos
- **Meta 4B**: Processos em análise profunda resolvidos

### 3. **Filtrar por Município** (Opção 3)
Gera um arquivo CSV contendo apenas os registros de um município específico.

**Características:**
- Busca case-insensitive
- Remove espaçamento desnecessário
- Mantém estrutura original do cabeçalho
- Informa total de registros encontrados

## 📁 Estrutura de Arquivos

```
├── main.c                          # Arquivo principal (menu e controle)
├── lista.h                         # Definições de estruturas e funções
├── lista.c                         # Implementação das funções
├── teste_TRE-*.csv                 # 27 arquivos de entrada (um por estado)
├── baseDeDadosTotal.csv            # Saída: arquivo concatenado
├── resumo.csv                      # Saída: resumo por tribunal
└── [MUNICIPIO].csv                 # Saída: dados filtrados por município
```

## 🔧 Estrutura de Dados

### Struct `AcumuladoTribunal`
Armazena estatísticas consolidadas de cada tribunal:

```c
typedef struct {
    char sigla[32];              // Sigla do estado (ex: "SP", "RJ")
    double casos_novos_2026;     // Total de casos novos em 2026
    double julgados_2026;        // Total de casos julgados em 2026
    double suspensos_2026;       // Total de casos suspensos
    double dessobrestados_2026;  // Casos dessobrestados
    double distm2_a;             // Casos com até 2 anos distribuídos
    double julgm2_a;             // Casos com até 2 anos julgados
    double suspm2_a;             // Casos com até 2 anos suspensos
    double distm2_ant;           // Casos antigos distribuídos
    double julgm2_ant;           // Casos antigos julgados
    double suspm2_ant;           // Casos antigos suspensos
    double desom2_ant;           // Casos antigos dessobrestados
    double distm4_a;             // Análise preliminar distribuída
    double julgm4_a;             // Análise preliminar resolvida
    double suspm4_a;             // Análise preliminar suspensa
    double distm4_b;             // Análise profunda distribuída
    double julgm4_b;             // Análise profunda resolvida
    double suspm4_b;             // Análise profunda suspensa
} AcumuladoTribunal;
```

## 💻 Como Compilar e Executar

### Compilação
```bash
gcc -o main main.c lista.c
```

### Execução
```bash
./main
```

### Menu Interativo
```
========================================
       TP01 - Manipulando Arquivos
========================================

1 - Concatenar todos os arquivos CSV
2 - Gerar resumo dos arquivos CSV
3 - Gerar arquivo por municipio
0 - Sair

Escolha uma opcao:
```

## 🌍 Compatibilidade

- **Windows**: Suporte a UTF-8 e limpeza de tela
- **Linux/macOS**: Limpeza de tela e encoding UTF-8
- **Separadores**: Suporta tanto vírgula quanto ponto-e-vírgula

## 📝 Formato dos Arquivos CSV

### Entrada (teste_TRE-*.csv)
```
sigla_tribunal,municipio,mes_ano,...,casos_novos_2026,julgados_2026,...
```

### Saída - resumo.csv
```
sigla_tribunal;total_julgados_2026;Meta1;Meta2A;Meta2Ant;Meta4A;Meta4B
SP;50000;95.5;88.2;72.1;91.3;85.6
RJ;45000;92.3;85.1;68.9;88.2;82.1
```

## 🔄 Fluxo de Execução Recomendado

1. Execute a **Opção 1** primeiro (concatena os arquivos)
2. Execute a **Opção 2** para análise geral (gera resumo com metas)
3. Execute a **Opção 3** para consultas específicas por município
4. Consulte os arquivos gerados com ferramentas de planilha ou terminal

```bash
cat baseDeDadosTotal.csv | head -5      # Ver primeiras linhas
cat resumo.csv | column -t -s ";"       # Ver resumo formatado
cat MUNICIPIO.csv                        # Ver dados de município específico
```

## 👥 Autores

Trabalho Prático realizado em 2026 para curso de Programação C.

---

**Dúvidas?** Consulte a documentação técnica em `DOCUMENTACAO.md`.
