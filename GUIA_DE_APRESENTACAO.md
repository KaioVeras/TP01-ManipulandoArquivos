# GUIA DE APRESENTAÇÃO - TP01 Manipulando Arquivos

## 📊 Para Apresentar ao Professor/Equipe

Use este guia para estruturar sua apresentação sobre o projeto.

---

## 1️⃣ Introdução (2-3 minutos)

### O que é o projeto?
Sistema em C para **processamento e consolidação de dados eleitorais** de 27 Tribunais Regionais Eleitorais brasileiros.

### Objetivo Pedagógico
- ✓ Praticar manipulação de arquivos (leitura/escrita)
- ✓ Trabalhar com dados em formato CSV
- ✓ Implementar estruturas de dados e agregações
- ✓ Criar interface interativa com menu

### Contexto
Dados de Tribunais Regionais Eleitorais contêm estatísticas de julgamentos e precisam ser:
- Consolidados
- Analisados
- Consultados por localização

---

## 2️⃣ Escopo e Funcionalidades (3-4 minutos)

### Diagrama Visual
```
Entrada: 27 CSV (1 por estado)
         ↓
    [OPÇÃO 1]
    Concatenar
         ↓
   baseDeDadosTotal.csv
    /              \
   /                \
[OPÇÃO 2]        [OPÇÃO 3]
Resumo           Filtro
  ↓                ↓
resumo.csv    MUNICIPIO.csv
```

### Funcionalidade 1: Concatenação
- **O que faz**: Mescla 27 arquivos em 1
- **Entrada**: teste_TRE-AC.csv, teste_TRE-AL.csv, ... teste_TRE-TO.csv
- **Saída**: baseDeDadosTotal.csv
- **Detalhe técnico**: Normaliza separadores (`;` → `,`)

### Funcionalidade 2: Resumo com Metas
- **O que faz**: Consolida dados por tribunal e calcula performance
- **Entrada**: baseDeDadosTotal.csv
- **Saída**: resumo.csv
- **Cálculos**: 5 metas judiciais
  - Meta 1: Taxa geral de julgamento (%)
  - Meta 2A: Eficiência em casos até 2 anos (%)
  - Meta 2Ant: Eficiência em casos antigos (%)
  - Meta 4A: Análise preliminar (%)
  - Meta 4B: Análise profunda (%)

### Funcionalidade 3: Filtro por Município
- **O que faz**: Extrai registros de um município
- **Entrada**: baseDeDadosTotal.csv + nome do município (usuário digita)
- **Saída**: [MUNICIPIO].csv
- **Detalhe**: Busca case-insensitive (MACAPA = macapa)

---

## 3️⃣ Arquitetura do Código (3 minutos)

### Estrutura de Arquivos
```
main.c      → Menu interativo + controle de fluxo
lista.h     → Definições de estruturas e protótipos
lista.c     → Implementação de todas as funções
```

### Estrutura de Dados Principal
```c
typedef struct {
    char sigla[32];              // "SP", "RJ", etc
    double casos_novos_2026;     // Acumuladores
    double julgados_2026;        // de dados
    double suspensos_2026;       // por tribunal
    // ... (14 campos adicionais)
} AcumuladoTribunal;
```

### Fluxo de Execução
1. main() exibe menu
2. Usuário escolhe opção (1, 2, 3 ou 0)
3. Função apropriada é chamada
4. Resultado é salvo em arquivo
5. Volta ao menu

---

## 4️⃣ Detalhes Técnicos Importantes (4-5 minutos)

### Parse CSV Robusto
**Problema**: Dados podem ter vírgulas dentro de aspas
```
"Silva, José,,"RJ"  ← Não é 3 campos, é 2!
```

**Solução**: Máquina de estados que rastreia aspas
```
Estado "emAspas" = controla interpretação de separadores
```

### Validações Implementadas
- ✓ Verificação de existência de arquivo antes de usar
- ✓ Proteção contra divisão por zero
- ✓ Validação de quantidade de colunas
- ✓ Tratamento de campos vazios → 0.0

### Portabilidade
- **Windows**: `system("cls")` para limpar tela
- **Linux/Mac**: `system("clear")`
- **Acentuação**: Configurado UTF-8 em ambas plataformas

---

## 5️⃣ Exemplo de Uso Prático (3 minutos)

### Console
```bash
$ gcc -o main main.c lista.c
$ ./main

========================================
       TP01 - Manipulando Arquivos
========================================

1 - Concatenar todos os arquivos CSV
2 - Gerar resumo dos arquivos CSV
3 - Gerar arquivo por municipio
0 - Sair

Escolha uma opcao: 1
Arquivos concatenados com sucesso em 'baseDeDadosTotal.csv'.

[Volta ao menu]
Escolha uma opcao: 2
Resumo por sigla_tribunal gerado com sucesso em 'resumo.csv'

[Volta ao menu]
Escolha uma opcao: 3
Digite o nome do municipio (ex: MACAPA): MACAPA
Arquivo 'MACAPA.csv' gerado com sucesso.
Total de ocorrencias encontradas para 'MACAPA': 457
```

### Visualização de Resultados
```bash
# Ver resumo (pequeno)
$ head resumo.csv

# Ver filtro por município
$ wc -l MACAPA.csv    # Contar linhas
$ head MACAPA.csv     # Ver amostra
```

---

## 6️⃣ Desafios Superados (2 minutos)

| Desafio | Solução |
|---------|---------|
| CSV com separadores dentro de aspas | Parse com rastreamento de aspas |
| Múltiplos formatos (`,` e `;`) | Conversão de separadores |
| Cabeçalhos repetidos | Skip de cabeçalho após 1º arquivo |
| Divisão por zero em cálculos | Validação de denominador |
| Case-sensitive em buscas | Normalização de texto (maiúsculas) |
| Diferentes estruturas de diretório | Prefixos de caminho testados |

---

## 7️⃣ Decisões de Design (2 minutos)

### Por que não usar banco de dados?
- Projeto é educacional (foco em manipulação de arquivos)
- Simples e direto (sem overhead)

### Por que array estático em vez de dinâmico?
- Limite conhecido (27 estados → 64 tribunais máximo)
- Mais fácil de debugar
- Menos alocações de memória

### Por que struct AcumuladoTribunal?
- Agrupa dados relacionados por tribunal
- Facilita cálculos de metas
- Código mais legível e manutenível

---

## 8️⃣ Testes Sugeridos (2 minutos)

Demonstre que o código é robusto:

### Teste 1: Funcionamento Normal
```bash
./main
# Executa opções 1, 2, 3 em sequência
# Verifica arquivos gerados
```

### Teste 2: Validações
```bash
./main
# Opção 2 sem executar opção 1 primeiro
# → Aviso apropriado
```

### Teste 3: Busca Tolerante
```bash
Escolha uma opcao: 3
Digite o nome do municipio: macapa
# Encontra resultados (case-insensitive)
```

### Teste 4: Múltiplas Execuções
```bash
./main
# Roda opção 1 duas vezes
# Verifica se arquivo é sobrescrito corretamente
```

---

## 9️⃣ Conclusão (1 minuto)

### Resumo de Aprendizados
- ✓ Leitura/escrita avançada de arquivos
- ✓ Parsing robusto de dados
- ✓ Agregação de dados em estruturas
- ✓ Cálculos numéricos com validacao
- ✓ HCI com menu interativo

### Possíveis Extensões Futuras
- [ ] Ordenação de resultados (por meta, por tribunal)
- [ ] Estatísticas adicionais (desvio padrão, comparativas)
- [ ] Interface gráfica
- [ ] Integração com banco de dados
- [ ] API REST para consultas

---

## 📝 Fichas de Resposta Rápida

### "Como funciona o parser CSV?"
**Resposta**: Máquina de estados que controla um flag `emAspas`. Quando encontra separador (`,` ou `;`) FORA de aspas, finaliza o campo. Caracteres dentro de aspas são ignorados como separadores.

### "Por que as 5 metas têm fórmulas diferentes?"
**Resposta**: Cada meta avalia um aspecto diferente do desempenho judicial. Meta 2A usa fator 1000/7 porque é uma exigência específica do sistema judiciário para normalizar o cálculo daquele tipo de processo.

### "O que acontece se um arquivo de entrada estiver corrompido?"
**Resposta**: O programa contabiliza um erro com `perror()` mas continua processando os próximos arquivos. Garante robustez mesmo com dados incompletos.

### "Como garantir acentuação correta?"
**Resposta**: Configuração explícita de locale e code page no início do programa:
- Windows: `SetConsoleOutputCP(CP_UTF8)`
- Linux/Mac: `setlocale(LC_ALL, "")`

---

## 🎯 Dicas para Apresentação

1. **Comece com o problema**: "Temos dados de 27 tribunais em arquivos separados..."
2. **Mostre o menu funcionando**: Demonstração prática > explicação teórica
3. **Explique um fluxo completo**: 1 → 2 → 3 em sequência
4. **Foque nos desafios**: Parsing CSV é o ponto técnico principale
5. **Tenha os arquivos prontos**: Compile antes de apresentar
6. **Esteja pronto para perguntas**: Revise a estrutura de dados e algoritmos

---

## 📚 Documentação Complementar

Para dúvidas detalhadas:
- `README.md` - Visão geral do projeto
- `DOCUMENTACAO.md` - Detalhes técnicos profundos
- Comentários no código (main.c, lista.c)

Boa apresentação! 👍
