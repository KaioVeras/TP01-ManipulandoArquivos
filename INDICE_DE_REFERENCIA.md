# ÍNDICE DE REFERÊNCIA RÁPIDA

Navegação rápida pela documentação do projeto.

---

## 📑 Arquivos de Documentação

### 1. [README.md](README.md) - Start Here! ⭐
   - Visão geral do projeto
   - 3 funcionalidades principais
   - Como compilar e executar
   - Estrutura de arquivos
   - Formato CSV esperado

### 2. [DOCUMENTACAO.md](DOCUMENTACAO.md) - Técnico Profundo
   - Visão geral técnica
   - Estruturas de dados detalhadas
   - Explicação de cada função
   - Algoritmos importantes
   - Tratamento de erros

### 3. [GUIA_DE_APRESENTACAO.md](GUIA_DE_APRESENTACAO.md) - Para o Professor
   - Introdução (como começar a falar)
   - Escopo de funcionalidades
   - Arquitetura do código
   - Detalhes técnicos importantes
   - Exemplo de uso prático
   - Desafios superados
   - Testes sugeridos
   - Dicas de apresentação

### 4. [FAQ.md](FAQ.md) - Perguntas/Respostas
   - 15+ perguntas frequentes
   - Respostas técnicas
   - Respostas a críticas possíveis
   - Checklist para apresentação

### 5. [EXEMPLOS_DE_SAIDA.md](EXEMPLOS_DE_SAIDA.md) - Validação
   - Execução normal esperada
   - Arquivos gerados (amostras)
   - Casos de erro
   - Testes de robustez
   - Performance esperada

### 6. [INDICE_DE_REFERENCIA.md](INDICE_DE_REFERENCIA.md) - Você está aqui! 🔍

---

## 🎯 Encontre Respostas por Tópico

### "Como funciona o programa?"
→ Comece em **README.md** → Seção "Funcionalidades"

### "Qual é a estrutura técnica?"
→ **DOCUMENTACAO.md** → Seção "Estruturas de Dados"

### "Como se compila?"
→ **README.md** → Seção "Como Compilar e Executar"

### "Como apresentar para o professor?"
→ **GUIA_DE_APRESENTACAO.md** → Seção "Introdução"

### "Como funciona o parsing CSV?"
→ **DOCUMENTACAO.md** → Seção "Funções de Parsing CSV"

### "Por que fazer assim?"
→ **FAQ.md** → Perguntas iniciadas com "Por que"

### "O que deve ser a saída?"
→ **EXEMPLOS_DE_SAIDA.md** → Seção correspondente

### "Meu código está certo?"
→ **EXEMPLOS_DE_SAIDA.md** → Compare suas saídas

### "Qual é a explicação das 5 metas?"
→ **README.md** → Seção "Funcionalidades → Opção 2"
→ **DOCUMENTACAO.md** → Seção "Algoritmos Importantes"

### "Como é a máquina de estados do CSV?"
→ **DOCUMENTACAO.md** → Função `extrairCamposCSV()`

---

## 🔑 Termos Técnicos Explicados

| Termo | Onde Encontrar | Rápido |
|-------|----------------|--------|
| AcumuladoTribunal | DOCUMENTACAO.md | Struct com 18 fields, um por tribunal |
| Parse CSV | DOCUMENTACAO.md | Extrai campos respeitando aspas |
| Normalização de Texto | DOCUMENTACAO.md | Maiúsculas + trim espaços |
| Meta 1 | README.md | Taxa geral de julgamento |
| Meta 2A | README.md | Eficiência em casos até 2 anos |
| Meta 2Ant | README.md | Eficiência em casos antigos |
| Meta 4A | README.md | Análise preliminar |
| Meta 4B | README.md | Análise profunda |

---

## 🚀 Fluxo por Caso de Uso

### Caso: Preparando apresentação para professor
1. Leia **README.md** completamente
2. Revise **GUIA_DE_APRESENTACAO.md** seções 1-5
3. Consulte **FAQ.md** para ter respostas prontas
4. Estude **DOCUMENTACAO.md** seção "Algoritmos Importantes"

### Caso: Testando validação do código
1. Compile: `gcc -o main main.c lista.c`
2. Consulte **EXEMPLOS_DE_SAIDA.md**
3. Execute cada cenário (1, 2, 3, erros)
4. Compare saída esperada com sua saída

### Caso: Respondendo pergunta técnica do professor
1. Procure por palavra-chave em **FAQ.md**
2. Se não encontrar, vá a **DOCUMENTACAO.md**
3. Como último recurso, verifique comentários em `main.c` ou `lista.c`

### Caso: Entender uma função específica
1. Procure nome da função em **DOCUMENTACAO.md**
2. Seção "Funções Principais"
3. Compreenda pseudocódigo
4. Verifique comentários no código

### Caso: Extensão/Melhoria do projeto
1. Verifique **GUIA_DE_APRESENTACAO.md** → "Possíveis Extensões"
2. Estude **DOCUMENTACAO.md** → "Algoritmos Importantes"
3. Considere impacto nas estruturas de dados

---

## 📊 Mapa de Conteúdo por Arquivo

### main.c
- Menu interativo
- Inicialização
- Validação de dependências
- Despacho para funções

### lista.h
- `typedef AcumuladoTribunal`
- Constantes (#define)
- Protótipos de funções

### lista.c
- `abrirArquivoComPrefixos()` - I/O robusto
- `extrairCamposCSV()` - Parser principal
- `converterSeparadorLinha()` - Normalização
- `normalizarTexto()` - Busca case-insensitive
- `encontrarOuCriarTribunal()` - Consolidação
- `concatenarArquivosCSV()` - Função 1
- `gerarResumoCSV()` - Função 2
- `gerarCSVPorMunicipio()` - Função 3

---

## ⏱️ Tempo de Leitura Estimado

Por Documento:

| Documento | Tempo | Mínimo | Máximo |
|-----------|-------|--------|--------|
| README.md | 10 min | 5 min | 15 min |
| DOCUMENTACAO.md | 25 min | 15 min | 40 min |
| GUIA_DE_APRESENTACAO.md | 15 min | 10 min | 25 min |
| FAQ.md | 10 min | 5 min | 20 min |
| EXEMPLOS_DE_SAIDA.md | 10 min | 5 min | 20 min |
| **TOTAL** | **70 min** | **40 min** | **120 min** |

---

## ✅ Checklist de Leitura

Para apresentação sem nervosismo:

- [ ] Li **README.md** completamente
- [ ] Entendi as 3 funcionalidades
- [ ] Revisei **GUIA_DE_APRESENTACAO.md** seções 1-5
- [ ] Consulti **FAQ.md** para respostas rápidas
- [ ] Testei código e validei com **EXEMPLOS_DE_SAIDA.md**
- [ ] Entendi como funciona o CSV parsing
- [ ] Posso explicar as 5 metas judiciais
- [ ] Sei responder "Por que" de 3 decisões de design
- [ ] Executei código em tempo real simul

ação
- [ ] Tenho plano B se algo falhar na apresentação

---

## 🔗 Ligações Entre Documentos

```
README.md
  ├─ remete para → DOCUMENTACAO.md (detalhes técnicos)
  ├─ remete para → EXEMPLOS_DE_SAIDA.md (validação)
  └─ remete para → GUIA_DE_APRESENTACAO.md (apresentação)

DOCUMENTACAO.md
  ├─ remete para → README.md (conceitos básicos)
  ├─ remete para → FAQ.md (design decisions)
  └─ remete para → código comentado (main.c, lista.c)

GUIA_DE_APRESENTACAO.md
  ├─ remete para → README.md (visão geral)
  ├─ remete para → DOCUMENTACAO.md (detalhes)
  └─ remete para → FAQ.md (respostas)

FAQ.md
  ├─ remete para → DOCUMENTACAO.md (fundamentação)
  ├─ remete para → README.md (contexto)
  └─ remete para → EXEMPLOS_DE_SAIDA.md (validação)

EXEMPLOS_DE_SAIDA.md
  ├─ remete para → README.md (interpretação)
  └─ remete para → DOCUMENTACAO.md (cálculos das metas)
```

---

## 🎓 Sequência Recomendada de Aprendizado

### Semana 1: Compreensão Geral
- Dia 1: README.md completo
- Dia 2: GUIA_DE_APRESENTACAO.md seções 1-3
- Dia 3: EXEMPLOS_DE_SAIDA.md - executar código

### Semana 2: Aprofundamento Técnico
- Dia 4: DOCUMENTACAO.md - estruturas de dados
- Dia 5: DOCUMENTACAO.md - funções principais
- Dia 6: FAQ.md - consolidar conhecimento

### Semana 3: Apresentação
- Dia 7: GUIA_DE_APRESENTACAO.md completo
- Dia 8: Ensaiar apresentação (mock presentation)
- Dia 9: Dia da apresentação!

---

## 💬 Como Citar Este Projeto

Se precisar referenciar em trabalho escrito:

```
TP01 - Manipulando Arquivos [sistema]
Trabalho Prático de Programação C
Consolidação de dados de Tribunais Regionais Eleitorais
Implementação: 2026
Linguagem: C (ANSI C99)
Plataformas: Windows, Linux, macOS
```

---

## 🔧 Referência Rápida de Compilação

### Simples
```bash
gcc -o main main.c lista.c
```

### Com Warnings
```bash
gcc -Wall -Wextra -o main main.c lista.c
```

### Com Debug
```bash
gcc -g -Wall -Wextra -o main main.c lista.c
```

### Com Otimização
```bash
gcc -O2 -o main main.c lista.c
```

---

## 🏆 Pontos de Ênfase para Apresentação

Quando apresentar, destaque:

1. **Parser CSV robusto** (máquina de estados)
2. **Normalização inteligente de texto** (case-insensitive)
3. **5 métricas judiciais complexas** (fórmulas matemáticas)
4. **Validação de dependências** (opção 2 requer opção 1)
5. **Tratamento de erros gracioso** (programa continua)

---

## 🚨 ERROS COMUNS - Evite!

- ❌ Tentar opção 2 antes de opção 1
- ❌ Compilar sem incluir `lista.c`
- ❌ Não ter os 27 arquivos CSV
- ❌ Digitar nome de tribunal na opção 3 (é para MUNICÍPIO)
- ❌ Esperar captura automática de enter entre opções

---

## 📞 Suporte Rápido

### "Código não compila"
→ Verifique se ambos arquivos estão: `gcc -o main main.c lista.c`

### "Arquivo não encontrado"
→ Verifique se está no diretório correto com os 27 CSVs

### "Acentuação não aparece"
→ Windows: use PowerShell. Linux: configure locale.

### "Programa travou"
→ Normal de ser lento na concatenação (150MB+)

### "Não me lembro qual opção fazer"
→ Execute e veja o menu! Program tem interface clara.

---

## 🎯 Foco por Públic-alvo

### Para o Professor
→ Ler: **GUIA_DE_APRESENTAÇÃO.md** completo

### Para Colegas da Equipe
→ Ler: **README.md** + **EXEMPLOS_DE_SAIDA.md**

### Para o Próprio Time Técnico
→ Ler: **DOCUMENTACAO.md** + código comentado

### Para Alguém Tentando Entender o Projeto
→ Ler: **README.md** primeiro, depois **FAQ.md**

---

## 📌 Últimas Dicas

1. **Não memorize**. Use este índice para localizar respostas.
2. **Pratique a apresentação** usando **GUIA_DE_APRESENTAÇÃO.md**
3. **Valide sempre** com **EXEMPLOS_DE_SAIDA.md**
4. **Tenha respostas prontas** consultando **FAQ.md**
5. **Código é referência final** - consulte comentários se dúvida!

---

Bom projeto! 🎓

*Última atualização: 2026*
