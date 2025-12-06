/* src/texto.c
 *
 * Implementação do TAD Texto.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texto.h"

/* Estrutura interna do texto */
typedef struct texto_internal
{
    int id;
    double x;
    double y;
    char corBorda[20];
    char corPreenchimento[20];
    char ancora[20];
    char *conteudo;
    char fontFamily[50];
    char fontWeight[20];
    double fontSize;
} TextoInternal;

/**
 * Cria um texto.
 */
Texto criaTexto(int id, double x, double y, const char *corBorda,
                const char *corPreenchimento, const char *ancora, const char *conteudo,
                const char *fontFamily, const char *fontWeight, double fontSize)
{
    TextoInternal *t = (TextoInternal *)malloc(sizeof(TextoInternal));
    if (t == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para texto.\n");
        return NULL;
    }

    t->id = id;
    t->x = x;
    t->y = y;
    strncpy(t->corBorda, corBorda, 19);
    t->corBorda[19] = '\0';
    strncpy(t->corPreenchimento, corPreenchimento, 19);
    t->corPreenchimento[19] = '\0';
    strncpy(t->ancora, ancora, 19);
    t->ancora[19] = '\0';

    t->conteudo = (char *)malloc(strlen(conteudo) + 1);
    if (t->conteudo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para conteúdo do texto.\n");
        free(t);
        return NULL;
    }
    strcpy(t->conteudo, conteudo);

    strncpy(t->fontFamily, fontFamily, 49);
    t->fontFamily[49] = '\0';
    strncpy(t->fontWeight, fontWeight, 19);
    t->fontWeight[19] = '\0';
    t->fontSize = fontSize;

    return (Texto)t;
}

/**
 * Cria uma cópia de um texto.
 */
Texto clonaTexto(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    if (t == NULL)
    {
        return NULL;
    }
    return criaTexto(t->id, t->x, t->y,
                     t->corBorda, t->corPreenchimento,
                     t->ancora, t->conteudo,
                     t->fontFamily, t->fontWeight, t->fontSize);
}

/**
 * Libera memória de um texto.
 */
void destroiTexto(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    if (t != NULL)
    {
        if (t->conteudo != NULL)
        {
            free(t->conteudo);
        }
        free(t);
    }
}

/**
 * Getters
 */
int getTextoId(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    return t ? t->id : -1;
}

double getTextoX(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    return t ? t->x : 0.0;
}

double getTextoY(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    return t ? t->y : 0.0;
}

const char *getTextoCorBorda(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    return t ? t->corBorda : "";
}

const char *getTextoCorPreenchimento(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    return t ? t->corPreenchimento : "";
}

const char *getTextoAncora(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    return t ? t->ancora : "";
}

const char *getTextoConteudo(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    return t ? t->conteudo : "";
}

const char *getTextoFontFamily(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    return t ? t->fontFamily : "sans-serif";
}

const char *getTextoFontWeight(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    return t ? t->fontWeight : "normal";
}

double getTextoFontSize(Texto texto)
{
    TextoInternal *t = (TextoInternal *)texto;
    return t ? t->fontSize : 12.0;
}

/**
 * Setters
 */
void setTextoPosicao(Texto texto, double x, double y)
{
    TextoInternal *t = (TextoInternal *)texto;
    if (t != NULL)
    {
        t->x = x;
        t->y = y;
    }
}

/**
 * Define as cores do texto.
 */
void setTextoCores(Texto texto, const char *corBorda, const char *corPreenchimento)
{
    TextoInternal *t = (TextoInternal *)texto;
    if (t == NULL)
        return;

    if (corBorda != NULL)
    {
        strncpy(t->corBorda, corBorda, 19);
        t->corBorda[19] = '\0';
    }

    if (corPreenchimento != NULL)
    {
        strncpy(t->corPreenchimento, corPreenchimento, 19);
        t->corPreenchimento[19] = '\0';
    }
}
