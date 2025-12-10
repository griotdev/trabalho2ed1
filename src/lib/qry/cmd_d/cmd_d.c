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
    if (poligono == NULL || num_vertices == NULL) return NULL;
    
    *num_vertices = poligono_num_vertices(poligono);
    if (*num_vertices < 3) return NULL;
    
    double *vertices = (double*)malloc(*num_vertices * 2 * sizeof(double));
    if (vertices == NULL) return NULL;
    
    for (int i = 0; i < *num_vertices; i++)
    {
        Ponto p = poligono_obter_vertice(poligono, i);
        if (p != NULL)
        {
            vertices[i * 2] = get_ponto_x(p);
            vertices[i * 2 + 1] = get_ponto_y(p);
        }
    }
    
    return vertices;
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
static void gerar_relatorio_txt(const char *caminho, Lista formas_visiveis)
{
    FILE *arquivo = fopen(caminho, "a"); /* Append */
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro: não foi possível abrir %s\n", caminho);
        return;
    }
    
    fprintf(arquivo, "d:\n");
    
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
    int num_vertices;
    double *vertices = poligono_para_array(poligono, &num_vertices);
    
    /* Identifica formas visíveis */
    Lista formas_visiveis = criar_lista();
    int contador = 0;
    
    No atual = obter_primeiro(lista_formas);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        
        if (forma_visivel(forma, vertices, num_vertices))
        {
            inserir_fim(formas_visiveis, forma);
            contador++;
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Gera arquivos de saída */
    char caminho_txt[MAX_CAMINHO];
    snprintf(caminho_txt, MAX_CAMINHO, "%s/%s.txt", dir_saida, nome_base);
    gerar_relatorio_txt(caminho_txt, formas_visiveis);
    
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
        double margem = 10.0;
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
    
    if (vertices != NULL) free(vertices);
    
    /* Destroi listas temporárias */
    destruir_lista(formas_visiveis, NULL);
    
    return contador;
}
