#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "poligono.h"
#include "lista.h"
#include "ponto.h"

#define INITIAL_CAPACITY 16

/* Definição concreta da struct baseada em Array Dinâmico */
typedef struct poligono_st {
    double *coords;     /* [x0, y0, x1, y1, ...] */
    int num_vertices;
    int capacity;       /* Capacidade atual do array em número de vértices */
    Lista lista_cache;  /* Cache para uso legado, invalidada ao alterar */
} PoligonoStruct;

Poligono poligono_criar() {
    PoligonoStruct *p = (PoligonoStruct*)malloc(sizeof(PoligonoStruct));
    if (p == NULL) return NULL;

    p->coords = (double*)malloc(2 * INITIAL_CAPACITY * sizeof(double));
    if (p->coords == NULL) {
        free(p);
        return NULL;
    }

    p->num_vertices = 0;
    p->capacity = INITIAL_CAPACITY;
    p->lista_cache = NULL;
    
    return (Poligono)p;
}

static void limpar_cache(PoligonoStruct *ps) {
    if (ps->lista_cache != NULL) {
        /* Precisamos de uma func que destrói ponto, mas aqui os pontos são cópias? */
        /* Sim, lista_cache terá pontos alocados */
        destruir_lista(ps->lista_cache, (void(*)(void*))destruir_ponto);
        ps->lista_cache = NULL;
    }
}

void poligono_destruir(Poligono p) {
    PoligonoStruct *ps = (PoligonoStruct*)p;
    if (ps != NULL) {
        if (ps->coords != NULL) {
            free(ps->coords);
        }
        limpar_cache(ps);
        free(ps);
    }
}

void poligono_inserir_vertice(Poligono p, double x, double y) {
    PoligonoStruct *ps = (PoligonoStruct*)p;
    if (ps == NULL) return;

    if (ps->num_vertices >= ps->capacity) {
        int new_capacity = ps->capacity * 2;
        double *new_coords = (double*)realloc(ps->coords, 2 * new_capacity * sizeof(double));
        if (new_coords == NULL) return; /* Falha na alocação, perde dado mas mantém integro? */
        
        ps->coords = new_coords;
        ps->capacity = new_capacity;
    }

    ps->coords[2 * ps->num_vertices] = x;
    ps->coords[2 * ps->num_vertices + 1] = y;
    ps->num_vertices++;
    
    limpar_cache(ps); /* Invalida cache legado */
}

int poligono_qtd_vertices(Poligono p) {
    PoligonoStruct *ps = (PoligonoStruct*)p;
    if (ps == NULL) return 0;
    return ps->num_vertices;
}

Ponto poligono_get_vertice(Poligono p, int indice) {
    PoligonoStruct *ps = (PoligonoStruct*)p;
    if (ps == NULL || indice < 0 || indice >= ps->num_vertices) return NULL;

    /* Retorna uma copia fresca, pois Ponto é opaco e ponteiro */
    return criar_ponto(ps->coords[2*indice], ps->coords[2*indice+1]);
}

double* poligono_get_vertices_ref(Poligono p, int *num_vertices) {
    PoligonoStruct *ps = (PoligonoStruct*)p;
    if (ps == NULL) {
        if(num_vertices) *num_vertices = 0;
        return NULL;
    }
    
    if(num_vertices) *num_vertices = ps->num_vertices;
    return ps->coords;
}

int poligono_obter_vertices_array(Poligono p, double **vertices_out) {
    PoligonoStruct *ps = (PoligonoStruct*)p;
    if (ps == NULL || vertices_out == NULL || ps->num_vertices == 0) {
        *vertices_out = NULL;
        return 0;
    }

    /* Versão legada: faz MALLOC e copia */
    double *arr = (double*)malloc(2 * ps->num_vertices * sizeof(double));
    if (arr == NULL) return 0;

    memcpy(arr, ps->coords, 2 * ps->num_vertices * sizeof(double));

    *vertices_out = arr;
    return ps->num_vertices;
}

Lista poligono_obter_lista(Poligono p) {
    PoligonoStruct *ps = (PoligonoStruct*)p;
    if (ps == NULL) return NULL;
    
    if (ps->lista_cache == NULL) {
        /* Reconstrói lista sob demanda */
        ps->lista_cache = criar_lista();
        for (int i = 0; i < ps->num_vertices; i++) {
            Ponto pt = criar_ponto(ps->coords[2*i], ps->coords[2*i+1]);
            inserir_fim(ps->lista_cache, pt);
        }
    }
    
    return ps->lista_cache;
}
