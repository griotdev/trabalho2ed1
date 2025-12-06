/* include/retangulo.h
 *
 * TAD para representar retângulos.
 * Retângulo é definido por: id, posição (x, y), largura, altura e cores.
 */

#ifndef RETANGULO_H
#define RETANGULO_H

/* Tipo opaco para Retângulo */
typedef void* Retangulo;

/**
 * Cria um retângulo.
 * @param id Identificador único
 * @param x Coordenada x do canto superior esquerdo
 * @param y Coordenada y do canto superior esquerdo
 * @param largura Largura do retângulo
 * @param altura Altura do retângulo
 * @param corBorda Cor da borda (formato #RRGGBB)
 * @param corPreenchimento Cor de preenchimento (formato #RRGGBB)
 * @return Ponteiro para o retângulo criado, ou NULL em caso de erro
 */
Retangulo criaRetangulo(int id, double x, double y, double largura, double altura,
                        const char *corBorda, const char *corPreenchimento);

/**
 * Cria uma cópia de um retângulo.
 */
Retangulo clonaRetangulo(Retangulo retangulo);

/**
 * Libera memória de um retângulo.
 */
void destroiRetangulo(Retangulo retangulo);

/**
 * Getters
 */
int getRetanguloId(Retangulo r);
double getRetanguloX(Retangulo r);
double getRetanguloY(Retangulo r);
double getRetanguloLargura(Retangulo r);
double getRetanguloAltura(Retangulo r);
const char* getRetanguloCorBorda(Retangulo r);
const char* getRetanguloCorPreenchimento(Retangulo r);

/**
 * Setters
 */
void setRetanguloPosicao(Retangulo r, double x, double y);

/**
 * Verifica se dois retângulos se sobrepõem.
 */
int retangulosSobrepoe(Retangulo r1, Retangulo r2);

/**
 * Define as cores do retângulo.
 * @param r Ponteiro para o retângulo
 * @param corBorda Nova cor de borda
 * @param corPreenchimento Nova cor de preenchimento
 */
void setRetanguloCores(Retangulo r, const char *corBorda, const char *corPreenchimento);

#endif /* RETANGULO_H */
