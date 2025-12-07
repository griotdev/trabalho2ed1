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

#define MAX_CAMINHO 1024

/* ============================================================================
 * Funções Auxiliares
 * ============================================================================ */

/**
 * Verifica se uma forma está dentro do polígono de visibilidade.
 * Simplificação: verifica se o centro/âncora da forma está no polígono.
 */
static int forma_visivel(Forma forma, PoligonoVisibilidade poligono)
{
    /* TODO: Implementar verificação precisa de ponto no polígono */
    /* Por enquanto, retorna 1 (todas visíveis) para teste */
    (void)forma;
    (void)poligono;
    return 1;
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
                   double bbox[4])
{
    if (origem == NULL || lista_formas == NULL)
    {
        return 0;
    }
    
    /* Calcula polígono de visibilidade */
    PoligonoVisibilidade poligono = calcular_visibilidade(
        origem, lista_anteparos,
        bbox[0], bbox[1], bbox[2], bbox[3]
    );
    
    if (poligono == NULL)
    {
        fprintf(stderr, "Aviso: falha ao calcular visibilidade\n");
        return 0;
    }
    
    /* Identifica formas visíveis */
    Lista formas_visiveis = criar_lista();
    int contador = 0;
    
    No atual = obter_primeiro(lista_formas);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        
        if (forma_visivel(forma, poligono))
        {
            inserir_fim(formas_visiveis, forma);
            contador++;
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Gera arquivos de saída */
    char caminho_txt[MAX_CAMINHO];
    snprintf(caminho_txt, MAX_CAMINHO, "%s/%s-consultas.txt", dir_saida, nome_base);
    gerar_relatorio_txt(caminho_txt, formas_visiveis);
    
    /* SVG: se sufixo é "-", desenha no principal; senão, cria novo arquivo */
    if (strcmp(sufixo, "-") != 0)
    {
        char caminho_svg[MAX_CAMINHO];
        snprintf(caminho_svg, MAX_CAMINHO, "%s/%s-%s.svg", dir_saida, nome_base, sufixo);
        
        SvgContexto svg = criar_svg(caminho_svg, 
                                     bbox[2] - bbox[0] + 20,
                                     bbox[3] - bbox[1] + 20);
        if (svg != NULL)
        {
            /* Desenha polígono de visibilidade */
            Lista vertices = poligono_obter_vertices(poligono);
            if (vertices != NULL && !lista_vazia(vertices))
            {
                /* TODO: desenhar polígono no SVG */
            }
            
            finalizar_svg(svg);
        }
    }
    
    /* Limpa (não destrói as formas, só a lista temporária) */
    destruir_lista(formas_visiveis, NULL);
    destruir_poligono_visibilidade(poligono);
    
    return contador;
}
