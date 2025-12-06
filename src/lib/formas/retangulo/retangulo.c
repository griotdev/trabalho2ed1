/* src/retangulo.c
 *
 * Implementação do TAD Retângulo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulo.h"

/* Estrutura interna do retângulo */
typedef struct retangulo_internal
{
    int id;
    double x;
    double y;
    double largura;
    double altura;
    char corBorda[20];
    char corPreenchimento[20];
} RetanguloInternal;

/**
 * Cria um retângulo.
 */
Retangulo criaRetangulo(int id, double x, double y, double largura, double altura,
                        const char *corBorda, const char *corPreenchimento)
{
    RetanguloInternal *r = (RetanguloInternal *)malloc(sizeof(RetanguloInternal));
    if (r == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para retângulo.\n");
        return NULL;
    }

    r->id = id;
    r->x = x;
    r->y = y;
    r->largura = largura;
    r->altura = altura;
    strncpy(r->corBorda, corBorda, 19);
    r->corBorda[19] = '\0';
    strncpy(r->corPreenchimento, corPreenchimento, 19);
    r->corPreenchimento[19] = '\0';

    return (Retangulo)r;
}

/**
 * Cria uma cópia de um retângulo.
 */
Retangulo clonaRetangulo(Retangulo retangulo)
{
    RetanguloInternal *r = (RetanguloInternal *)retangulo;
    if (r == NULL)
    {
        return NULL;
    }
    return criaRetangulo(r->id, r->x, r->y,
                         r->largura, r->altura,
                         r->corBorda, r->corPreenchimento);
}

/**
 * Libera memória de um retângulo.
 */
void destroiRetangulo(Retangulo retangulo)
{
    RetanguloInternal *r = (RetanguloInternal *)retangulo;
    if (r != NULL)
    {
        free(r);
    }
}

/**
 * Getters
 */
int getRetanguloId(Retangulo retangulo)
{
    RetanguloInternal *r = (RetanguloInternal *)retangulo;
    return r ? r->id : -1;
}

double getRetanguloX(Retangulo retangulo)
{
    RetanguloInternal *r = (RetanguloInternal *)retangulo;
    return r ? r->x : 0.0;
}

double getRetanguloY(Retangulo retangulo)
{
    RetanguloInternal *r = (RetanguloInternal *)retangulo;
    return r ? r->y : 0.0;
}

double getRetanguloLargura(Retangulo retangulo)
{
    RetanguloInternal *r = (RetanguloInternal *)retangulo;
    return r ? r->largura : 0.0;
}

double getRetanguloAltura(Retangulo retangulo)
{
    RetanguloInternal *r = (RetanguloInternal *)retangulo;
    return r ? r->altura : 0.0;
}

const char *getRetanguloCorBorda(Retangulo retangulo)
{
    RetanguloInternal *r = (RetanguloInternal *)retangulo;
    return r ? r->corBorda : "";
}

const char *getRetanguloCorPreenchimento(Retangulo retangulo)
{
    RetanguloInternal *r = (RetanguloInternal *)retangulo;
    return r ? r->corPreenchimento : "";
}

/**
 * Setters
 */
void setRetanguloPosicao(Retangulo retangulo, double x, double y)
{
    RetanguloInternal *r = (RetanguloInternal *)retangulo;
    if (r != NULL)
    {
        r->x = x;
        r->y = y;
    }
}

/**
 * Verifica se dois retângulos se sobrepõem (colisão AABB).
 */
int retangulosSobrepoe(Retangulo retangulo1, Retangulo retangulo2)
{
    RetanguloInternal *r1 = (RetanguloInternal *)retangulo1;
    RetanguloInternal *r2 = (RetanguloInternal *)retangulo2;
    if (r1 == NULL || r2 == NULL)
    {
        return 0;
    }

    double x1_min = r1->x;
    double x1_max = r1->x + r1->largura;
    double y1_min = r1->y;
    double y1_max = r1->y + r1->altura;

    double x2_min = r2->x;
    double x2_max = r2->x + r2->largura;
    double y2_min = r2->y;
    double y2_max = r2->y + r2->altura;

    return !(x1_max < x2_min || x2_max < x1_min ||
             y1_max < y2_min || y2_max < y1_min);
}

/**
 * Define as cores do retângulo.
 */
void setRetanguloCores(Retangulo retangulo, const char *corBorda, const char *corPreenchimento)
{
    RetanguloInternal *r = (RetanguloInternal *)retangulo;
    if (r == NULL)
        return;

    if (corBorda != NULL)
    {
        strncpy(r->corBorda, corBorda, 19);
        r->corBorda[19] = '\0';
    }

    if (corPreenchimento != NULL)
    {
        strncpy(r->corPreenchimento, corPreenchimento, 19);
        r->corPreenchimento[19] = '\0';
    }
}
