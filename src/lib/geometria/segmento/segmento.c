/* segmento.c
 *
 * Implementação do TAD Segmento
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "segmento.h"
#include "ponto.h"

/* ============================================================================
 * Estrutura Interna (Ponteiro Opaco)
 * ============================================================================ */

typedef struct segmento_internal
{
    int id;      /* ID da forma original */
    Ponto p1;    /* Ponto inicial */
    Ponto p2;    /* Ponto final */
} SegmentoInternal;

/* ============================================================================
 * Implementação das Funções de Criação e Destruição
 * ============================================================================ */

Segmento criar_segmento(int id, double x1, double y1, double x2, double y2)
{
    SegmentoInternal *seg = (SegmentoInternal*)malloc(sizeof(SegmentoInternal));
    if (seg == NULL)
    {
        fprintf(stderr, "Erro: falha ao alocar segmento.\n");
        return NULL;
    }
    
    seg->id = id;
    seg->p1 = criar_ponto(x1, y1);
    seg->p2 = criar_ponto(x2, y2);
    
    if (seg->p1 == NULL || seg->p2 == NULL)
    {
        destruir_ponto(seg->p1);
        destruir_ponto(seg->p2);
        free(seg);
        return NULL;
    }
    
    return (Segmento)seg;
}

Segmento criar_segmento_pontos(int id, Ponto p1, Ponto p2)
{
    if (p1 == NULL || p2 == NULL) return NULL;
    
    return criar_segmento(id, 
                          ponto_obter_x(p1), ponto_obter_y(p1),
                          ponto_obter_x(p2), ponto_obter_y(p2));
}

Segmento clonar_segmento(Segmento segmento)
{
    SegmentoInternal *seg = (SegmentoInternal*)segmento;
    if (seg == NULL) return NULL;
    
    return criar_segmento(seg->id,
                          ponto_obter_x(seg->p1), ponto_obter_y(seg->p1),
                          ponto_obter_x(seg->p2), ponto_obter_y(seg->p2));
}

void destruir_segmento(Segmento segmento)
{
    SegmentoInternal *seg = (SegmentoInternal*)segmento;
    if (seg == NULL) return;
    
    destruir_ponto(seg->p1);
    destruir_ponto(seg->p2);
    free(seg);
}

/* ============================================================================
 * Implementação das Funções de Acesso
 * ============================================================================ */

int segmento_obter_id(Segmento segmento)
{
    SegmentoInternal *seg = (SegmentoInternal*)segmento;
    return seg ? seg->id : -1;
}

Ponto segmento_obter_p1(Segmento segmento)
{
    SegmentoInternal *seg = (SegmentoInternal*)segmento;
    return seg ? seg->p1 : NULL;
}

Ponto segmento_obter_p2(Segmento segmento)
{
    SegmentoInternal *seg = (SegmentoInternal*)segmento;
    return seg ? seg->p2 : NULL;
}

double segmento_obter_x1(Segmento segmento)
{
    SegmentoInternal *seg = (SegmentoInternal*)segmento;
    return seg ? ponto_obter_x(seg->p1) : 0.0;
}

double segmento_obter_y1(Segmento segmento)
{
    SegmentoInternal *seg = (SegmentoInternal*)segmento;
    return seg ? ponto_obter_y(seg->p1) : 0.0;
}

double segmento_obter_x2(Segmento segmento)
{
    SegmentoInternal *seg = (SegmentoInternal*)segmento;
    return seg ? ponto_obter_x(seg->p2) : 0.0;
}

double segmento_obter_y2(Segmento segmento)
{
    SegmentoInternal *seg = (SegmentoInternal*)segmento;
    return seg ? ponto_obter_y(seg->p2) : 0.0;
}

/* ============================================================================
 * Implementação das Funções Geométricas
 * ============================================================================ */

double segmento_comprimento(Segmento segmento)
{
    SegmentoInternal *seg = (SegmentoInternal*)segmento;
    if (seg == NULL) return 0.0;
    
    return ponto_distancia(seg->p1, seg->p2);
}

int segmento_dividir(Segmento segmento, Ponto ponto, Segmento *seg1, Segmento *seg2)
{
    SegmentoInternal *seg = (SegmentoInternal*)segmento;
    if (seg == NULL || ponto == NULL || seg1 == NULL || seg2 == NULL)
    {
        return 0;
    }
    
    /* Primeiro segmento: p1 até ponto de divisão */
    *seg1 = criar_segmento(seg->id,
                           ponto_obter_x(seg->p1), ponto_obter_y(seg->p1),
                           ponto_obter_x(ponto), ponto_obter_y(ponto));
    
    /* Segundo segmento: ponto de divisão até p2 */
    *seg2 = criar_segmento(seg->id,
                           ponto_obter_x(ponto), ponto_obter_y(ponto),
                           ponto_obter_x(seg->p2), ponto_obter_y(seg->p2));
    
    return (*seg1 != NULL && *seg2 != NULL);
}
