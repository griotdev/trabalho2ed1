/* include/circulo.h
 *
 * TAD para representar círculos.
 * Círculo é definido por: id, centro (x, y), raio e cores.
 */

#ifndef CIRCULO_H
#define CIRCULO_H

/* Tipo opaco para Círculo */
typedef void* Circulo;

/**
 * Cria um círculo.
 * @param id Identificador único do círculo
 * @param x Coordenada x do centro
 * @param y Coordenada y do centro
 * @param raio Raio do círculo
 * @param corBorda Cor da borda (formato #RRGGBB)
 * @param corPreenchimento Cor de preenchimento (formato #RRGGBB)
 * @return Ponteiro para o círculo criado, ou NULL em caso de erro
 */
Circulo criaCirculo(int id, double x, double y, double raio, 
                    const char *corBorda, const char *corPreenchimento);

/**
 * Cria uma cópia de um círculo.
 * @param circulo Círculo a ser clonado
 * @return Ponteiro para o novo círculo, ou NULL em caso de erro
 */
Circulo clonaCirculo(Circulo circulo);

/**
 * Libera memória de um círculo.
 * @param circulo Ponteiro para o círculo
 */
void destroiCirculo(Circulo circulo);

/**
 * Obtém o ID do círculo.
 */
int getCirculoId(Circulo c);

/**
 * Obtém a coordenada x do centro do círculo.
 */
double getCirculoX(Circulo c);

/**
 * Obtém a coordenada y do centro do círculo.
 */
double getCirculoY(Circulo c);

/**
 * Obtém o raio do círculo.
 */
double getCirculoRaio(Circulo c);

/**
 * Obtém a cor da borda do círculo.
 */
const char* getCirculoCorBorda(Circulo c);

/**
 * Obtém a cor de preenchimento do círculo.
 */
const char* getCirculoCorPreenchimento(Circulo c);

/**
 * Define nova posição do círculo.
 * @param c Ponteiro para o círculo
 * @param x Nova coordenada x
 * @param y Nova coordenada y
 */
void setCirculoPosicao(Circulo c, double x, double y);

/**
 * Define as cores do círculo.
 * @param c Ponteiro para o círculo
 * @param corBorda Nova cor de borda
 * @param corPreenchimento Nova cor de preenchimento
 */
void setCirculoCores(Circulo c, const char *corBorda, const char *corPreenchimento);

/**
 * Verifica se dois círculos se sobrepõem.
 * @param c1 Primeiro círculo
 * @param c2 Segundo círculo
 * @return 1 se sobrepõem, 0 caso contrário
 */
int circulosSobrepoe(Circulo c1, Circulo c2);

#endif /* CIRCULO_H */
