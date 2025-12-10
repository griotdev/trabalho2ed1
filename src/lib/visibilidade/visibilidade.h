/* visibilidade.h
 *
 * Algoritmo de Região de Visibilidade
 * Implementa varredura angular para calcular polígono visível.
 */

#ifndef VISIBILIDADE_H
#define VISIBILIDADE_H

#include "lista.h"
#include "ponto.h"
#include "segmento.h"
#include "poligono.h"

/* Alias para compatibilidade ou uso semântico */
typedef Poligono PoligonoVisibilidade;

/* ============================================================================
 * Funções Principais
 * ============================================================================ */

/**
 * Calcula o polígono de visibilidade a partir de um ponto.
 * 
 * @param origem Ponto de vista (de onde a "luz" emana)
 * @param segmentos Lista de Segmento (obstáculos)
 * @param min_x Limite mínimo X do cenário (bounding box)
 * @param min_y Limite mínimo Y do cenário
 * @param max_x Limite máximo X do cenário
 * @param min_y Limite mínimo Y do cenário
 * @param max_x Limite máximo X do cenário
 * @param max_y Limite máximo Y do cenário
 * @param algoritmo_ordenacao "qsort" ou "mergesort"
 * @return Polígono de visibilidade (lista de pontos), ou NULL em caso de erro
 * 
 * @note O polígono retornado deve ser destruído com destruir_poligono_visibilidade()
 */

PoligonoVisibilidade calcular_visibilidade(Ponto origem, Lista segmentos,
                                            double min_x, double min_y,
                                            double max_x, double max_y,
                                            const char *tipo_ordenacao,
                                            int limiar_insertion);

/**
 * Destroi um polígono de visibilidade.
 * @param poligono Polígono a ser destruído
 */
void destruir_poligono_visibilidade(PoligonoVisibilidade poligono);

/* ============================================================================
 * Funções de Acesso ao Polígono
 * ============================================================================ */

/**
 * Obtém o número de vértices do polígono.
 */
int poligono_num_vertices(PoligonoVisibilidade poligono);

/**
 * Obtém o i-ésimo vértice do polígono.
 * @param poligono O polígono
 * @param indice Índice do vértice (0 a num_vertices-1)
 * @return Ponto do vértice, ou NULL se índice inválido
 * 
 * @note NÃO destrua o ponto retornado!
 */
Ponto poligono_obter_vertice(PoligonoVisibilidade poligono, int indice);

/**
 * Obtém a lista de pontos do polígono.
 * @return Lista de Ponto (não destrua!)
 */
Lista poligono_obter_vertices(PoligonoVisibilidade poligono);

/* ============================================================================
 * Funções de Conversão
 * ============================================================================ */

/**
 * Converte formas geométricas em segmentos para o algoritmo.
 * 
 * @param lista_formas Lista de Forma (círculos, retângulos, linhas, textos)
 * @param lista_segmentos Lista onde os segmentos serão inseridos
 * @param orientacao Para círculos: 'h' (horizontal) ou 'v' (vertical)
 * @return Número de segmentos criados
 * 
 * @note Os segmentos criados devem ser destruídos pelo chamador.
 */
int converter_formas_para_segmentos(Lista lista_formas, Lista lista_segmentos, char orientacao);

#endif /* VISIBILIDADE_H */
