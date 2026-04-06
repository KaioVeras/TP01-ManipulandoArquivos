# FAQ e Q&A - TP01 Manipulando Arquivos

Respostas rápidas para perguntas esperadas do professor/colegas.

---

## ❓ Perguntas Frequentes

### P: Por que o programa usa estruturas estáticas em vez de alocação dinâmica?

**R**: Para este trabalho prático, estruturas estáticas são mais apropriadas porque:

1. **Limite conhecido**: Máximo 27 estados = máximo 64 tribunais
2. **Simplicidade**: Foco fica em manipulação de arquivos, não em gerência de memória
3. **Seg segurança**: Sem risco de vazamento de memória
4. **Debugging**: Mais fácil rastrear valores em memória stack

Um projeto em produção provavelmente usaria `malloc/realloc` ou banco de dados.

---

### P: Como o programa diferencia entre vírgula e ponto-e-vírgula?

**R**: Implementamos dois mecanismos:

1. **Normalização**: Função `converterSeparadorLinha()` transforma `;` em `,`
   - Mas **não converte** se dentro de aspas
   - Exemplo: `"Rua; Av","SP"` → `"Rua; Av","SP"`

2. **Parsing**: `extrairCamposCSV()` aceita ambos como separadores
   - Se encontra `,` or `;` **fora** de aspas → novo campo
   - Se encontra `,` or `;` **dentro** de aspas → parte do conteúdo

Isso torna o programa robusto contra diferentes formatos CSV.

---

### P: O arquivo está muito grande, afeta desempenho?

**R**: Não significativamente porque:

1. **Buffers de 40KB**: Processa linha por linha (streaming)
2. **Não carrega tudo em memória**: Apenas ~20KB de structs
3. **Complexidade O(n)**: Uma passagem por arquivo

Se o arquivo chegasse a **GB**, seria necessário considerar:
- Banco de dados indexado
- Processamento paralelo
- Formato otimizado (Parquet, HDF5)

Para este escopo, performance é adequada.

---

### P: E se o usuário digitar um município que não existe?

**R**: O programa oferece **tratamento gracioso**:

```
Escolha uma opcao: 3
Digite o nome do municipio: MUNICIPIO_INEXISTENTE
Nenhum registro encontrado para o municipio informado.
O arquivo 'MUNICIPIO_INEXISTENTE.csv' nao foi criado.
```

**Vantagens**:
- Não cria arquivo vazio
- Mensagem clara informando 0 resultados
- Não interrompe o programa

---

### P: Como garantir que o programa funcione igual no Windows e Linux?

**R**: Implementamos compilação condicional (`#ifdef _WIN32`):

| Aspecto | Windows | Linux/Mac |
|--------|---------|----------|
| Limpeza de tela | `system("cls")` | `system("clear")` |
| Code page | UTF-8 via SetConsoleCP | Locale padrão |
| Codificação de arquivo | UTF-8 | UTF-8 |
| Paths | `./` e `../` (funciona igual) | `./` e `../` |

**Teste em ambos** os sistemas executando:
```bash
gcc -o main main.c lista.c
./main
```

---

### P: Qual é o tamanho máximo de um campo CSV?

**R**: Definido por `#define TAM_CAMPO_CSV 256`

Isso significa:
- ✓ Aceita campos até 256 caracteres
- ✗ Rejeita (silenciosamente trunca) campos maiores

Para o domínio (dados de tribunais), 256 é mais que suficiente.

Se precisar aumentar:
```c
#define TAM_CAMPO_CSV 1024  // Para campos maiores
```

---

### P: Por que normalizar texto para maiúsculas?

**R**: Permite buscas **case-insensitive** e **tolerantes a espaçamento**:

```
Usuário digita:        Encontra:
"macapa"        ↔      "MACAPÁ"
"  são paulo  " ↔      "SÃO PAULO"
"rj"            ↔      "RJ"
```

Sem normalização, teria de digitar exatamente certo, o que frustra usuários.

---

### P: O que significa o "fator 1000/7" na Meta 2A?

**R**: É uma **exigência do sistema judiciário** (não escolha nossa).

Na fórmula:
```c
meta2A = (julgm2_a / (distm2_a - suspm2_a)) * (1000.0 / 7.0)
```

O fator `1000/7 ≈ 142.857` normaliza o cálculo para processos com até 2 anos, compensando fatores específicos do sistema eleitoral.

Resultado: um número que pode ser:
- < 100: Tribunal abaixo da meta
- = 100: Tribunal na meta
- > 100: Tribunal acima da meta

---

### P: Todos os tribunais têm os mesmos índices de colunas?

**R**: **Sim**, por design. Todos os 27 arquivos entrada seguem o mesmo padrão:

```
Coluna 0:  sigla_tribunal
Coluna 5:  município
Coluna 10: casos_novos_2026
...
```

Se houvesse variação, seria necessário:
- Cabeçalhos com nomes de colunas
- Busca de índice dinâmica
- Mais complexidade

Para este trabalho, foi simplificado com índices fixos.

---

### P: Como o programa sabe que a opção 2 requer a opção 1?

**R**: Validação explícita antes de executar:

```c
if (!arquivoBaseConcatenadoExiste()) {
    printf("Arquivo 'baseDeDadosTotal.csv' nao encontrado.\n");
    printf("E necessario concatenar os arquivos primeiro (opcao 1).\n");
    return;
}
```

Função `arquivoBaseConcatenadoExiste()` testa:
- Arquivo no diretório atual
- Arquivo no diretório pai

Oferece dois caminhos possíveis de flexibilidade.

---

### P: Qual é a complexidade de tempo do algoritmo?

**R**: Análise Big-O:

| Operação | Complexidade | Motivo |
|----------|--------------|--------|
| Concatenar | O(n) | Lê cada linha uma vez |
| Resumo | O(n×m) | n=linhas, m≤64 tribunais (busca linear) |
| Filtro | O(n) | Percorre arquivo uma vez |

Para n = milhões de linhas:
- Concatenar: ~segundos
- Resumo: ~segundos (m é constante pequeno)
- Filtro: ~segundos

Praticamente linear, muito eficiente.

---

### P: Posso criar múltiplas versões do resumo com filtros diferentes?

**R**: Atualmente, não há função para isso. Mas seria fácil implementar:

```c
void gerarResumoCSVFiltrado(const char *sigla) {
    // Gerar resumo apenas para um tribunal específico
    // Necessário: adicionar parâmetro, buscar por sigla
}
```

Isso seria uma **extensão natural** do programa sem alterar o núcleo.

---

### P: Há risco de overflow em algum cálculo?

**R**: **Risco baixo** porque:

1. Usamos `double` (64 bits, ~10^308 máximo)
2. Dados de tribunais são números razoáveis
   - Casos por ano: até ~1 milhão por tribunal
   - 27 tribunais × 1M = 27M ✓ Cabe em double

**Pior caso realista**:
```
total_geral = 27 tribunais × 1M casos = 27M
double máximo ≈ 10^308
Margem: 10^308 >> 27M ✓ Seguro
```

---

## 🎯 Respostas a Críticas Possíveis

### "Por que não validou entrada do usuário para opção do menu?"

**R**: Implementamos:
```c
opcao = (int)strtol(entradaMenu, NULL, 10);  // Converte com segurança

if (opcao < 0 || opcao > 3) {
    printf("Opcao invalida. Tente novamente.\n");
}
```

Aceita qualquer entrada, mas rejeita opções inválidas. Usuário pode tentar novamente.

---

### "Por que não usar fopen com errno para melhor diagnostic?"

**R**: Implementaçãoora atual usa `perror()` que já:
- Imprime mensagem do sistema
- Indica exatamente qual erro ocorreu
- Simples e suficiente para TP

Em produção, log estruturado seria melhor.

---

### "Não deveria há limite de 64 tribunais?"

**R**: Justo. Alternativas:

1. **Alocação dinâmica**: `tribunais = malloc(qtd * sizeof(...))`
   - Mais flexível
   - Mais complexo
   - Fora do escopo de TP

2. **Limite estático + validação**: ✓ Implementado
   - Simples
   - Suficiente (nunca há >27 tribunais)
   - Fácil debugar

Para educação, abordagem atual é apropriada.

---

### "Por que os indexs são hardcoded? Deveria ler do cabeçalho?"

**R**: Tradeoff entre **robustez** e **complexidade** da TP:

```c
// Atual (simples):
IDX_CASOS_NOVOS = 10;
// Lê coluna 10

// Alternativa (robusto):
IDX_CASOS_NOVOS = encontrarIndice(cabecalho, "casos_novos");
// Busca "casos_novos" no cabeçalho
```

Como o professor controla os arquivos de entrada e eles sempre seguem o padrão, a abordagem simplificada é apropriada.

---

## 💡 Dicas Adicionais

### Se o programa não compila:
```bash
# Verifique includes e bibliotecas necessárias
gcc -std=c99 -Wall -Wextra -o main main.c lista.c

# Certifique-se de ter includes:
// No Windows: windows.h
// No Linux/Mac: stdlib.h, string.h é suficiente
```

### Se acentuação não funciona:
```bash
# Windows: Seu console pode não suportar UTF-8
# Tente o PowerShell em vez de cmd.exe

# Linux: Geralmente já é UTF-8
# Se não: export LANG=pt_BR.UTF-8

# Mac: Padrão é UTF-8, deve funcionar
```

### Se arquivo não é encontrado:
```bash
# O programa testa:
# 1. ./baseDeDadosTotal.csv (atual)
# 2. ../baseDeDadosTotal.csv (pai)

# Se ainda assim não encontra:
cd para_o_diretório_correto
./main
```

---

## ✅ Checklist para Apresentação

- [ ] Código compila sem erros ou warnings
- [ ] Todos os 27 arquivos CSV estão presentes
- [ ] Menu exibe corretamente
- [ ] Opção 1 gera baseDeDadosTotal.csv
- [ ] Opção 2 gera resumo.csv com 5 metas
- [ ] Opção 3 gera arquivo por município
- [ ] Busca por município é case-insensitive
- [ ] Acentuação exibe corretamente
- [ ] Falha graciosamente em casos extremos

================================================================================
                         Desenvolvido em 2026
                    Disciplina: Programação C (TP01)
================================================================================