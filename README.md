# Trabalho 2 - Região de Visibilidade

**Estrutura de Dados - C99**

## Informações do Aluno

| Campo     | Valor                      |
|-----------|----------------------------|
| Matrícula | 202500560216               |
| Nome      | André Ichihashi Rodrigues  |

---

## Descrição

Este programa implementa o cálculo de **Região de Visibilidade** utilizando o algoritmo de **Varredura Angular (Angular Plane Sweep)**. O sistema lê arquivos de definição geométrica (`.geo`) e arquivos de consulta (`.qry`), gerando saídas visuais em SVG e relatórios em TXT.

### Funcionalidades

- **Formas Geométricas**: Suporte a círculos, retângulos, linhas e textos.
- **Anteparos**: Transformação de formas em segmentos bloqueantes (comando `a`).
- **Bombas de Destruição** (`d`): Remove formas visíveis a partir de um ponto.
- **Bombas de Pintura** (`P`): Altera a cor de formas visíveis.
- **Bombas de Clonagem** (`cln`): Duplica formas visíveis com deslocamento.

---

## Compilação

### Pré-requisitos

- GCC com suporte a C99
- Make

### Comandos

```bash
cd src
make        # Compila o projeto
make clean  # Remove arquivos objeto e executável
```

O executável gerado será `ted` dentro da pasta `src/`.

---

## Execução

### Sintaxe

```bash
./ted -e <dir_entrada> -f <arquivo.geo> -o <dir_saida> [-q <arquivo.qry>] [-to q|m] [-in N]
```

### Argumentos Obrigatórios

| Flag | Descrição                                      |
|------|------------------------------------------------|
| `-e` | Diretório base de entrada (onde estão os arquivos `.geo` e `.qry`) |
| `-f` | Nome do arquivo de geometria (`.geo`)          |
| `-o` | Diretório de saída para os arquivos gerados    |

### Argumentos Opcionais

| Flag   | Descrição                                           | Padrão |
|--------|-----------------------------------------------------|--------|
| `-q`   | Arquivo de consultas (`.qry`)                       | -      |
| `-to`  | Tipo de ordenação: `q` (QuickSort) ou `m` (MergeSort) | `q`    |
| `-in`  | Limiar para InsertionSort no MergeSort híbrido      | `10`   |

### Exemplos

```bash
# Apenas gerar SVG das formas (sem consultas)
./ted -e ../testes -f cenario.geo -o ../output

# Executar com arquivo de consultas
./ted -e ../testes -f cenario.geo -q consultas.qry -o ../output

# Usar MergeSort com limiar 15
./ted -e ../testes -f cenario.geo -q consultas.qry -o ../output -to m -in 15
```

---

## Estrutura de Arquivos de Entrada

### Arquivo `.geo` (Geometria)

Define as formas do cenário inicial:

```
c <id> <x> <y> <raio> <cor_borda> <cor_preenchimento>
r <id> <x> <y> <largura> <altura> <cor_borda> <cor_preenchimento>
l <id> <x1> <y1> <x2> <y2> <cor>
t <id> <x> <y> <cor_borda> <cor_preenchimento> <ancora> <texto>
```

### Arquivo `.qry` (Consultas)

Define comandos a serem executados:

```
a <id_inicio> <id_fim> [v|h]   # Transforma formas em anteparos
d <x> <y> <sufixo>             # Bomba de destruição
P <x> <y> <cor> <sufixo>       # Bomba de pintura
cln <x> <y> <dx> <dy> <sufixo> # Bomba de clonagem
```

---

## Arquivos de Saída

| Arquivo                          | Descrição                                    |
|----------------------------------|----------------------------------------------|
| `<nome>.svg`                     | SVG inicial com todas as formas              |
| `<nome>-<qry>.svg`               | SVG final após processamento das consultas   |
| `<nome>-<qry>-<sufixo>.svg`      | SVG individual de comando (quando sufixo ≠ `-`) |
| `<nome>-<qry>.txt`               | Relatório textual das ações realizadas       |

---

## Estrutura do Projeto

```
src/
├── main.c                  # Ponto de entrada
├── makefile                # Script de compilação
└── lib/
    ├── argumentos/         # Processamento de CLI
    ├── estruturas/         # Lista e Árvore
    ├── formas/             # Círculo, Retângulo, Linha, Texto
    ├── geo/                # Parser GEO e geração SVG
    ├── geometria/          # Ponto, Segmento, Cálculos, Polígono
    ├── qry/                # Parser QRY e comandos (a, d, P, cln)
    ├── util/               # Algoritmos de ordenação
    └── visibilidade/       # Algoritmo de varredura angular
```

---
