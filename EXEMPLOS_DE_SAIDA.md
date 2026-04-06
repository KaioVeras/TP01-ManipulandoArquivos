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
