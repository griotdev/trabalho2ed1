# =============================================================================
# Makefile - Projeto Estrutura de Dados (Região de Visibilidade)
# =============================================================================

CC = gcc
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -Wno-format-truncation -g
LDFLAGS = -lm

LIB_DIR = lib
BUILD_DIR = build
TARGET = ted

# =============================================================================
# Arquivos fonte
# =============================================================================

# Módulos base
ARGUMENTOS_SRC = $(LIB_DIR)/argumentos/argumentos.c
LISTA_SRC = $(LIB_DIR)/estruturas/lista/lista.c
ARVORE_SRC = $(LIB_DIR)/estruturas/arvore/arvore.c
SORT_SRC = $(LIB_DIR)/util/sort/sort.c

# Formas
CIRCULO_SRC = $(LIB_DIR)/formas/circulo/circulo.c
RETANGULO_SRC = $(LIB_DIR)/formas/retangulo/retangulo.c
LINHA_SRC = $(LIB_DIR)/formas/linha/linha.c
TEXTO_SRC = $(LIB_DIR)/formas/texto/texto.c
FORMAS_SRC = $(LIB_DIR)/formas/formas/formas.c

# GEO
PARSER_GEO_SRC = $(LIB_DIR)/geo/parser_geo/parser_geo.c
SVG_SRC = $(LIB_DIR)/geo/svg/svg.c

# Geometria
PONTO_SRC = $(LIB_DIR)/geometria/ponto/ponto.c
SEGMENTO_SRC = $(LIB_DIR)/geometria/segmento/segmento.c
CALCULOS_SRC = $(LIB_DIR)/geometria/calculos/calculos.c

# Visibilidade
VISIBILIDADE_SRC = $(LIB_DIR)/visibilidade/visibilidade.c

# QRY
PARSER_QRY_SRC = $(LIB_DIR)/qry/parser_qry/parser_qry.c
CMD_A_SRC = $(LIB_DIR)/qry/cmd_a/cmd_a.c
CMD_D_SRC = $(LIB_DIR)/qry/cmd_d/cmd_d.c
CMD_P_SRC = $(LIB_DIR)/qry/cmd_p/cmd_p.c
CMD_CLN_SRC = $(LIB_DIR)/qry/cmd_cln/cmd_cln.c

MAIN_SRC = main.c

# =============================================================================
# Agrupa fontes
# =============================================================================

SOURCES = $(ARGUMENTOS_SRC) $(LISTA_SRC) $(ARVORE_SRC) $(SORT_SRC) \
          $(CIRCULO_SRC) $(RETANGULO_SRC) $(LINHA_SRC) $(TEXTO_SRC) $(FORMAS_SRC) \
          $(PARSER_GEO_SRC) $(SVG_SRC) \
          $(PONTO_SRC) $(SEGMENTO_SRC) $(CALCULOS_SRC) \
          $(VISIBILIDADE_SRC) \
          $(PARSER_QRY_SRC) $(CMD_A_SRC) $(CMD_D_SRC) $(CMD_P_SRC) $(CMD_CLN_SRC)

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
           -I$(LIB_DIR)/geometria/calculos \
           -I$(LIB_DIR)/visibilidade \
           -I$(LIB_DIR)/qry/parser_qry \
           -I$(LIB_DIR)/qry/cmd_a \
           -I$(LIB_DIR)/qry/cmd_d \
           -I$(LIB_DIR)/qry/cmd_p \
           -I$(LIB_DIR)/qry/cmd_cln \
           -I$(LIB_DIR)/util/sort

# =============================================================================
# Regras
# =============================================================================

.PHONY: all clean dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJECTS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Regras de compilação
$(BUILD_DIR)/argumentos.o: $(ARGUMENTOS_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/lista.o: $(LISTA_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/arvore.o: $(ARVORE_SRC)
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

$(BUILD_DIR)/visibilidade.o: $(VISIBILIDADE_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/parser_qry.o: $(PARSER_QRY_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/cmd_a.o: $(CMD_A_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/cmd_d.o: $(CMD_D_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/cmd_p.o: $(CMD_P_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/cmd_cln.o: $(CMD_CLN_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/main.o: $(MAIN_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/sort.o: $(SORT_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpeza
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
