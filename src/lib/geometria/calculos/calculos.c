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

/* ============================================================================
 * Funções Auxiliares Estáticas para Colisão
 * ============================================================================ */

/* Verifica se q está no segmento pr */
static int no_segmento(double px, double py, double rx, double ry, double qx, double qy)
{
    return qx <= fmax(px, rx) && qx >= fmin(px, rx) &&
           qy <= fmax(py, ry) && qy >= fmin(py, ry);
}

static int seg_intersepta(double x1, double y1, double x2, double y2,
                          double x3, double y3, double x4, double y4)
{
    Orientacao o1 = calcular_orientacao_coords(x1, y1, x2, y2, x3, y3);
    Orientacao o2 = calcular_orientacao_coords(x1, y1, x2, y2, x4, y4);
    Orientacao o3 = calcular_orientacao_coords(x3, y3, x4, y4, x1, y1);
    Orientacao o4 = calcular_orientacao_coords(x3, y3, x4, y4, x2, y2);
    
    /* Caso geral */
    if (o1 != o2 && o3 != o4) return 1;
    
    /* Casos especiais de colinearidade */
    if (o1 == ORIENTACAO_COLINEAR && no_segmento(x1, y1, x2, y2, x3, y3)) return 1;
    if (o2 == ORIENTACAO_COLINEAR && no_segmento(x1, y1, x2, y2, x4, y4)) return 1;
    if (o3 == ORIENTACAO_COLINEAR && no_segmento(x3, y3, x4, y4, x1, y1)) return 1;
    if (o4 == ORIENTACAO_COLINEAR && no_segmento(x3, y3, x4, y4, x2, y2)) return 1;
    
    return 0;
}

/* Distância quadrada mínima de um ponto P a um segmento AB */
static double dist_sq_ponto_segmento(double px, double py, double ax, double ay, double bx, double by)
{
    double l2 = (bx - ax)*(bx - ax) + (by - ay)*(by - ay);
    if (l2 == 0) return (px - ax)*(px - ax) + (py - ay)*(py - ay);
    
    double t = ((px - ax)*(bx - ax) + (py - ay)*(by - ay)) / l2;
    t = fmax(0, fmin(1, t));
    
    double projx = ax + t * (bx - ax);
    double projy = ay + t * (by - ay);
    
    return (px - projx)*(px - projx) + (py - projy)*(py - projy);
}


/* ============================================================================
 * Função Principal de Verificação
 * ============================================================================ */

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
    
    /* Verifica intersecção de arestas do polígono com a forma */
    /* Para cada aresta do polígono (vx1, vy1) -> (vx2, vy2) */
    
    switch (tipo)
    {
        case TIPO_CIRCULO:
        {
            Circulo c = (Circulo)dados;
            double cx = getCirculoX(c);
            double cy = getCirculoY(c);
            double r = getCirculoRaio(c);
            double r_sq = r * r;
            
            /* 1. Centro do círculo no polígono */
            if (ponto_no_poligono(cx, cy, vertices, num_vertices)) return 1;
            
            /* 2. Algum vértice do polígono dentro do círculo */
            for (int i = 0; i < num_vertices; i++) {
                double vx = vertices[2*i];
                double vy = vertices[2*i+1];
                double d2 = (vx - cx)*(vx - cx) + (vy - cy)*(vy - cy);
                if (d2 <= r_sq) return 1;
            }
            
            /* 3. Alguma aresta do polígono cruza o círculo (distancia segmento a centro <= r) */
            for (int i = 0, j = num_vertices - 1; i < num_vertices; j = i++) {
                double vx1 = vertices[2*i];
                double vy1 = vertices[2*i+1];
                double vx2 = vertices[2*j];
                double vy2 = vertices[2*j+1];
                
                if (dist_sq_ponto_segmento(cx, cy, vx1, vy1, vx2, vy2) <= r_sq) return 1;
            }
            return 0;
        }
        
        case TIPO_RETANGULO:
        {
            Retangulo r = (Retangulo)dados;
            double rx = getRetanguloX(r);
            double ry = getRetanguloY(r);
            double w = getRetanguloLargura(r);
            double h = getRetanguloAltura(r);
            
            /* Definição das arestas do retângulo */
            double rect_x[4] = {rx, rx+w, rx+w, rx};
            double rect_y[4] = {ry, ry, ry+h, ry+h};
            
            /* 1. Vértices do retângulo no polígono */
            for(int k=0; k<4; k++)
                if(ponto_no_poligono(rect_x[k], rect_y[k], vertices, num_vertices)) return 1;

            /* 2. Vértices do polígono no retângulo */
            for(int i=0; i<num_vertices; i++) {
                double vx = vertices[2*i];
                double vy = vertices[2*i+1];
                if (vx >= rx && vx <= rx+w && vy >= ry && vy <= ry+h) return 1;
            }
            
            /* 3. Intersecção de arestas */
            for (int i = 0, j = num_vertices - 1; i < num_vertices; j = i++) {
                double vx1 = vertices[2*i];
                double vy1 = vertices[2*i+1];
                double vx2 = vertices[2*j];
                double vy2 = vertices[2*j+1];
                
                /* Compara com 4 arestas do retângulo */
                for(int k=0; k<4; k++) {
                    int next_k = (k+1)%4;
                    if (seg_intersepta(vx1, vy1, vx2, vy2, 
                                       rect_x[k], rect_y[k], 
                                       rect_x[next_k], rect_y[next_k])) return 1;
                }
            }
            return 0;
        }
        
        case TIPO_LINHA:
        {
            Linha l = (Linha)dados;
            double lx1 = getLinhaX1(l);
            double ly1 = getLinhaY1(l);
            double lx2 = getLinhaX2(l);
            double ly2 = getLinhaY2(l);
            
            /* 1. Extremos no polígono */
            if (ponto_no_poligono(lx1, ly1, vertices, num_vertices)) return 1;
            if (ponto_no_poligono(lx2, ly2, vertices, num_vertices)) return 1;
            
            /* 2. Intersecção de arestas */
            for (int i = 0, j = num_vertices - 1; i < num_vertices; j = i++) {
                double vx1 = vertices[2*i];
                double vy1 = vertices[2*i+1];
                double vx2 = vertices[2*j];
                double vy2 = vertices[2*j+1];
                
                if (seg_intersepta(vx1, vy1, vx2, vy2, lx1, ly1, lx2, ly2)) return 1;
            }
            return 0;
        }
        
        case TIPO_TEXTO:
        {
            Texto t = (Texto)dados;
            double txt_x = getTextoX(t);
            double txt_y = getTextoY(t);
            /* Texto é tratado como ponto (âncora) por simplificação ou bounding box?
               A regra pede apenas âncora?
               Regra: "Verificar intersecta(Forma)". Texto tem largura.
               Se o usuário não pediu implementação de BB de texto, mantemos âncora.
               Mas texto vira segmento em `converter_formas`.
               Aqui estamos testando a FORMA original.
               Vou manter apenas checagem da âncora para ser seguro, ou expandir para BB se soubesse a string.
               O PDF diz "Textos são convertidos em segmentos...". Isso é para anteparo.
               Para destruição, "Formas... dentro do polígono sofrem a ação".
               Assumindo âncora por enquanto, como estava.
            */
            return ponto_no_poligono(txt_x, txt_y, vertices, num_vertices);
        }
        
        default:
            return 0;
    }
}

