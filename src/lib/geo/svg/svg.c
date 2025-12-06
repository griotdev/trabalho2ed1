/* svg.c
 *
 * Implementação do gerador de arquivos SVG.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svg.h"
#include "lista.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "formas.h"

/* ============================================================================
 * Estrutura Interna
 * ============================================================================ */

typedef struct svg_contexto_internal
{
    FILE *arquivo;
    double min_x;
    double min_y;
    double largura;
    double altura;
} SvgContextoInternal;

/* ============================================================================
 * Funções Auxiliares
 * ============================================================================ */

/**
 * Escreve o header do arquivo SVG.
 */
static void escrever_header(SvgContextoInternal *ctx)
{
    fprintf(ctx->arquivo, 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<svg xmlns=\"http://www.w3.org/2000/svg\" "
            "viewBox=\"%.2f %.2f %.2f %.2f\">\n",
            ctx->min_x, ctx->min_y, ctx->largura, ctx->altura);
    
    /* Fundo branco opcional */
    fprintf(ctx->arquivo,
            "  <!-- Fundo -->\n"
            "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" "
            "fill=\"white\" stroke=\"none\"/>\n\n",
            ctx->min_x, ctx->min_y, ctx->largura, ctx->altura);
}

/**
 * Converte âncora do texto para text-anchor SVG.
 */
static const char* converter_ancora(const char *ancora)
{
    if (ancora == NULL || ancora[0] == '\0')
    {
        return "start";
    }
    
    switch (ancora[0])
    {
        case 'i': return "start";   /* início */
        case 'm': return "middle";  /* meio */
        case 'f': return "end";     /* fim */
        default: return "start";
    }
}

/**
 * Converte font-weight do parser para SVG.
 */
static const char* converter_font_weight(const char *weight)
{
    if (weight == NULL) return "normal";
    
    if (strcmp(weight, "b") == 0 || strcmp(weight, "bold") == 0)
        return "bold";
    if (strcmp(weight, "b+") == 0 || strcmp(weight, "bolder") == 0)
        return "bolder";
    if (strcmp(weight, "l") == 0 || strcmp(weight, "lighter") == 0)
        return "lighter";
    if (strcmp(weight, "n") == 0 || strcmp(weight, "normal") == 0)
        return "normal";
    
    return weight;
}

/* ============================================================================
 * Implementação das Funções Públicas
 * ============================================================================ */

SvgContexto criar_svg(const char *caminho_arquivo, double largura, double altura)
{
    return criar_svg_viewbox(caminho_arquivo, 0, 0, largura, altura);
}

SvgContexto criar_svg_viewbox(const char *caminho_arquivo,
                               double min_x, double min_y,
                               double largura, double altura)
{
    if (caminho_arquivo == NULL)
    {
        fprintf(stderr, "Erro: caminho do arquivo SVG é NULL\n");
        return NULL;
    }
    
    SvgContextoInternal *ctx = (SvgContextoInternal*)malloc(sizeof(SvgContextoInternal));
    if (ctx == NULL)
    {
        fprintf(stderr, "Erro: falha ao alocar contexto SVG\n");
        return NULL;
    }
    
    ctx->arquivo = fopen(caminho_arquivo, "w");
    if (ctx->arquivo == NULL)
    {
        fprintf(stderr, "Erro: não foi possível criar arquivo: %s\n", caminho_arquivo);
        free(ctx);
        return NULL;
    }
    
    ctx->min_x = min_x;
    ctx->min_y = min_y;
    ctx->largura = largura;
    ctx->altura = altura;
    
    escrever_header(ctx);
    
    return (SvgContexto)ctx;
}

void finalizar_svg(SvgContexto svg)
{
    SvgContextoInternal *ctx = (SvgContextoInternal*)svg;
    if (ctx == NULL) return;
    
    if (ctx->arquivo != NULL)
    {
        fprintf(ctx->arquivo, "</svg>\n");
        fclose(ctx->arquivo);
    }
    
    free(ctx);
}

/* ============================================================================
 * Funções de Desenho
 * ============================================================================ */

void svg_desenhar_circulo(SvgContexto svg, Circulo c)
{
    SvgContextoInternal *ctx = (SvgContextoInternal*)svg;
    if (ctx == NULL || ctx->arquivo == NULL || c == NULL) return;
    
    fprintf(ctx->arquivo,
            "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" "
            "stroke=\"%s\" fill=\"%s\" stroke-width=\"1\"/>\n",
            getCirculoX(c),
            getCirculoY(c),
            getCirculoRaio(c),
            getCirculoCorBorda(c),
            getCirculoCorPreenchimento(c));
}

void svg_desenhar_retangulo(SvgContexto svg, Retangulo r)
{
    SvgContextoInternal *ctx = (SvgContextoInternal*)svg;
    if (ctx == NULL || ctx->arquivo == NULL || r == NULL) return;
    
    fprintf(ctx->arquivo,
            "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" "
            "stroke=\"%s\" fill=\"%s\" stroke-width=\"1\"/>\n",
            getRetanguloX(r),
            getRetanguloY(r),
            getRetanguloLargura(r),
            getRetanguloAltura(r),
            getRetanguloCorBorda(r),
            getRetanguloCorPreenchimento(r));
}

void svg_desenhar_linha(SvgContexto svg, Linha l)
{
    SvgContextoInternal *ctx = (SvgContextoInternal*)svg;
    if (ctx == NULL || ctx->arquivo == NULL || l == NULL) return;
    
    fprintf(ctx->arquivo,
            "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" "
            "stroke=\"%s\" stroke-width=\"1\"/>\n",
            getLinhaX1(l),
            getLinhaY1(l),
            getLinhaX2(l),
            getLinhaY2(l),
            getLinhaCor(l));
}

void svg_desenhar_texto(SvgContexto svg, Texto t)
{
    SvgContextoInternal *ctx = (SvgContextoInternal*)svg;
    if (ctx == NULL || ctx->arquivo == NULL || t == NULL) return;
    
    const char *ancora_svg = converter_ancora(getTextoAncora(t));
    const char *weight_svg = converter_font_weight(getTextoFontWeight(t));
    
    fprintf(ctx->arquivo,
            "  <text x=\"%.2f\" y=\"%.2f\" "
            "font-family=\"%s\" font-weight=\"%s\" font-size=\"%.2f\" "
            "text-anchor=\"%s\" "
            "stroke=\"%s\" fill=\"%s\">"
            "%s</text>\n",
            getTextoX(t),
            getTextoY(t),
            getTextoFontFamily(t),
            weight_svg,
            getTextoFontSize(t),
            ancora_svg,
            getTextoCorBorda(t),
            getTextoCorPreenchimento(t),
            getTextoConteudo(t));
}

void svg_desenhar_forma(SvgContexto svg, Forma *forma)
{
    if (svg == NULL || forma == NULL) return;
    
    TipoForma tipo = getFormaTipo(forma);
    void *dados = getFormaDados(forma);
    
    switch (tipo)
    {
        case TIPO_CIRCULO:
            svg_desenhar_circulo(svg, (Circulo)dados);
            break;
        case TIPO_RETANGULO:
            svg_desenhar_retangulo(svg, (Retangulo)dados);
            break;
        case TIPO_LINHA:
            svg_desenhar_linha(svg, (Linha)dados);
            break;
        case TIPO_TEXTO:
            svg_desenhar_texto(svg, (Texto)dados);
            break;
        default:
            fprintf(stderr, "Aviso: tipo de forma desconhecido: %d\n", tipo);
            break;
    }
}

void svg_desenhar_lista(SvgContexto svg, Lista lista)
{
    SvgContextoInternal *ctx = (SvgContextoInternal*)svg;
    if (ctx == NULL || lista == NULL) return;
    
    fprintf(ctx->arquivo, "  <!-- Formas do cenário -->\n");
    
    No atual = obter_primeiro(lista);
    while (atual != NULL)
    {
        Forma *forma = (Forma*)obter_elemento(atual);
        svg_desenhar_forma(svg, forma);
        atual = obter_proximo(atual);
    }
    
    fprintf(ctx->arquivo, "\n");
}

void svg_desenhar_poligono(SvgContexto svg, double *pontos, int num_pontos,
                           const char *cor_borda, const char *cor_preenchimento,
                           double opacidade)
{
    SvgContextoInternal *ctx = (SvgContextoInternal*)svg;
    if (ctx == NULL || ctx->arquivo == NULL || pontos == NULL || num_pontos < 3)
        return;
    
    fprintf(ctx->arquivo, "  <polygon points=\"");
    
    for (int i = 0; i < num_pontos; i++)
    {
        fprintf(ctx->arquivo, "%.2f,%.2f", pontos[i * 2], pontos[i * 2 + 1]);
        if (i < num_pontos - 1)
        {
            fprintf(ctx->arquivo, " ");
        }
    }
    
    fprintf(ctx->arquivo, "\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"%.2f\" "
            "stroke-width=\"1\"/>\n",
            cor_borda ? cor_borda : "black",
            cor_preenchimento ? cor_preenchimento : "none",
            opacidade);
}

void svg_comentario(SvgContexto svg, const char *texto)
{
    SvgContextoInternal *ctx = (SvgContextoInternal*)svg;
    if (ctx == NULL || ctx->arquivo == NULL || texto == NULL) return;
    
    fprintf(ctx->arquivo, "  <!-- %s -->\n", texto);
}
