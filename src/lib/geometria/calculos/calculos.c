/* calculos.c
 *
 * Implementação das funções de cálculos geométricos
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calculos.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef INFINITY
#define INFINITY (1.0/0.0)
#endif

/* ============================================================================
 * Implementação das Funções de Orientação
 * ============================================================================ */

double produto_vetorial(Ponto p1, Ponto p2, Ponto p3)
{
    if (p1 == NULL || p2 == NULL || p3 == NULL) return 0.0;
    
    double x1 = ponto_obter_x(p1);
    double y1 = ponto_obter_y(p1);
    double x2 = ponto_obter_x(p2);
    double y2 = ponto_obter_y(p2);
    double x3 = ponto_obter_x(p3);
    double y3 = ponto_obter_y(p3);
    
    /* (P2 - P1) x (P3 - P1) */
    return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}

Orientacao calcular_orientacao(Ponto p1, Ponto p2, Ponto p3)
{
    double cross = produto_vetorial(p1, p2, p3);
    
    if (fabs(cross) < GEO_EPSILON)
    {
        return ORIENTACAO_COLINEAR;
    }
    else if (cross > 0)
    {
        return ORIENTACAO_ANTIHORARIO;
    }
    else
    {
        return ORIENTACAO_HORARIO;
    }
}

Orientacao calcular_orientacao_coords(double x1, double y1, 
                                       double x2, double y2, 
                                       double x3, double y3)
{
    double cross = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
    
    if (fabs(cross) < GEO_EPSILON)
    {
        return ORIENTACAO_COLINEAR;
    }
    else if (cross > 0)
    {
        return ORIENTACAO_ANTIHORARIO;
    }
    else
    {
        return ORIENTACAO_HORARIO;
    }
}

/* ============================================================================
 * Implementação das Funções de Interseção
 * ============================================================================ */

int intersecao_raio_segmento(Ponto origem, Ponto direcao, Segmento seg, Ponto *resultado)
{
    if (origem == NULL || direcao == NULL || seg == NULL || resultado == NULL)
    {
        return 0;
    }
    
    double ox = ponto_obter_x(origem);
    double oy = ponto_obter_y(origem);
    double dx = ponto_obter_x(direcao) - ox;
    double dy = ponto_obter_y(direcao) - oy;
    
    double sx1 = segmento_obter_x1(seg);
    double sy1 = segmento_obter_y1(seg);
    double sx2 = segmento_obter_x2(seg);
    double sy2 = segmento_obter_y2(seg);
    
    /* Vetor do segmento */
    double segx = sx2 - sx1;
    double segy = sy2 - sy1;
    
    /* Denominador do sistema de equações paramétricas */
    double denom = dx * segy - dy * segx;
    
    /* Raio paralelo ao segmento */
    if (fabs(denom) < GEO_EPSILON)
    {
        return 0;
    }
    
    /* Parâmetro t para o raio (origem + t * direção) */
    double t = ((sx1 - ox) * segy - (sy1 - oy) * segx) / denom;
    
    /* Parâmetro u para o segmento (s1 + u * (s2 - s1)) */
    double u = ((sx1 - ox) * dy - (sy1 - oy) * dx) / denom;
    
    /* Interseção válida: t >= 0 (na direção do raio) e 0 <= u <= 1 (dentro do segmento) */
    if (t >= -GEO_EPSILON && u >= -GEO_EPSILON && u <= 1.0 + GEO_EPSILON)
    {
        double ix = ox + t * dx;
        double iy = oy + t * dy;
        *resultado = criar_ponto(ix, iy);
        return 1;
    }
    
    return 0;
}

double distancia_raio_segmento(Ponto origem, double angulo, Segmento seg)
{
    if (origem == NULL || seg == NULL) return INFINITY;
    
    double ox = ponto_obter_x(origem);
    double oy = ponto_obter_y(origem);
    
    /* Criar ponto de direção */
    double dx = cos(angulo);
    double dy = sin(angulo);
    
    double sx1 = segmento_obter_x1(seg);
    double sy1 = segmento_obter_y1(seg);
    double sx2 = segmento_obter_x2(seg);
    double sy2 = segmento_obter_y2(seg);
    
    double segx = sx2 - sx1;
    double segy = sy2 - sy1;
    
    double denom = dx * segy - dy * segx;
    
    if (fabs(denom) < GEO_EPSILON)
    {
        return INFINITY;
    }
    
    double t = ((sx1 - ox) * segy - (sy1 - oy) * segx) / denom;
    double u = ((sx1 - ox) * dy - (sy1 - oy) * dx) / denom;
    
    if (t >= -GEO_EPSILON && u >= -GEO_EPSILON && u <= 1.0 + GEO_EPSILON)
    {
        return t;
    }
    
    return INFINITY;
}

int ponto_na_frente(Ponto origem, Ponto ponto, Segmento seg)
{
    if (origem == NULL || ponto == NULL || seg == NULL) return 0;
    
    /* Calcular distância do ponto até a origem */
    double dist_ponto = ponto_distancia(origem, ponto);
    
    /* Calcular ângulo do ponto */
    double angulo = ponto_angulo_polar(origem, ponto);
    
    /* Calcular distância até o segmento nesse ângulo */
    double dist_seg = distancia_raio_segmento(origem, angulo, seg);
    
    /* Ponto está na frente se está mais perto que o segmento */
    return (dist_ponto < dist_seg - GEO_EPSILON);
}

int comparar_segmentos_raio(Ponto origem, double angulo, Segmento seg1, Segmento seg2)
{
    double dist1 = distancia_raio_segmento(origem, angulo, seg1);
    double dist2 = distancia_raio_segmento(origem, angulo, seg2);
    
    if (fabs(dist1 - dist2) < GEO_EPSILON)
    {
        return 0;
    }
    
    return (dist1 < dist2) ? -1 : 1;
}
