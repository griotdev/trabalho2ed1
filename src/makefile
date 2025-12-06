# =============================================================================
# Makefile - Projeto Estrutura de Dados (Região de Visibilidade)
# Compilador: GCC com C99
# =============================================================================

CC = gcc
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -Wno-format-truncation -g
LDFLAGS = -lm

SRC_DIR = .
LIB_DIR = lib
BUILD_DIR = build

TARGET = t2

# =============================================================================
# Arquivos fonte dos módulos
# =============================================================================

# Módulo de argumentos
ARGUMENTOS_SRC = $(LIB_DIR)/argumentos/argumentos.c

# Módulo de estruturas
LISTA_SRC = $(LIB_DIR)/estruturas/lista/lista.c

# Módulos de formas
CIRCULO_SRC = $(LIB_DIR)/formas/circulo/circulo.c
RETANGULO_SRC = $(LIB_DIR)/formas/retangulo/retangulo.c
LINHA_SRC = $(LIB_DIR)/formas/linha/linha.c
TEXTO_SRC = $(LIB_DIR)/formas/texto/texto.c
FORMAS_SRC = $(LIB_DIR)/formas/formas/formas.c

# Módulos GEO (parser e svg)
PARSER_GEO_SRC = $(LIB_DIR)/geo/parser_geo/parser_geo.c
SVG_SRC = $(LIB_DIR)/geo/svg/svg.c

# Módulos de geometria
PONTO_SRC = $(LIB_DIR)/geometria/ponto/ponto.c
SEGMENTO_SRC = $(LIB_DIR)/geometria/segmento/segmento.c
CALCULOS_SRC = $(LIB_DIR)/geometria/calculos/calculos.c

# Árvore de segmentos ativos
ARVORE_SRC = $(LIB_DIR)/estruturas/arvore/arvore.c

# Arquivo principal
MAIN_SRC = main.c

# =============================================================================
# Agrupa todos os fontes
# =============================================================================

SOURCES = $(ARGUMENTOS_SRC) \
          $(LISTA_SRC) \
          $(CIRCULO_SRC) \
          $(RETANGULO_SRC) \
          $(LINHA_SRC) \
          $(TEXTO_SRC) \
          $(FORMAS_SRC) \
          $(PARSER_GEO_SRC) \
          $(SVG_SRC) \
          $(PONTO_SRC) \
          $(SEGMENTO_SRC) \
          $(CALCULOS_SRC) \
          $(ARVORE_SRC)

OBJECTS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))
MAIN_OBJ = $(BUILD_DIR)/main.o

# =============================================================================
# Includes
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
           -I$(LIB_DIR)/geo/svg \
           -I$(LIB_DIR)/geometria/ponto \
           -I$(LIB_DIR)/geometria/segmento \
           -I$(LIB_DIR)/geometria/calculos

# =============================================================================
# Regras principais
# =============================================================================

.PHONY: all clean dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJECTS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# =============================================================================
# Regras de compilação
# =============================================================================

$(BUILD_DIR)/argumentos.o: $(ARGUMENTOS_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/lista.o: $(LISTA_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/circulo.o: $(CIRCULO_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/retangulo.o: $(RETANGULO_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/linha.o: $(LINHA_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/texto.o: $(TEXTO_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/formas.o: $(FORMAS_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/parser_geo.o: $(PARSER_GEO_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/svg.o: $(SVG_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/ponto.o: $(PONTO_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/segmento.o: $(SEGMENTO_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/calculos.o: $(CALCULOS_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/arvore.o: $(ARVORE_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/main.o: $(MAIN_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# =============================================================================
# Limpeza
# =============================================================================

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
