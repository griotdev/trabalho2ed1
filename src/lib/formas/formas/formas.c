/* formas.c
 *
 * Implementação do TAD Forma genérica.
 */

#include <stdio.h>
#include <stdlib.h>
#include "formas.h"

/* ============================================================================
 * Estrutura Interna (Ponteiro Opaco)
 * ============================================================================ */

typedef struct forma_internal
{
    TipoForma tipo;
    void *dados;
    int ativa;  /* 1 = forma deve ser desenhada, 0 = forma inativa (convertida em anteparo) */
} FormaInternal;

/* ============================================================================
 * Implementação das Funções Públicas
 * ============================================================================ */

Forma criaForma(TipoForma tipo, void *dados)
{
    FormaInternal *f = (FormaInternal*)malloc(sizeof(FormaInternal));
    if (f == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para forma genérica.\n");
        return NULL;
    }

    f->tipo = tipo;
    f->dados = dados;
    f->ativa = 1;  /* Por padrão, forma está ativa */

    return (Forma)f;
}

TipoForma getFormaTipo(Forma forma)
{
    FormaInternal *f = (FormaInternal*)forma;
    if (f == NULL)
    {
        return (TipoForma)-1;
    }
    return f->tipo;
}

void* getFormaDados(Forma forma)
{
    FormaInternal *f = (FormaInternal*)forma;
    return f ? f->dados : NULL;
}

int getFormaId(Forma forma)
{
    FormaInternal *f = (FormaInternal*)forma;
    if (f == NULL || f->dados == NULL)
    {
        return -1;
    }

    switch (f->tipo)
    {
        case TIPO_CIRCULO:
            return getCirculoId((Circulo)f->dados);
        case TIPO_RETANGULO:
            return getRetanguloId((Retangulo)f->dados);
        case TIPO_LINHA:
            return getLinhaId((Linha)f->dados);
        case TIPO_TEXTO:
            return getTextoId((Texto)f->dados);
        default:
            return -1;
    }
}

int getFormaAtiva(Forma forma)
{
    FormaInternal *f = (FormaInternal*)forma;
    if (f == NULL)
    {
        return 0;
    }
    return f->ativa;
}

void setFormaAtiva(Forma forma, int ativa)
{
    FormaInternal *f = (FormaInternal*)forma;
    if (f != NULL)
    {
        f->ativa = ativa;
    }
}

void destroiForma(Forma forma)
{
    FormaInternal *f = (FormaInternal*)forma;
    if (f == NULL)
    {
        return;
    }

    if (f->dados != NULL)
    {
        switch (f->tipo)
        {
            case TIPO_CIRCULO:
                destroiCirculo((Circulo)f->dados);
                break;
            case TIPO_RETANGULO:
                destroiRetangulo((Retangulo)f->dados);
                break;
            case TIPO_LINHA:
                destroiLinha((Linha)f->dados);
                break;
            case TIPO_TEXTO:
                destroiTexto((Texto)f->dados);
                break;
        }
    }

    free(f);
}

void setFormaPosicao(Forma forma, double x, double y)
{
    FormaInternal *f = (FormaInternal*)forma;
    if (f == NULL || f->dados == NULL)
    {
        return;
    }

    switch (f->tipo)
    {
        case TIPO_CIRCULO:
            setCirculoPosicao((Circulo)f->dados, x, y);
            break;
        case TIPO_RETANGULO:
            setRetanguloPosicao((Retangulo)f->dados, x, y);
            break;
        case TIPO_LINHA:
        {
            double x1 = getLinhaX1((Linha)f->dados);
            double y1 = getLinhaY1((Linha)f->dados);
            double dx = x - x1;
            double dy = y - y1;
            setLinhaPosicao((Linha)f->dados, dx, dy);
        }
        break;
        case TIPO_TEXTO:
            setTextoPosicao((Texto)f->dados, x, y);
            break;
    }
}

Forma clonaForma(Forma forma)
{
    FormaInternal *f = (FormaInternal*)forma;
    if (f == NULL || f->dados == NULL)
    {
        return NULL;
    }

    void *clone_data = NULL;

    switch (f->tipo)
    {
        case TIPO_CIRCULO:
            clone_data = clonaCirculo((Circulo)f->dados);
            break;
        case TIPO_RETANGULO:
            clone_data = clonaRetangulo((Retangulo)f->dados);
            break;
        case TIPO_LINHA:
            clone_data = clonaLinha((Linha)f->dados);
            break;
        case TIPO_TEXTO:
            clone_data = clonaTexto((Texto)f->dados);
            break;
    }

    if (clone_data == NULL)
    {
        return NULL;
    }

    return criaForma(f->tipo, clone_data);
}
