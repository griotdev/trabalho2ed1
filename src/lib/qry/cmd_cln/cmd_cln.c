/* cmd_cln.c
 *
 * Implementação do comando 'cln' (bomba de clonagem)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_cln.h"
#include "lista.h"
#include "ponto.h"
#include "segmento.h"
#include "visibilidade.h"
#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "svg.h"
#include "calculos.h"

#define MAX_CAMINHO 1024

/* ============================================================================
 * Funções Auxiliares
 * ============================================================================ */

/**
 * Converte polígono de visibilidade para array de coordenadas.
 */
static double* poligono_para_array(PoligonoVisibilidade poligono, int *num_vertices)
{
    return poligono_get_vertices_ref((Poligono)poligono, num_vertices);
}

/**
 * Verifica se uma forma está dentro do polígono de visibilidade.
 */
static int forma_visivel(Forma forma, double *vertices, int num_vertices)
{
    if (forma == NULL || vertices == NULL || num_vertices < 3)
    {
        return 0;
    }
    
    int visivel = forma_no_poligono(forma, vertices, num_vertices);
    
    return visivel;
}

/**
 * Clona uma forma e aplica deslocamento.
 * O novo ID é passado mas não usado diretamente (forma tem ID interno).
 */
static Forma clonar_forma_com_deslocamento(Forma forma, double dx, double dy, int novo_id)
{
    TipoForma tipo = getFormaTipo(forma);
    void *dados = getFormaDados(forma);
    void *clone_dados = NULL;
    
    (void)novo_id; /* ID não é modificável diretamente nas formas clonadas */
    
    switch (tipo)
    {
        case TIPO_CIRCULO:
        {
            Circulo c = (Circulo)dados;
            clone_dados = clonaCirculo(c);
            if (clone_dados)
            {
                setCirculoPosicao((Circulo)clone_dados,
                                   getCirculoX(c) + dx,
                                   getCirculoY(c) + dy);
            }
            break;
        }
        
        case TIPO_RETANGULO:
        {
            Retangulo r = (Retangulo)dados;
            clone_dados = clonaRetangulo(r);
            if (clone_dados)
            {
                setRetanguloPosicao((Retangulo)clone_dados,
                                     getRetanguloX(r) + dx,
                                     getRetanguloY(r) + dy);
            }
            break;
        }
        
        case TIPO_LINHA:
        {
            Linha l = (Linha)dados;
            clone_dados = clonaLinha(l);
            if (clone_dados)
            {
                setLinhaPosicao((Linha)clone_dados, dx, dy);
            }
            break;
        }
        
        case TIPO_TEXTO:
        {
            Texto t = (Texto)dados;
            clone_dados = clonaTexto(t);
            if (clone_dados)
            {
                setTextoPosicao((Texto)clone_dados,
                                 getTextoX(t) + dx,
                                 getTextoY(t) + dy);
            }
            break;
        }
    }
    
    if (clone_dados == NULL) return NULL;
    
    return criaForma(tipo, clone_dados);
}

/**
 * Gera o arquivo de relatório TXT.
 */
static void gerar_relatorio_txt(const char *caminho, Lista formas_clonadas, 
                                 double dx, double dy)
{
    FILE *arquivo = fopen(caminho, "a");
    if (arquivo == NULL) return;
    
    fprintf(arquivo, "cln: dx=%.2f dy=%.2f\n", dx, dy);
    
    No atual = obter_primeiro(formas_clonadas);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        int id = getFormaId(forma);
        TipoForma tipo = getFormaTipo(forma);
        
        const char *tipo_str;
        switch (tipo)
        {
            case TIPO_CIRCULO: tipo_str = "circulo"; break;
            case TIPO_RETANGULO: tipo_str = "retangulo"; break;
            case TIPO_LINHA: tipo_str = "linha"; break;
            case TIPO_TEXTO: tipo_str = "texto"; break;
            default: tipo_str = "desconhecido";
        }
        
        fprintf(arquivo, "  %d %s (clone)\n", id, tipo_str);
        atual = obter_proximo(atual);
    }
    
    fclose(arquivo);
}

/* ============================================================================
 * Implementação
 * ============================================================================ */

int executar_cmd_cln(Ponto origem,
                     Lista lista_formas,
                     Lista lista_anteparos,
                     double dx,
                     double dy,
                     const char *dir_saida,
                     const char *nome_base,
                     const char *sufixo,
                     double bbox[4],
                     int *proximo_id,
                     const char *tipo_ordenacao,
                     int limiar_insertion,
                     Lista acumulador_poligonos,
                     Lista acumulador_bombas)
{
    if (origem == NULL || lista_formas == NULL || proximo_id == NULL)
    {
        return 0;
    }
    
    /* Calcula polígono de visibilidade */
    PoligonoVisibilidade poligono = calcular_visibilidade(
        origem, lista_anteparos,
        bbox[0], bbox[1], bbox[2], bbox[3],
        tipo_ordenacao, limiar_insertion
    );
    
    if (poligono == NULL)
    {
        fprintf(stderr, "Aviso: falha ao calcular visibilidade\n");
        return 0;
    }
    
    /* Converte polígono para array de vértices */
    int num_vertices = 0;
    double *vertices = poligono_para_array(poligono, &num_vertices);
    
    if (vertices == NULL)
    {
        if (acumulador_poligonos == NULL) destruir_poligono_visibilidade(poligono);
        return 0;
    }
    
    /* Primeiro, coleta formas visíveis (não modifica lista durante iteração) */
    Lista formas_para_clonar = criar_lista();
    
    No atual = obter_primeiro(lista_formas);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        
        if (getFormaAtiva(forma) && forma_visivel(forma, vertices, num_vertices))
        {
            inserir_fim(formas_para_clonar, forma);
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Clona e adiciona à lista principal */
    Lista clones = criar_lista();
    int contador = 0;
    
    atual = obter_primeiro(formas_para_clonar);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        
        Forma clone = clonar_forma_com_deslocamento(forma, dx, dy, *proximo_id);
        if (clone != NULL)
        {
            inserir_fim(lista_formas, clone);
            inserir_fim(clones, clone);
            (*proximo_id)++;
            contador++;
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Gera relatório */
    char caminho_txt[MAX_CAMINHO];
    snprintf(caminho_txt, MAX_CAMINHO, "%s/%s.txt", dir_saida, nome_base);
    gerar_relatorio_txt(caminho_txt, clones, dx, dy);
    
    /* SVG: Gerencia saída baseada no sufixo */
    if (strcmp(sufixo, "-") == 0)
    {
        /* Acumula para desenho final */
        if (acumulador_poligonos != NULL && acumulador_bombas != NULL)
        {
            inserir_fim(acumulador_poligonos, poligono);
            
            /* Clona o ponto para salvar na lista */
            Ponto bomba_clone = criar_ponto(get_ponto_x(origem), get_ponto_y(origem));
            inserir_fim(acumulador_bombas, bomba_clone);
        }
        else
        {
            destruir_poligono_visibilidade(poligono);
        }
    }
    else
    {
        /* Cria arquivo específico para este comando */
        char caminho_svg[MAX_CAMINHO];
        snprintf(caminho_svg, MAX_CAMINHO, "%s/%s-%s.svg", dir_saida, nome_base, sufixo);
        
        /* Usa viewBox com as dimensões do cenário */
        double margem = 40.0;
        SvgContexto svg = criar_svg_viewbox(
            caminho_svg,
            bbox[0] - margem,
            bbox[1] - margem,
            (bbox[2] - bbox[0]) + 2 * margem,
            (bbox[3] - bbox[1]) + 2 * margem
        );
        
        if (svg != NULL)
        {
            /* 1. Desenha as formas originais (com clones) */
            svg_comentario(svg, "Formas originais do cenário");
            svg_desenhar_lista(svg, lista_formas);
            
            /* 2. Desenha os anteparos */
            if (lista_anteparos != NULL && !lista_vazia(lista_anteparos))
            {
                svg_desenhar_lista_segmentos(svg, lista_anteparos);
            }
            
            /* 3. Desenha a região de visibilidade */
            svg_desenhar_poligono_visibilidade(svg, poligono, 
                                                "none", "#FFFF00", 0.3);
            
            /* 4. Desenha a bomba */
            svg_desenhar_bomba(svg, get_ponto_x(origem), get_ponto_y(origem), 
                               5.0, "#FF0000");
            
            finalizar_svg(svg);
            printf("          SVG gerado: %s\n", caminho_svg);
        }
        /* Destroi poligono */
        destruir_poligono_visibilidade(poligono);
    }
    // if (vertices != NULL) free(vertices);
    
    (void)sufixo;
    
    destruir_lista(formas_para_clonar, NULL);
    destruir_lista(clones, NULL);
    
    return contador;
}

