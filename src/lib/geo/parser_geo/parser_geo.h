/* parser_geo.h
 *
 * TAD para leitura e parsing de arquivos .geo
 * Lê o arquivo de geometria e popula uma lista com as formas definidas.
 */

#ifndef PARSER_GEO_H
#define PARSER_GEO_H

#include "lista.h"

/* ============================================================================
 * Funções Principais
 * ============================================================================ */

/**
 * Lê um arquivo .geo e popula a lista com as formas geométricas.
 * 
 * Comandos reconhecidos:
 *   c  - Círculo:    c id x y raio corBorda corPreenchimento
 *   r  - Retângulo:  r id x y largura altura corBorda corPreenchimento
 *   l  - Linha:      l id x1 y1 x2 y2 cor
 *   t  - Texto:      t id x y corBorda corPreenchimento ancora texto...
 *   ts - Estilo:     ts fontFamily fontWeight fontSize
 * 
 * @param caminho_arquivo Caminho completo para o arquivo .geo
 * @param lista_formas Lista onde as formas serão inseridas
 * @return Número de formas lidas com sucesso, ou -1 em caso de erro
 * 
 * @pre caminho_arquivo != NULL
 * @pre lista_formas != NULL (lista já criada)
 * @post As formas são inseridas no final da lista
 * 
 * Exemplo:
 *   Lista formas = criar_lista();
 *   int n = ler_arquivo_geo("mapa.geo", formas);
 *   printf("Lidas %d formas\n", n);
 */
int ler_arquivo_geo(const char *caminho_arquivo, Lista lista_formas);

/**
 * Obtém as dimensões do cenário (bounding box de todas as formas).
 * Útil para definir o viewBox do SVG.
 * 
 * @param lista_formas Lista com as formas
 * @param min_x Ponteiro para armazenar X mínimo (saída)
 * @param min_y Ponteiro para armazenar Y mínimo (saída)
 * @param max_x Ponteiro para armazenar X máximo (saída)
 * @param max_y Ponteiro para armazenar Y máximo (saída)
 * @return 1 se sucesso, 0 se lista vazia ou erro
 */
int obter_dimensoes_cenario(Lista lista_formas, 
                            double *min_x, double *min_y,
                            double *max_x, double *max_y);

#endif /* PARSER_GEO_H */
