/* include/linha.h
 *
 * TAD para representar linhas.
 * Linha é definida por: id, dois pontos (x1, y1) e (x2, y2), e cor.
 */

#ifndef LINHA_H
#define LINHA_H

/* Tipo opaco para Linha */
typedef void* Linha;

/**
 * Cria uma linha.
 * @param id Identificador único
 * @param x1 Coordenada x do ponto inicial
 * @param y1 Coordenada y do ponto inicial
 * @param x2 Coordenada x do ponto final
 * @param y2 Coordenada y do ponto final
 * @param cor Cor da linha (formato #RRGGBB)
 * @return Ponteiro para a linha criada, ou NULL em caso de erro
 */
Linha criaLinha(int id, double x1, double y1, double x2, double y2, const char *cor);

/**
 * Cria uma cópia de uma linha.
 */
Linha clonaLinha(Linha linha);

/**
 * Libera memória de uma linha.
 */
void destroiLinha(Linha linha);

/**
 * Getters
 */
int getLinhaId(Linha l);
double getLinhaX1(Linha l);
double getLinhaY1(Linha l);
double getLinhaX2(Linha l);
double getLinhaY2(Linha l);
const char* getLinhaCor(Linha l);

/**
 * Setters - desloca a linha mantendo seu comprimento e direção
 */
void setLinhaPosicao(Linha l, double dx, double dy);

#endif /* LINHA_H */
