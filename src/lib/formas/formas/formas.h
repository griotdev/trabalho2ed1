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

/* Tipo opaco para Forma genérica */
typedef void* Forma;

/**
 * Cria uma forma genérica a partir de uma forma específica.
 * @param tipo Tipo da forma
 * @param dados Ponteiro para os dados da forma (Circulo, Retangulo, etc)
 * @return Ponteiro para a forma genérica, ou NULL em caso de erro
 */
Forma criaForma(TipoForma tipo, void *dados);

/**
 * Obtém o tipo da forma.
 * @param f Ponteiro para a forma
 * @return TipoForma da forma
 */
TipoForma getFormaTipo(Forma f);

/**
 * Obtém os dados da forma.
 * @param f Ponteiro para a forma
 * @return Ponteiro para os dados internos
 */
void* getFormaDados(Forma f);

/**
 * Obtém o ID da forma (funciona para qualquer tipo).
 * @param f Ponteiro para a forma
 * @return ID da forma, ou -1 se inválida
 */
int getFormaId(Forma f);

/**
 * Libera memória de uma forma genérica.
 * @param forma Ponteiro para a forma a ser destruída
 */
void destroiForma(Forma forma);

/**
 * Clona uma forma genérica.
 * @param forma Ponteiro para a forma a ser clonada
 * @return Nova forma clonada, ou NULL em caso de erro
 */
Forma clonaForma(Forma forma);

/**
 * Define a posição (âncora) de uma forma.
 * @param forma Ponteiro para a forma
 * @param x Nova coordenada X
 * @param y Nova coordenada Y
 */
void setFormaPosicao(Forma forma, double x, double y);

#endif /* FORMAS_H */
