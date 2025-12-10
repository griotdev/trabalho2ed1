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
    
    double x1 = get_ponto_x(p1);
    double y1 = get_ponto_y(p1);
    double x2 = get_ponto_x(p2);
    double y2 = get_ponto_y(p2);
    double x3 = get_ponto_x(p3);
    double y3 = get_ponto_y(p3);
    
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
    
    double ox = get_ponto_x(origem);
    double oy = get_ponto_y(origem);
    double dx = get_ponto_x(direcao) - ox;
    double dy = get_ponto_y(direcao) - oy;
    
    double sx1 = get_segmento_x1(seg);
    double sy1 = get_segmento_y1(seg);
    double sx2 = get_segmento_x2(seg);
    double sy2 = get_segmento_y2(seg);
    
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
    
    double ox = get_ponto_x(origem);
    double oy = get_ponto_y(origem);
    
    /* Criar ponto de direção */
    double dx = cos(angulo);
    double dy = sin(angulo);
    
    double sx1 = get_segmento_x1(seg);
    double sy1 = get_segmento_y1(seg);
    double sx2 = get_segmento_x2(seg);
    double sy2 = get_segmento_y2(seg);
    
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

/* ============================================================================
 * Implementação das Funções de Ponto no Polígono
 * ============================================================================ */

int ponto_no_poligono(double px, double py, double *vertices, int num_vertices)
{
    if (vertices == NULL || num_vertices < 3)
    {
        return 0;
    }
    
    /* Algoritmo Ray Casting (par/ímpar)
     * Dispara um raio horizontal para a direita e conta quantas vezes
     * cruza as arestas do polígono. Se cruzar um número ímpar de vezes,
     * o ponto está dentro.
     */
    int dentro = 0;
    
    for (int i = 0, j = num_vertices - 1; i < num_vertices; j = i++)
    {
        double xi = vertices[i * 2];
        double yi = vertices[i * 2 + 1];
        double xj = vertices[j * 2];
        double yj = vertices[j * 2 + 1];
        
        /* Verifica se o raio horizontal cruza a aresta (i, j) */
        if (((yi > py) != (yj > py)) &&
            (px < (xj - xi) * (py - yi) / (yj - yi) + xi))
        {
            dentro = !dentro;
        }
    }
    
    return dentro;
}

/* Precisamos incluir os headers de formas para forma_no_poligono */
#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

int forma_no_poligono(void *forma_ptr, double *vertices, int num_vertices)
{
    if (forma_ptr == NULL || vertices == NULL || num_vertices < 3)
    {
        return 0;
    }
    
    Forma forma = (Forma)forma_ptr;
    TipoForma tipo = getFormaTipo(forma);
    void *dados = getFormaDados(forma);
    
    switch (tipo)
    {
        case TIPO_CIRCULO:
        {
            Circulo c = (Circulo)dados;
            double cx = getCirculoX(c);
            double cy = getCirculoY(c);
            
            /* Verifica se o centro está dentro */
            return ponto_no_poligono(cx, cy, vertices, num_vertices);
        }
        
        case TIPO_RETANGULO:
        {
            Retangulo r = (Retangulo)dados;
            double x = getRetanguloX(r);
            double y = getRetanguloY(r);
            double w = getRetanguloLargura(r);
            double h = getRetanguloAltura(r);
            
            /* Verifica se algum dos 4 cantos está dentro */
            if (ponto_no_poligono(x, y, vertices, num_vertices) ||
                ponto_no_poligono(x + w, y, vertices, num_vertices) ||
                ponto_no_poligono(x, y + h, vertices, num_vertices) ||
                ponto_no_poligono(x + w, y + h, vertices, num_vertices))
            {
                return 1;
            }
            
            /* Ou se o centro está dentro */
            return ponto_no_poligono(x + w/2, y + h/2, vertices, num_vertices);
        }
        
        case TIPO_LINHA:
        {
            Linha l = (Linha)dados;
            double x1 = getLinhaX1(l);
            double y1 = getLinhaY1(l);
            double x2 = getLinhaX2(l);
            double y2 = getLinhaY2(l);
            
            /* Verifica se algum extremo está dentro */
            return ponto_no_poligono(x1, y1, vertices, num_vertices) ||
                   ponto_no_poligono(x2, y2, vertices, num_vertices);
        }
        
        case TIPO_TEXTO:
        {
            Texto t = (Texto)dados;
            double x = getTextoX(t);
            double y = getTextoY(t);
            
            /* Verifica se a âncora está dentro */
            return ponto_no_poligono(x, y, vertices, num_vertices);
        }
        
        default:
            return 0;
    }
}

