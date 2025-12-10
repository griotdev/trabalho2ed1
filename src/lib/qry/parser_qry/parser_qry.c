/* parser_qry.c
 *
 * Implementação do parser de arquivos .qry
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser_qry.h"
#include "cmd_a.h"
#include "cmd_d.h"
#include "cmd_p.h"
#include "cmd_cln.h"
#include "ponto.h"
#include "lista.h"
#include "visibilidade.h"
#include "svg.h"
#include "formas.h"

#define MAX_LINHA 512

/* ============================================================================
 * Funções Auxiliares
 * ============================================================================ */

/**
 * Remove espaços do início e fim da string.
 */
static char* trim(char *str)
{
    if (str == NULL) return NULL;
    
    while (isspace((unsigned char)*str)) str++;
    if (*str == '\0') return str;
    
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    
    return str;
}

/* ============================================================================
 * Implementação
 * ============================================================================ */

int processar_arquivo_qry(const char *caminho_qry,
                          Lista lista_formas,
                          Lista lista_anteparos,
                          const char *dir_saida,
                          const char *sufixo_saida, // Renamed from nome_base
                          double bbox[4],
                          const char *tipo_ordenacao, // Renamed from algoritmo_ordenacao
                          int limiar_insertion) // Added
{
    if (caminho_qry == NULL)
    {
        fprintf(stderr, "Erro: caminho do arquivo .qry é NULL\n");
        return -1;
    }
    
    FILE *arquivo = fopen(caminho_qry, "r");
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro: não foi possível abrir %s\n", caminho_qry);
        return -1;
    }
    
    char linha[MAX_LINHA];
    int num_comandos = 0;
    int num_linha = 0;
    int proximo_id = 10000; /* IDs para clones */
    
    /* Lista acumuladora de polígonos de visibilidade para o SVG principal */
    Lista acumulador_poligonos = criar_lista();
    /* Lista acumuladora de pontos (bombas) correspondentes aos polígonos */
    Lista acumulador_bombas = criar_lista();
    
    /* Limpa arquivo de relatório anterior, se existir */
    {
        char caminho_txt[1024];
        snprintf(caminho_txt, 1024, "%s/%s.txt", dir_saida, sufixo_saida);
        remove(caminho_txt);
    }
    
    printf("    Processando comandos...\n");
    
    while (fgets(linha, MAX_LINHA, arquivo) != NULL)
    {
        num_linha++;
        char *linha_limpa = trim(linha);
        
        /* Ignora linhas vazias e comentários */
        if (linha_limpa[0] == '\0' || linha_limpa[0] == '#')
        {
            continue;
        }
        
        /* Identifica o comando */
        if (linha_limpa[0] == 'a' && isspace((unsigned char)linha_limpa[1]))
        {
            /* Comando 'a i j [v|h]' */
            int id_inicio, id_fim;
            char orientacao = 'h'; /* default */
            
            int lidos = sscanf(linha_limpa, "a %d %d %c", &id_inicio, &id_fim, &orientacao);
            if (lidos >= 2)
            {
                printf("[8] Processando arquivo .qry: %s (Ordenação: %s, Limiar: %d)\n", 
           caminho_qry, tipo_ordenacao ? tipo_ordenacao : "padrão", limiar_insertion);
                       
                int convertidos = executar_cmd_a(lista_formas, lista_anteparos, 
                                                  id_inicio, id_fim, orientacao);
                printf("          %d formas convertidas\n", convertidos);
                num_comandos++;
            }
            else
            {
                fprintf(stderr, "Aviso: formato inválido na linha %d: %s\n", 
                        num_linha, linha_limpa);
            }
        }
        else if (linha_limpa[0] == 'd' && isspace((unsigned char)linha_limpa[1]))
        {
            /* Comando 'd x y sfx' */
            double x, y;
            char sufixo_cmd[100]; // Renamed to avoid conflict with function parameter
            
            int lidos = sscanf(linha_limpa, "d %lf %lf %99s", &x, &y, sufixo_cmd);
            if (lidos == 3)
            {
                printf("      [d] Destruição em (%.2f, %.2f) sfx=%s\n", x, y, sufixo_cmd);
                
                Ponto origem = criar_ponto(x, y);
                int destruidos = executar_cmd_d(origem, lista_formas, lista_anteparos,
                                             dir_saida, sufixo_saida, sufixo_cmd, bbox,
                                             tipo_ordenacao, limiar_insertion, 
                                             acumulador_poligonos, acumulador_bombas);
                destruir_ponto(origem);
                
                printf("          %d formas destruídas\n", destruidos);
                num_comandos++;
            }
            else
            {
                fprintf(stderr, "Aviso: formato inválido na linha %d: %s\n", 
                        num_linha, linha_limpa);
            }
        }
        else if ((linha_limpa[0] == 'P' || linha_limpa[0] == 'p') && isspace((unsigned char)linha_limpa[1]))
        {
            /* Comando 'P x y cor sfx' ou 'p x y cor sfx' */
            double x, y;
            char cor[50], sufixo_cmd[100]; // Renamed to avoid conflict with function parameter
            
            /* Pula o primeiro caractere (P ou p) e o espaço */
            int lidos = sscanf(linha_limpa + 2, "%lf %lf %49s %99s", &x, &y, cor, sufixo_cmd);
            if (lidos == 4)
            {
                printf("      [P] Pintura em (%.2f, %.2f) cor=%s sfx=%s\n", x, y, cor, sufixo_cmd);
                
                Ponto origem = criar_ponto(x, y);
                int pintados = executar_cmd_p(origem, lista_formas, lista_anteparos,
                                           cor, dir_saida, sufixo_saida, sufixo_cmd, bbox,
                                           tipo_ordenacao, limiar_insertion,
                                           acumulador_poligonos, acumulador_bombas);
                destruir_ponto(origem);
                
                printf("          %d formas pintadas\n", pintados);
                num_comandos++;
            }
            else
            {
                fprintf(stderr, "Aviso: formato inválido na linha %d: %s\n", 
                        num_linha, linha_limpa);
            }
        }
        else if (strncmp(linha_limpa, "cln", 3) == 0 && isspace((unsigned char)linha_limpa[3]))
        {
            /* Comando 'cln x y dx dy sfx' */
            double x, y, dx, dy;
            char sufixo_cmd[100]; // Renamed to avoid conflict with function parameter
            
            int lidos = sscanf(linha_limpa, "cln %lf %lf %lf %lf %99s", &x, &y, &dx, &dy, sufixo_cmd);
            if (lidos == 5)
            {
                printf("      [cln] Clonagem em (%.2f, %.2f) delta=(%.2f, %.2f) sfx=%s\n", 
                       x, y, dx, dy, sufixo_cmd);
                
                Ponto origem = criar_ponto(x, y);
                int clonados = executar_cmd_cln(origem, lista_formas, lista_anteparos,
                                             dx, dy, dir_saida, sufixo_saida, sufixo_cmd,
                                             bbox, &proximo_id,
                                             tipo_ordenacao, limiar_insertion,
                                             acumulador_poligonos, acumulador_bombas);
                destruir_ponto(origem);
                
                printf("          %d formas clonadas\n", clonados);
                num_comandos++;
            }
            else
            {
                fprintf(stderr, "Aviso: formato inválido na linha %d: %s\n", 
                        num_linha, linha_limpa);
            }
        }
        else
        {
            fprintf(stderr, "Aviso: comando desconhecido na linha %d: %s\n", 
                    num_linha, linha_limpa);
        }
    }
    
    fclose(arquivo);
    
    /* ============================================================================
     * Geração do SVG Principal (Acumulado)
     * ============================================================================ */
    if (acumulador_poligonos != NULL)
    {
        /* Verifica se há algo para desenhar */
        // if (!lista_vazia(acumulador_poligonos)) // Desenhar mesmo vazio para refletir estado final
        {
            char caminho_svg[1024];
            snprintf(caminho_svg, sizeof(caminho_svg), "%s/%s.svg", dir_saida, sufixo_saida);
            
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
                /* 1. Desenha formas (estado final) */
                svg_comentario(svg, "Estado Final das Formas");
                svg_desenhar_lista(svg, lista_formas);
                
                /* 2. Desenha anteparos */
                if (lista_anteparos != NULL && !lista_vazia(lista_anteparos))
                {
                    svg_desenhar_lista_segmentos(svg, lista_anteparos);
                }
                
                /* 3. Desenha TODOS os polígonos acumulados E suas bombas */
                svg_comentario(svg, "Poligonos de Visibilidade Acumulados");
                
                No node_poly = obter_primeiro(acumulador_poligonos);
                No node_bomba = obter_primeiro(acumulador_bombas);
                
                while (node_poly != NULL)
                {
                    PoligonoVisibilidade poly = (PoligonoVisibilidade)obter_elemento(node_poly);
                    svg_desenhar_poligono_visibilidade(svg, poly, "none", "#FFFF00", 0.3);
                    
                    /* Desenha a bomba se disponível */
                    if (node_bomba != NULL)
                    {
                        Ponto pt = (Ponto)obter_elemento(node_bomba);
                        if (pt != NULL)
                        {
                            svg_desenhar_bomba(svg, get_ponto_x(pt), get_ponto_y(pt), 
                                               5.0, "#FF0000");
                        }
                        node_bomba = obter_proximo(node_bomba);
                    }
                    
                    node_poly = obter_proximo(node_poly);
                }
                
                finalizar_svg(svg);
                printf("    [OK] SVG Principal gerado: %s\n", caminho_svg);
            }
        }
        
        /* Limpa a lista de polígonos */
        destruir_lista(acumulador_poligonos, destruir_poligono_visibilidade);
        /* Limpa a lista de bombas */
        destruir_lista(acumulador_bombas, destruir_ponto);
    }
    
    printf("    Total: %d comandos processados\n", num_comandos);
    return num_comandos;
}
