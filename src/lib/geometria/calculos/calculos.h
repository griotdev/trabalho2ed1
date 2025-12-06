/* calculos.h
 *
 * Funções para cálculos geométricos
 * Orientação, interseção raio-segmento, etc.
 */

#ifndef CALCULOS_H
#define CALCULOS_H

#include "ponto.h"
#include "segmento.h"

/* Tolerância para comparações de ponto flutuante */
#define GEO_EPSILON 1e-9

/* Resultados de orientação */
typedef enum {
    ORIENTACAO_COLINEAR = 0,
    ORIENTACAO_HORARIO = -1,      /* Right turn */
    ORIENTACAO_ANTIHORARIO = 1    /* Left turn */
} Orientacao;

/* ============================================================================
 * Funções de Orientação
 * ============================================================================ */

/**
 * Calcula a orientação de três pontos P1 -> P2 -> P3.
 * 
 * @param p1 Primeiro ponto
 * @param p2 Segundo ponto
 * @param p3 Terceiro ponto
 * @return ORIENTACAO_ANTIHORARIO (esquerda), ORIENTACAO_HORARIO (direita), ou COLINEAR
 * 
 * @note Usa produto vetorial: (p2-p1) x (p3-p1)
 */
Orientacao calcular_orientacao(Ponto p1, Ponto p2, Ponto p3);

/**
 * Calcula a orientação usando coordenadas diretamente.
 */
Orientacao calcular_orientacao_coords(double x1, double y1, 
                                       double x2, double y2, 
                                       double x3, double y3);

/**
 * Calcula o produto vetorial 2D (P2-P1) x (P3-P1).
 * Também conhecido como "área com sinal" * 2.
 * 
 * @return Positivo se antihorário, negativo se horário, zero se colinear
 */
double produto_vetorial(Ponto p1, Ponto p2, Ponto p3);

/* ============================================================================
 * Funções de Interseção
 * ============================================================================ */

/**
 * Calcula o ponto de interseção entre um raio e um segmento.
 * O raio parte de 'origem' passando por 'direcao'.
 * 
 * @param origem Origem do raio
 * @param direcao Ponto que define a direção do raio
 * @param seg Segmento a testar
 * @param resultado Saída: ponto de interseção (se houver)
 * @return 1 se há interseção, 0 caso contrário
 * 
 * @note O resultado deve ser destruído pelo chamador!
 */
int intersecao_raio_segmento(Ponto origem, Ponto direcao, Segmento seg, Ponto *resultado);

/**
 * Verifica se um ponto está "à frente" de um segmento do ponto de vista da origem.
 * Usado para determinar se um segmento bloqueia a visão.
 * 
 * @param origem Ponto de vista
 * @param ponto Ponto a testar
 * @param seg Segmento bloqueador
 * @return 1 se o ponto está à frente (mais perto), 0 se está atrás (bloqueado)
 */
int ponto_na_frente(Ponto origem, Ponto ponto, Segmento seg);

/**
 * Calcula a distância de um ponto até um segmento ao longo de uma direção.
 * Usado para comparar segmentos na árvore de sweep.
 * 
 * @param origem Origem do raio
 * @param angulo Ângulo do raio (radianos)
 * @param seg Segmento
 * @return Distância até o segmento, ou INFINITY se não intersecta
 */
double distancia_raio_segmento(Ponto origem, double angulo, Segmento seg);

/* ============================================================================
 * Funções de Comparação para Ordenação
 * ============================================================================ */

/**
 * Compara dois segmentos pela distância ao longo do raio atual.
 * Usado na árvore de segmentos ativos.
 * 
 * @param origem Ponto de vista (origem do raio)
 * @param angulo Ângulo atual da varredura
 * @param seg1 Primeiro segmento
 * @param seg2 Segundo segmento
 * @return < 0 se seg1 mais perto, > 0 se seg2 mais perto, 0 se equidistantes
 */
int comparar_segmentos_raio(Ponto origem, double angulo, Segmento seg1, Segmento seg2);

#endif /* CALCULOS_H */
