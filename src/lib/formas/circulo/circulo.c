/* src/circulo.c
 *
 * Implementação do TAD Círculo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "circulo.h"

/* Estrutura interna do círculo */
typedef struct circulo_internal
{
    int id;
    double x;
    double y;
    double raio;
    char corBorda[20];
    char corPreenchimento[20];
} CirculoInternal;

/**
 * Cria um círculo.
 */
Circulo criaCirculo(int id, double x, double y, double raio,
                    const char *corBorda, const char *corPreenchimento)
{
    CirculoInternal *c = (CirculoInternal *)malloc(sizeof(CirculoInternal));
    if (c == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para círculo.\n");
        return NULL;
    }

    c->id = id;
    c->x = x;
    c->y = y;
    c->raio = raio;
    strncpy(c->corBorda, corBorda, 19);
    c->corBorda[19] = '\0';
    strncpy(c->corPreenchimento, corPreenchimento, 19);
    c->corPreenchimento[19] = '\0';

    return (Circulo)c;
}

/**
 * Cria uma cópia de um círculo.
 */
Circulo clonaCirculo(Circulo circulo)
{
    CirculoInternal *c = (CirculoInternal *)circulo;
    if (c == NULL)
    {
        return NULL;
    }
    return criaCirculo(c->id, c->x, c->y, c->raio,
                       c->corBorda, c->corPreenchimento);
}

/**
 * Libera memória de um círculo.
 */
void destroiCirculo(Circulo circulo)
{
    CirculoInternal *c = (CirculoInternal *)circulo;
    if (c != NULL)
    {
        free(c);
    }
}

/**
 * Getters
 */
int getCirculoId(Circulo circulo)
{
    CirculoInternal *c = (CirculoInternal *)circulo;
    return c ? c->id : -1;
}

double getCirculoX(Circulo circulo)
{
    CirculoInternal *c = (CirculoInternal *)circulo;
    return c ? c->x : 0.0;
}

double getCirculoY(Circulo circulo)
{
    CirculoInternal *c = (CirculoInternal *)circulo;
    return c ? c->y : 0.0;
}

double getCirculoRaio(Circulo circulo)
{
    CirculoInternal *c = (CirculoInternal *)circulo;
    return c ? c->raio : 0.0;
}

const char *getCirculoCorBorda(Circulo circulo)
{
    CirculoInternal *c = (CirculoInternal *)circulo;
    return c ? c->corBorda : "";
}

const char *getCirculoCorPreenchimento(Circulo circulo)
{
    CirculoInternal *c = (CirculoInternal *)circulo;
    return c ? c->corPreenchimento : "";
}

/**
 * Setters
 */
void setCirculoPosicao(Circulo circulo, double x, double y)
{
    CirculoInternal *c = (CirculoInternal *)circulo;
    if (c != NULL)
    {
        c->x = x;
        c->y = y;
    }
}

/**
 * Define as cores do círculo.
 */
void setCirculoCores(Circulo circulo, const char *corBorda, const char *corPreenchimento)
{
    CirculoInternal *c = (CirculoInternal *)circulo;
    if (c == NULL)
        return;

    if (corBorda != NULL)
    {
        strncpy(c->corBorda, corBorda, 19);
        c->corBorda[19] = '\0';
    }

    if (corPreenchimento != NULL)
    {
        strncpy(c->corPreenchimento, corPreenchimento, 19);
        c->corPreenchimento[19] = '\0';
    }
}

/**
 * Verifica se dois círculos se sobrepõem.
 */
int circulosSobrepoe(Circulo circulo1, Circulo circulo2)
{
    CirculoInternal *c1 = (CirculoInternal *)circulo1;
    CirculoInternal *c2 = (CirculoInternal *)circulo2;
    if (c1 == NULL || c2 == NULL)
    {
        return 0;
    }

    double dx = c1->x - c2->x;
    double dy = c1->y - c2->y;
    double distancia = sqrt(dx * dx + dy * dy);
    double somaRaios = c1->raio + c2->raio;

    return distancia < somaRaios;
}
