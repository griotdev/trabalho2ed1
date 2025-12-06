# =============================================================================
# Makefile - Projeto Estrutura de Dados (Região de Visibilidade)
# Compilador: GCC com C99
# =============================================================================

# Compilador e flags obrigatórias
CC = gcc
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -g
LDFLAGS = -lm

# Diretórios
SRC_DIR = .
LIB_DIR = lib
BUILD_DIR = build

# Nome do executável
TARGET = t2

# =============================================================================
# Arquivos fonte dos módulos
# =============================================================================

# Módulo de argumentos
ARGUMENTOS_SRC = $(LIB_DIR)/argumentos/argumentos.c

# Módulo de estruturas
LISTA_SRC = $(LIB_DIR)/estruturas/lista/lista.c
ARVORE_SRC = $(LIB_DIR)/estruturas/arvore/arvore.c

# Módulos de formas
CIRCULO_SRC = $(LIB_DIR)/formas/circulo/circulo.c
RETANGULO_SRC = $(LIB_DIR)/formas/retangulo/retangulo.c
LINHA_SRC = $(LIB_DIR)/formas/linha/linha.c
TEXTO_SRC = $(LIB_DIR)/formas/texto/texto.c
FORMAS_SRC = $(LIB_DIR)/formas/formas/formas.c

# Módulos GEO (parser e svg)
PARSER_GEO_SRC = $(LIB_DIR)/geo/parser_geo/parser_geo.c
SVG_SRC = $(LIB_DIR)/geo/svg/svg.c

# Módulos de geometria (futuros)
# PONTO_SRC = $(LIB_DIR)/geometria/ponto/ponto.c
# SEGMENTO_SRC = $(LIB_DIR)/geometria/segmento/segmento.c
# POLIGONO_SRC = $(LIB_DIR)/geometria/poligono/poligono.c
# CALCULOS_SRC = $(LIB_DIR)/geometria/calculos/calculos.c

# Arquivo principal
MAIN_SRC = main.c

# =============================================================================
# Agrupa todos os fontes existentes
# =============================================================================

# Fontes que existem agora
SOURCES = $(ARGUMENTOS_SRC) \
          $(LISTA_SRC) \
          $(CIRCULO_SRC) \
          $(RETANGULO_SRC) \
          $(LINHA_SRC) \
          $(TEXTO_SRC) \
          $(FORMAS_SRC) \
          $(PARSER_GEO_SRC) \
          $(SVG_SRC)

# Objetos correspondentes
OBJECTS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))

# Objeto do main (quando existir)
MAIN_OBJ = $(BUILD_DIR)/main.o

# =============================================================================
# Includes - todos os diretórios de headers
# =============================================================================

INCLUDES = -I$(LIB_DIR)/argumentos \
           -I$(LIB_DIR)/estruturas/lista \
           -I$(LIB_DIR)/estruturas/arvore \
           -I$(LIB_DIR)/formas/circulo \
           -I$(LIB_DIR)/formas/retangulo \
           -I$(LIB_DIR)/formas/linha \
           -I$(LIB_DIR)/formas/texto \
           -I$(LIB_DIR)/formas/formas \
           -I$(LIB_DIR)/geo/parser_geo \
           -I$(LIB_DIR)/geo/svg

# =============================================================================
# Regras principais
# =============================================================================

.PHONY: all clean test dirs

# Regra padrão: cria diretórios e compila tudo
all: dirs $(TARGET)

# Cria diretório de build
dirs:
	@mkdir -p $(BUILD_DIR)

# Linka o executável final
$(TARGET): $(OBJECTS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# =============================================================================
# Regras para compilar cada módulo
# =============================================================================

# Argumentos
$(BUILD_DIR)/argumentos.o: $(ARGUMENTOS_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Lista
$(BUILD_DIR)/lista.o: $(LISTA_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Círculo
$(BUILD_DIR)/circulo.o: $(CIRCULO_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Retângulo
$(BUILD_DIR)/retangulo.o: $(RETANGULO_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Linha
$(BUILD_DIR)/linha.o: $(LINHA_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Texto
$(BUILD_DIR)/texto.o: $(TEXTO_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Formas (agregador)
$(BUILD_DIR)/formas.o: $(FORMAS_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Parser GEO
$(BUILD_DIR)/parser_geo.o: $(PARSER_GEO_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# SVG
$(BUILD_DIR)/svg.o: $(SVG_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Main
$(BUILD_DIR)/main.o: $(MAIN_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# =============================================================================
# Testes unitários
# =============================================================================

# Teste da lista
test_lista: dirs $(BUILD_DIR)/lista.o
	$(CC) $(CFLAGS) $(INCLUDES) -DTESTE_LISTA $(LISTA_SRC) -o $(BUILD_DIR)/test_lista $(LDFLAGS)
	./$(BUILD_DIR)/test_lista

# Teste de argumentos
test_argumentos: dirs $(BUILD_DIR)/argumentos.o
	$(CC) $(CFLAGS) $(INCLUDES) -DTESTE_ARGUMENTOS $(ARGUMENTOS_SRC) -o $(BUILD_DIR)/test_argumentos $(LDFLAGS)
	./$(BUILD_DIR)/test_argumentos

# =============================================================================
# Limpeza
# =============================================================================

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

# =============================================================================
# Informações de debug
# =============================================================================

info:
	@echo "Compilador: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Fontes: $(SOURCES)"
	@echo "Objetos: $(OBJECTS)"
	@echo "Includes: $(INCLUDES)"
