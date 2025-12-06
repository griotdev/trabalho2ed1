#include <stdio.h>
#include <stdlib.h>
#include "formas.h"

struct forma
{
    TipoForma tipo;
    void *dados;
};

Forma *criaForma(TipoForma tipo, void *dados)
{
    Forma *f = (Forma *)malloc(sizeof(Forma));
    if (f == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para forma genérica.\n");
        return NULL;
    }

    f->tipo = tipo;
    f->dados = dados;

    return f;
}

TipoForma getFormaTipo(Forma *f)
{
    if (!f)
    {
        return (TipoForma)-1; // Sentinel for inválido; avoids signedness warning
    }
    return f->tipo;
}

void *getFormaDados(Forma *f)
{
    return f ? f->dados : NULL;
}

int getFormaId(Forma *f)
{
    if (f == NULL || f->dados == NULL)
    {
        return -1;
    }

    switch (f->tipo)
    {
    case TIPO_CIRCULO:
        return getCirculoId((Circulo *)f->dados);
    case TIPO_RETANGULO:
        return getRetanguloId((Retangulo *)f->dados);
    case TIPO_LINHA:
        return getLinhaId((Linha *)f->dados);
    case TIPO_TEXTO:
        return getTextoId((Texto *)f->dados);
    default:
        return -1;
    }
}

void destroiForma(Forma *forma)
{
    if (forma == NULL)
    {
        return;
    }

    if (forma->dados != NULL)
    {
        switch (forma->tipo)
        {
        case TIPO_CIRCULO:
            destroiCirculo((Circulo *)forma->dados);
            break;
        case TIPO_RETANGULO:
            destroiRetangulo((Retangulo *)forma->dados);
            break;
        case TIPO_LINHA:
            destroiLinha((Linha *)forma->dados);
            break;
        case TIPO_TEXTO:
            destroiTexto((Texto *)forma->dados);
            break;
        }
    }

    free(forma);
}

void setFormaPosicao(Forma *forma, double x, double y)
{
    if (forma == NULL || forma->dados == NULL)
    {
        return;
    }

    switch (forma->tipo)
    {
    case TIPO_CIRCULO:
        setCirculoPosicao((Circulo *)forma->dados, x, y);
        break;
    case TIPO_RETANGULO:
        setRetanguloPosicao((Retangulo *)forma->dados, x, y);
        break;
    case TIPO_LINHA:
    {
        double x1 = getLinhaX1((Linha *)forma->dados);
        double y1 = getLinhaY1((Linha *)forma->dados);
        double dx = x - x1;
        double dy = y - y1;
        setLinhaPosicao((Linha *)forma->dados, dx, dy);
    }
    break;
    case TIPO_TEXTO:
        setTextoPosicao((Texto *)forma->dados, x, y);
        break;
    }
}

Forma *clonaForma(Forma *forma)
{
    if (forma == NULL || forma->dados == NULL)
    {
        return NULL;
    }

    void *clone_data = NULL;

    switch (forma->tipo)
    {
    case TIPO_CIRCULO:
        clone_data = clonaCirculo((Circulo *)forma->dados);
        break;
    case TIPO_RETANGULO:
        clone_data = clonaRetangulo((Retangulo *)forma->dados);
        break;
    case TIPO_LINHA:
        clone_data = clonaLinha((Linha *)forma->dados);
        break;
    case TIPO_TEXTO:
        clone_data = clonaTexto((Texto *)forma->dados);
        break;
    }

    if (clone_data == NULL)
    {
        return NULL;
    }

    return criaForma(forma->tipo, clone_data);
}
