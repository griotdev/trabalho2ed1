/* include/formas.h
 *
 * Módulo de formas geométricas - Interface genérica.
 * Usa os TADs específicos: circulo, retangulo, linha, texto.
 */

#ifndef FORMAS_H
#define FORMAS_H

#include "../circulo/circulo.h"
#include "../retangulo/retangulo.h"
#include "../linha/linha.h"
#include "../texto/texto.h"

/* Enumeração para tipo de forma */
typedef enum {
    TIPO_CIRCULO,
    TIPO_RETANGULO,
    TIPO_LINHA,
    TIPO_TEXTO
} TipoForma;

/* Estrutura genérica para encapsular qualquer forma */
typedef struct forma Forma;

/**
 * Cria uma forma genérica a partir de uma forma específica.
 * @param tipo Tipo da forma
 * @param dados Ponteiro para os dados da forma (Circulo*, Retangulo*, etc)
 * @return Ponteiro para a forma genérica
 */
Forma* criaForma(TipoForma tipo, void *dados);

/**
 * Obtém o tipo da forma.
 */
TipoForma getFormaTipo(Forma *f);

/**
 * Obtém os dados da forma.
 */
void* getFormaDados(Forma *f);

/**
 * Obtém o ID da forma (funciona para qualquer tipo).
 */
int getFormaId(Forma *f);

/**
 * Libera memória de uma forma genérica.
 */
void destroiForma(Forma *forma);

/**
 * Clona uma forma genérica.
 */
Forma* clonaForma(Forma *forma);

/**
 * Define a posição (âncora) de uma forma.
 */
void setFormaPosicao(Forma *forma, double x, double y);

#endif /* FORMAS_H */
