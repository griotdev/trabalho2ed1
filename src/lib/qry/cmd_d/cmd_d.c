/* cmd_d.c
 *
 * Implementação do comando 'd' (bomba de destruição)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_d.h"
#include "lista.h"
#include "ponto.h"
#include "segmento.h"
#include "visibilidade.h"
#include "formas.h"
#include "svg.h"
#include "calculos.h"

#define MAX_CAMINHO 1024

/* ============================================================================
 * Funções Auxiliares
 * ============================================================================ */

/**
 * Converte polígono de visibilidade para array de coordenadas.
 * Retorna array alocado [x0, y0, x1, y1, ...] que deve ser liberado pelo chamador.
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
    
    return forma_no_poligono(forma, vertices, num_vertices);
}

/**
 * Gera o arquivo de relatório TXT.
 */
static void gerar_relatorio_txt(const char *caminho, Lista formas_visiveis, 
                                Lista segmentos_destruidos, double x, double y)
{
    FILE *arquivo = fopen(caminho, "a"); /* Append */
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro: não foi possível abrir %s\n", caminho);
        return;
    }
    
    fprintf(arquivo, "d: x=%.2f y=%.2f\n", x, y);
    
    /* Log formas destruídas */
    No atual = obter_primeiro(formas_visiveis);
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
        
        fprintf(arquivo, "  %d %s\n", id, tipo_str);
        atual = obter_proximo(atual);
    }
    
    /* Log segmentos (anteparos) destruídos */
    atual = obter_primeiro(segmentos_destruidos);
    while (atual != NULL)
    {
        Segmento seg = (Segmento)obter_elemento(atual);
        int id = get_segmento_id(seg);
        int id_orig = get_segmento_id_original(seg);
        
        fprintf(arquivo, "  %d segmento (anteparo de %d)\n", id, id_orig);
        atual = obter_proximo(atual);
    }
    
    fclose(arquivo);
}

/* ============================================================================
 * Implementação
 * ============================================================================ */

int executar_cmd_d(Ponto origem,
                   Lista lista_formas,
                   Lista lista_anteparos,
                   const char *dir_saida,
                   const char *nome_base,

                   const char *sufixo,
                   double bbox[4],
                   const char *tipo_ordenacao,
                   int limiar_insertion,
                   Lista acumulador_poligonos,
                   Lista acumulador_bombas)
{
    if (origem == NULL || lista_formas == NULL)
    {
        return 0;
    }
    
    /* Cria lista para rastrear segmentos visíveis */
    Lista segmentos_visiveis = criar_lista();
    
    /* Calcula polígono de visibilidade COM rastreamento de segmentos */
    PoligonoVisibilidade poligono = calcular_visibilidade_com_segmentos(
        origem, lista_anteparos,
        bbox[0], bbox[1], bbox[2], bbox[3],
        tipo_ordenacao, limiar_insertion,
        segmentos_visiveis
    );
    
    if (poligono == NULL)
    {
        destruir_lista(segmentos_visiveis, NULL);
        fprintf(stderr, "Aviso: falha ao calcular visibilidade\n");
        return 0;
    }
    
    /* Converte polígono para array de vértices */
    int num_vertices;
    double *vertices = poligono_para_array(poligono, &num_vertices);
    
    /* Identifica formas visíveis */
    Lista formas_visiveis = criar_lista();
    int contador = 0;
    
    No atual = obter_primeiro(lista_formas);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        
        if (getFormaAtiva(forma) && forma_visivel(forma, vertices, num_vertices))
        {
            inserir_fim(formas_visiveis, forma);
            setFormaAtiva(forma, 0); /* Destrói a forma */
            contador++;
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Remove segmentos visíveis da lista de anteparos */
    Lista segmentos_destruidos = criar_lista(); /* Para log */
    No seg_atual = obter_primeiro(segmentos_visiveis);
    while (seg_atual != NULL)
    {
        Segmento seg = (Segmento)obter_elemento(seg_atual);
        
        /* Encontra e remove da lista de anteparos */
        No ant_atual = obter_primeiro(lista_anteparos);
        while (ant_atual != NULL)
        {
            No proximo = obter_proximo(ant_atual);
            Segmento ant_seg = (Segmento)obter_elemento(ant_atual);
            
            if (ant_seg == seg)
            {
                /* Salva referência para log antes de remover */
                inserir_fim(segmentos_destruidos, ant_seg);
                remover_no(lista_anteparos, ant_atual);
                break;
            }
            ant_atual = proximo;
        }
        
        seg_atual = obter_proximo(seg_atual);
    }
    
    destruir_lista(segmentos_visiveis, NULL);
    
    /* Gera arquivos de saída */
    char caminho_txt[MAX_CAMINHO];
    snprintf(caminho_txt, MAX_CAMINHO, "%s/%s.txt", dir_saida, nome_base);
    gerar_relatorio_txt(caminho_txt, formas_visiveis, segmentos_destruidos, 
                        get_ponto_x(origem), get_ponto_y(origem));
    
    /* Agora destrói os segmentos removidos */
    No seg_destruir = obter_primeiro(segmentos_destruidos);
    while (seg_destruir != NULL)
    {
        No prox = obter_proximo(seg_destruir);
        Segmento seg = (Segmento)obter_elemento(seg_destruir);
        destruir_segmento(seg);
        seg_destruir = prox;
    }
    destruir_lista(segmentos_destruidos, NULL);
    
    /* SVG: Gerencia saída baseada no sufixo */
    if (strcmp(sufixo, "-") == 0)
    {
        /* Acumula para desenho final */
        if (acumulador_poligonos != NULL && acumulador_bombas != NULL)
        {
            inserir_fim(acumulador_poligonos, poligono);
            
            /* Clona o ponto para salvar na lista (pois origem será destruída) */
            Ponto bomba_clone = criar_ponto(get_ponto_x(origem), get_ponto_y(origem));
            inserir_fim(acumulador_bombas, bomba_clone);
            
            /* NÃO destroi poligono aqui, será destruído no final do parser */
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
            /* 1. Desenha as formas originais */
            svg_comentario(svg, "Formas originais do cenário");
            svg_desenhar_lista(svg, lista_formas);
            
            /* 2. Desenha os anteparos (segmentos bloqueantes) */
            if (lista_anteparos != NULL && !lista_vazia(lista_anteparos))
            {
                svg_desenhar_lista_segmentos(svg, lista_anteparos);
            }
            
            /* 3. Desenha a região de visibilidade (polígono semi-transparente) */
            svg_desenhar_poligono_visibilidade(svg, poligono, 
                                                "none", "#FFFF00", 0.3);
            
            /* 4. Desenha a bomba (ponto de origem) */
            svg_desenhar_bomba(svg, get_ponto_x(origem), get_ponto_y(origem), 
                               5.0, "#FF0000");
            
            finalizar_svg(svg);
            printf("          SVG gerado: %s\n", caminho_svg);
        }
        
        /* Destroi poligono (não foi acumulado) */
        destruir_poligono_visibilidade(poligono);
    }
    
    /* vertices points to internal valid memory, do NOT free */
    // if (vertices != NULL) free(vertices);
    
    /* Destroi listas temporárias */
    destruir_lista(formas_visiveis, NULL);
    
    return contador;
}
