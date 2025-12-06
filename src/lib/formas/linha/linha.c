/* src/linha.c
 *
 * Implementação do TAD Linha.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linha.h"

/* Estrutura interna da linha */
typedef struct linha_internal
{
    int id;
    double x1;
    double y1;
    double x2;
    double y2;
    char cor[20];
} LinhaInternal;

/**
 * Cria uma linha.
 */
Linha criaLinha(int id, double x1, double y1, double x2, double y2, const char *cor)
{
    LinhaInternal *l = (LinhaInternal *)malloc(sizeof(LinhaInternal));
    if (l == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para linha.\n");
        return NULL;
    }

    l->id = id;
    l->x1 = x1;
    l->y1 = y1;
    l->x2 = x2;
    l->y2 = y2;
    strncpy(l->cor, cor, 19);
    l->cor[19] = '\0';

    return (Linha)l;
}

/**
 * Cria uma cópia de uma linha.
 */
Linha clonaLinha(Linha linha)
{
    LinhaInternal *l = (LinhaInternal *)linha;
    if (l == NULL)
    {
        return NULL;
    }
    return criaLinha(l->id, l->x1, l->y1,
                     l->x2, l->y2, l->cor);
}

/**
 * Libera memória de uma linha.
 */
void destroiLinha(Linha linha)
{
    LinhaInternal *l = (LinhaInternal *)linha;
    if (l != NULL)
    {
        free(l);
    }
}

/**
 * Getters
 */
int getLinhaId(Linha linha)
{
    LinhaInternal *l = (LinhaInternal *)linha;
    return l ? l->id : -1;
}

double getLinhaX1(Linha linha)
{
    LinhaInternal *l = (LinhaInternal *)linha;
    return l ? l->x1 : 0.0;
}

double getLinhaY1(Linha linha)
{
    LinhaInternal *l = (LinhaInternal *)linha;
    return l ? l->y1 : 0.0;
}

double getLinhaX2(Linha linha)
{
    LinhaInternal *l = (LinhaInternal *)linha;
    return l ? l->x2 : 0.0;
}

double getLinhaY2(Linha linha)
{
    LinhaInternal *l = (LinhaInternal *)linha;
    return l ? l->y2 : 0.0;
}

const char *getLinhaCor(Linha linha)
{
    LinhaInternal *l = (LinhaInternal *)linha;
    return l ? l->cor : "";
}

/**
 * Setters - desloca a linha
 */
void setLinhaPosicao(Linha linha, double dx, double dy)
{
    LinhaInternal *l = (LinhaInternal *)linha;
    if (l != NULL)
    {
        l->x1 += dx;
        l->y1 += dy;
        l->x2 += dx;
        l->y2 += dy;
    }
}
