/* visibilidade.c
 *
 * Implementação do Algoritmo de Região de Visibilidade
 * Varredura Angular (Angular Plane Sweep)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sort.h"

#include "visibilidade.h"
#include "lista.h"
#include "ponto.h"
#include "segmento.h"
#include "calculos.h"
#include "arvore.h"
#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define EPSILON 1e-9
#define MARGEM_BBOX 5.0

/* ============================================================================
 * Estruturas Internas
 * ============================================================================ */

/* Tipo de vértice (evento) */
typedef enum {
    EVENTO_INICIO,
    EVENTO_FIM
} TipoEvento;

/* Evento da varredura (vértice de um segmento) */
typedef struct evento
{
    Ponto ponto;        /* Coordenada do vértice */
    double angulo;      /* Ângulo polar em relação à origem */
    double distancia;   /* Distância até a origem */
    TipoEvento tipo;    /* INICIO ou FIM */
    Segmento segmento;  /* Segmento ao qual pertence */
} Evento;

/* Estrutura interna removida em favor do TAD Poligono em geometria/poligono */

/* ============================================================================
 * Funções Auxiliares - Eventos
 * ============================================================================ */

/**
 * Cria um evento.
 */
static Evento* criar_evento(Ponto ponto, TipoEvento tipo, Segmento seg, Ponto origem)
{
    Evento *e = (Evento*)malloc(sizeof(Evento));
    if (e == NULL) return NULL;
    
    e->ponto = clonar_ponto(ponto);
    e->tipo = tipo;
    e->segmento = seg;
    e->angulo = ponto_angulo_polar(origem, ponto);
    e->distancia = ponto_distancia(origem, ponto);
    
    return e;
}

/**
 * Destroi um evento.
 */
static void destruir_evento(void *ptr)
{
    Evento *e = (Evento*)ptr;
    if (e != NULL)
    {
        destruir_ponto(e->ponto);
        free(e);
    }
}

/**
 * Compara dois eventos para ordenação.
 * Ordenação: ângulo crescente, tipo INICIO antes de FIM, distância crescente.
 */
static int comparar_eventos(const void *a, const void *b)
{
    Evento *e1 = *(Evento**)a;
    Evento *e2 = *(Evento**)b;
    
    /* 1. Por ângulo */
    if (fabs(e1->angulo - e2->angulo) > EPSILON)
    {
        return (e1->angulo < e2->angulo) ? -1 : 1;
    }
    
    /* 2. INICIO antes de FIM */
    if (e1->tipo != e2->tipo)
    {
        return (e1->tipo == EVENTO_INICIO) ? -1 : 1;
    }
    
    /* 3. Por distância (mais perto primeiro) */
    if (fabs(e1->distancia - e2->distancia) > EPSILON)
    {
        return (e1->distancia < e2->distancia) ? -1 : 1;
    }
    
    return 0;
}

/* ============================================================================
 * Funções Auxiliares - Segmentos
 * ============================================================================ */

/**
 * Destroi um segmento (callback para lista).
 */
static void destruir_segmento_callback(void *ptr)
{
    destruir_segmento((Segmento)ptr);
}

/**
 * Cria os 4 segmentos da bounding box.
 */
static void criar_bounding_box(Lista segmentos, double min_x, double min_y, 
                                double max_x, double max_y)
{
    /* Adiciona margem */
    min_x -= MARGEM_BBOX;
    min_y -= MARGEM_BBOX;
    max_x += MARGEM_BBOX;
    max_y += MARGEM_BBOX;
    
    /* Cria 4 segmentos formando a bounding box (com margem) */
    /* Sentido anti-horário */
    /* ID -1 como "sistema", id_original -1 */
    inserir_fim(segmentos, criar_segmento(-1, -1, min_x, min_y, max_x, min_y, "none")); /* baixo */
    inserir_fim(segmentos, criar_segmento(-2, -1, max_x, min_y, max_x, max_y, "none")); /* direita */
    inserir_fim(segmentos, criar_segmento(-3, -1, max_x, max_y, min_x, max_y, "none")); /* cima */
    inserir_fim(segmentos, criar_segmento(-4, -1, min_x, max_y, min_x, min_y, "none")); /* esquerda */
}



/**
 * Extrai eventos (vértices) de todos os segmentos.
 */
static Lista extrair_eventos(Lista segmentos, Ponto origem)
{
    Lista eventos = criar_lista();
    if (eventos == NULL) return NULL;
    
    No atual = obter_primeiro(segmentos);
    while (atual != NULL)
    {
        Segmento seg = (Segmento)obter_elemento(atual);
        
        /* Evento de início (p1) */
        Evento *e1 = criar_evento(get_segmento_p1(seg), EVENTO_INICIO, seg, origem);
        if (e1 != NULL) inserir_fim(eventos, e1);
        
        /* Evento de fim (p2) */
        Evento *e2 = criar_evento(get_segmento_p2(seg), EVENTO_FIM, seg, origem);
        if (e2 != NULL) inserir_fim(eventos, e2);
        
        atual = obter_proximo(atual);
    }
    
    return eventos;
}

/**
 * Ordena lista de eventos por ângulo.
 */
static void ordenar_eventos(Lista eventos, const char *tipo_ordenacao, int limiar)
{
    int n = obter_tamanho(eventos);
    if (n <= 1) return;
    
    /* Converte para array para usar o módulo sort */
    Evento **arr = (Evento**)malloc(n * sizeof(Evento*));
    if (arr == NULL) return;
    
    No atual = obter_primeiro(eventos);
    for (int i = 0; i < n && atual != NULL; i++)
    {
        arr[i] = (Evento*)obter_elemento(atual);
        atual = obter_proximo(atual);
    }
    
    /* Seleciona algoritmo */
    AlgoritmoOrdenacao alg_enum = ALG_QSORT;
    if (tipo_ordenacao != NULL && strcmp(tipo_ordenacao, "mergesort") == 0)
    {
        alg_enum = ALG_MERGESORT;
    }
    
    /* Ordena usando o módulo sort */
    ordenar((void*)arr, n, sizeof(Evento*), comparar_eventos, alg_enum, limiar);
    
    /* Limpa a lista original e reinsere ordenado */
    while (!lista_vazia(eventos))
    {
        remover_inicio(eventos);
    }
    
    for (int i = 0; i < n; i++)
    {
        inserir_fim(eventos, arr[i]);
    }
    
    free(arr);
}

/* ============================================================================
 * Algoritmo Principal de Visibilidade
 * ============================================================================ */

PoligonoVisibilidade calcular_visibilidade(Ponto origem, Lista segmentos_entrada,
                                            double min_x, double min_y,
                                            double max_x, double max_y,
                                            const char *tipo_ordenacao,
                                            int limiar_insertion)
{
    if (origem == NULL) return NULL;
    
    /* Cria lista de segmentos de trabalho (cópia + bounding box) */
    Lista segmentos = criar_lista();
    if (segmentos == NULL) return NULL;
    
    /* Copia segmentos de entrada */
    if (segmentos_entrada != NULL)
    {
        No atual = obter_primeiro(segmentos_entrada);
        while (atual != NULL)
        {
            Segmento seg = (Segmento)obter_elemento(atual);
            inserir_fim(segmentos, clonar_segmento(seg));
            atual = obter_proximo(atual);
        }
    }
    
    /* Expande bounding box para incluir a origem */
    double ox = get_ponto_x(origem);
    double oy = get_ponto_y(origem);
    if (ox < min_x) min_x = ox;
    if (ox > max_x) max_x = ox;
    if (oy < min_y) min_y = oy;
    if (oy > max_y) max_y = oy;
    
    /* Adiciona bounding box */
    criar_bounding_box(segmentos, min_x, min_y, max_x, max_y);
    
    /* ========================================================================
     * PRÉ-PROCESSAMENTO: Divisão de Segmentos no Ângulo 0
     * ======================================================================== */
    No node_seg = obter_primeiro(segmentos);
    while (node_seg != NULL)
    {
        Segmento seg = (Segmento)obter_elemento(node_seg);
        No proximo_node = obter_proximo(node_seg);
        
        Ponto dir_zero = criar_ponto(ox + 1.0, oy);
        Ponto intersecao = NULL;
        
        if (intersecao_raio_segmento(origem, dir_zero, seg, &intersecao))
        {
            double ix = get_ponto_x(intersecao);
            double iy = get_ponto_y(intersecao);
            
            double x1 = get_segmento_x1(seg);
            double y1 = get_segmento_y1(seg);
            double x2 = get_segmento_x2(seg);
            double y2 = get_segmento_y2(seg);
            
            if (hypot(ix - x1, iy - y1) > EPSILON &&
                hypot(ix - x2, iy - y2) > EPSILON)
            {
                int id = get_segmento_id(seg);
                int id_orig = get_segmento_id_original(seg);
                const char *cor = get_segmento_cor(seg);
                
                Segmento s1 = criar_segmento(id, id_orig, x1, y1, ix, iy, cor);
                Segmento s2 = criar_segmento(id, id_orig, ix, iy, x2, y2, cor);
                
                inserir_fim(segmentos, s1);
                inserir_fim(segmentos, s2);
                
                void *removido = remover_no(segmentos, node_seg);
                if (removido) destruir_segmento((Segmento)removido);
            }
            destruir_ponto(intersecao);
        }
        destruir_ponto(dir_zero); 
        node_seg = proximo_node;
    }

    /* Extrai eventos (vértices) */
    Lista eventos = extrair_eventos(segmentos, origem);
    if (eventos == NULL || lista_vazia(eventos))
    {
        destruir_lista(segmentos, destruir_segmento_callback);
        if (eventos) destruir_lista(eventos, destruir_evento);
        return NULL;
    }
    
    /* Ordena eventos por ângulo */
    ordenar_eventos(eventos, tipo_ordenacao, limiar_insertion);
    
    /* Cria árvore de segmentos ativos */
    ArvoreSegmentos arvore = arvore_criar(origem);
    if (arvore == NULL)
    {
        destruir_lista(segmentos, destruir_segmento_callback);
        destruir_lista(eventos, destruir_evento);
        return NULL;
    }
    
    /* Cria polígono de saída usando TAD */
    Poligono resultado = poligono_criar();
    if (resultado == NULL)
    {
        arvore_destruir(arvore);
        destruir_lista(segmentos, destruir_segmento_callback);
        destruir_lista(eventos, destruir_evento);
        return NULL;
    }
    
    /* Inicializa árvore com segmentos que cruzam ângulo 0 */
    No atual = obter_primeiro(segmentos);
    while (atual != NULL)
    {
        Segmento seg = (Segmento)obter_elemento(atual);
        double dist = distancia_raio_segmento(origem, 0.0, seg);
        if (dist < 1e9)
        {
            arvore_inserir(arvore, seg);
        }
        atual = obter_proximo(atual);
    }
    
    /* Ponto inicial do polígono */
    Segmento biombo = arvore_obter_primeiro(arvore);
    Ponto ultimo_ponto = NULL;
    
    if (biombo != NULL)
    {
        Ponto dir = criar_ponto(ox + 1000, oy);
        Ponto intersecao = NULL;
        
        if (intersecao_raio_segmento(origem, dir, biombo, &intersecao))
        {
            poligono_inserir_vertice(resultado, get_ponto_x(intersecao), get_ponto_y(intersecao));
            ultimo_ponto = intersecao; /* Mantemos ownership desta copia para comparação */
        }
        destruir_ponto(dir);
    }
    
    /* Loop principal de varredura */
    atual = obter_primeiro(eventos);
    while (atual != NULL)
    {
        Evento *evento = (Evento*)obter_elemento(atual);
        arvore_definir_angulo(arvore, evento->angulo);
        
        if (evento->tipo == EVENTO_INICIO)
        {
            arvore_inserir(arvore, evento->segmento);
            Segmento novo_biombo = arvore_obter_primeiro(arvore);
            
            if (novo_biombo == evento->segmento && biombo != evento->segmento)
            {
                if (biombo != NULL && ultimo_ponto != NULL)
                {
                    Ponto intersecao = NULL;
                    if (intersecao_raio_segmento(origem, evento->ponto, biombo, &intersecao))
                    {
                        if (!ponto_igual(ultimo_ponto, intersecao))
                        {
                            poligono_inserir_vertice(resultado, get_ponto_x(intersecao), get_ponto_y(intersecao));
                            destruir_ponto(ultimo_ponto);
                            ultimo_ponto = intersecao;
                        }
                        else
                        {
                            destruir_ponto(intersecao);
                        }
                    }
                }
                
                Ponto pt = evento->ponto;
                if (!ultimo_ponto || !ponto_igual(ultimo_ponto, pt))
                {
                    poligono_inserir_vertice(resultado, get_ponto_x(pt), get_ponto_y(pt));
                    if(ultimo_ponto) destruir_ponto(ultimo_ponto);
                    ultimo_ponto = clonar_ponto(pt);
                }
                biombo = novo_biombo;
            }
        }
        else /* EVENTO_FIM */
        {
            if (evento->segmento == biombo)
            {
                Ponto pt = evento->ponto;
                if (!ultimo_ponto || !ponto_igual(ultimo_ponto, pt))
                {
                    poligono_inserir_vertice(resultado, get_ponto_x(pt), get_ponto_y(pt));
                    if(ultimo_ponto) destruir_ponto(ultimo_ponto);
                    ultimo_ponto = clonar_ponto(pt);
                }
                
                arvore_remover(arvore, evento->segmento);
                Segmento novo_biombo = arvore_obter_primeiro(arvore);
                
                if (novo_biombo != NULL)
                {
                    Ponto intersecao = NULL;
                    if (intersecao_raio_segmento(origem, evento->ponto, novo_biombo, &intersecao))
                    {
                        if (!ultimo_ponto || !ponto_igual(ultimo_ponto, intersecao))
                        {
                             poligono_inserir_vertice(resultado, get_ponto_x(intersecao), get_ponto_y(intersecao));
                             if(ultimo_ponto) destruir_ponto(ultimo_ponto);
                             ultimo_ponto = intersecao;
                        }
                        else
                        {
                            destruir_ponto(intersecao);
                        }
                    }
                }
                biombo = novo_biombo;
            }
            else
            {
                arvore_remover(arvore, evento->segmento);
            }
        }
        
        atual = obter_proximo(atual);
    }
    
    if (ultimo_ponto) destruir_ponto(ultimo_ponto);
    
    arvore_destruir(arvore);
    destruir_lista(segmentos, destruir_segmento_callback);
    destruir_lista(eventos, destruir_evento);
    
    return (PoligonoVisibilidade)resultado;
}

/* Função auxiliar para verificar se segmento já está na lista (evita duplicatas) */
static int segmento_na_lista(Lista lista, Segmento seg)
{
    if (lista == NULL || seg == NULL) return 0;
    
    int id = get_segmento_id(seg);
    No atual = obter_primeiro(lista);
    while (atual != NULL)
    {
        Segmento s = (Segmento)obter_elemento(atual);
        if (get_segmento_id(s) == id) return 1;
        atual = obter_proximo(atual);
    }
    return 0;
}

/* Função auxiliar para verificar se é segmento de bounding box (ID negativo) */
static int eh_segmento_bbox(Segmento seg)
{
    return get_segmento_id(seg) < 0;
}

PoligonoVisibilidade calcular_visibilidade_com_segmentos(
    Ponto origem, Lista segmentos_entrada,
    double min_x, double min_y,
    double max_x, double max_y,
    const char *tipo_ordenacao,
    int limiar_insertion,
    Lista segmentos_visiveis)
{
    if (origem == NULL) return NULL;
    
    /* Cria lista de segmentos de trabalho (cópia + bounding box) */
    Lista segmentos = criar_lista();
    if (segmentos == NULL) return NULL;
    
    /* Cria mapeamento de clones para originais */
    Lista segmentos_originais = criar_lista(); /* Lista paralela dos originais */
    
    /* Copia segmentos de entrada, mantendo referência aos originais */
    if (segmentos_entrada != NULL)
    {
        No atual = obter_primeiro(segmentos_entrada);
        while (atual != NULL)
        {
            Segmento seg = (Segmento)obter_elemento(atual);
            inserir_fim(segmentos, clonar_segmento(seg));
            inserir_fim(segmentos_originais, seg); /* Referência ao original */
            atual = obter_proximo(atual);
        }
    }
    
    /* Expande bounding box para incluir a origem */
    double ox = get_ponto_x(origem);
    double oy = get_ponto_y(origem);
    if (ox < min_x) min_x = ox;
    if (ox > max_x) max_x = ox;
    if (oy < min_y) min_y = oy;
    if (oy > max_y) max_y = oy;
    
    /* Adiciona bounding box (segmentos artificiais com ID negativo) */
    criar_bounding_box(segmentos, min_x, min_y, max_x, max_y);
    
    /* PRÉ-PROCESSAMENTO: Divisão de Segmentos no Ângulo 0 */
    No node_seg = obter_primeiro(segmentos);
    while (node_seg != NULL)
    {
        Segmento seg = (Segmento)obter_elemento(node_seg);
        No proximo_node = obter_proximo(node_seg);
        
        Ponto dir_zero = criar_ponto(ox + 1.0, oy);
        Ponto intersecao = NULL;
        
        if (intersecao_raio_segmento(origem, dir_zero, seg, &intersecao))
        {
            double ix = get_ponto_x(intersecao);
            double iy = get_ponto_y(intersecao);
            
            double x1 = get_segmento_x1(seg);
            double y1 = get_segmento_y1(seg);
            double x2 = get_segmento_x2(seg);
            double y2 = get_segmento_y2(seg);
            
            if (hypot(ix - x1, iy - y1) > EPSILON &&
                hypot(ix - x2, iy - y2) > EPSILON)
            {
                int id = get_segmento_id(seg);
                int id_orig = get_segmento_id_original(seg);
                const char *cor = get_segmento_cor(seg);
                
                Segmento s1 = criar_segmento(id, id_orig, x1, y1, ix, iy, cor);
                Segmento s2 = criar_segmento(id, id_orig, ix, iy, x2, y2, cor);
                
                inserir_fim(segmentos, s1);
                inserir_fim(segmentos, s2);
                
                void *removido = remover_no(segmentos, node_seg);
                if (removido) destruir_segmento((Segmento)removido);
            }
            destruir_ponto(intersecao);
        }
        destruir_ponto(dir_zero);
        
        node_seg = proximo_node;
    }
    
    /* Cria lista de eventos */
    Lista eventos = criar_lista();
    No atual = obter_primeiro(segmentos);
    while (atual != NULL)
    {
        Segmento seg = (Segmento)obter_elemento(atual);
        Ponto p1 = get_segmento_p1(seg);
        Ponto p2 = get_segmento_p2(seg);
        
        double ang1 = ponto_angulo_polar(origem, p1);
        double ang2 = ponto_angulo_polar(origem, p2);
        
        if (ang1 < ang2 || (fabs(ang1 - ang2) < EPSILON && 
            ponto_distancia(origem, p1) < ponto_distancia(origem, p2)))
        {
            inserir_fim(eventos, criar_evento(p1, EVENTO_INICIO, seg, origem));
            inserir_fim(eventos, criar_evento(p2, EVENTO_FIM, seg, origem));
        }
        else
        {
            inserir_fim(eventos, criar_evento(p2, EVENTO_INICIO, seg, origem));
            inserir_fim(eventos, criar_evento(p1, EVENTO_FIM, seg, origem));
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Ordena eventos */
    ordenar_eventos(eventos, tipo_ordenacao, limiar_insertion);
    
    /* Inicializa árvore de segmentos ativos */
    ArvoreSegmentos arvore = arvore_criar(origem);
    
    /* Inicialização: insere segmentos no ângulo 0 */
    atual = obter_primeiro(segmentos);
    while (atual != NULL)
    {
        Segmento seg = (Segmento)obter_elemento(atual);
        Ponto p1 = get_segmento_p1(seg);
        Ponto p2 = get_segmento_p2(seg);
        
        double ang1 = ponto_angulo_polar(origem, p1);
        double ang2 = ponto_angulo_polar(origem, p2);
        
        if ((ang1 < EPSILON && ang2 > M_PI) || (ang2 < EPSILON && ang1 > M_PI))
        {
            arvore_definir_angulo(arvore, 0.0);
            arvore_inserir(arvore, seg);
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Cria polígono de saída */
    Poligono resultado = poligono_criar();
    if (resultado == NULL)
    {
        destruir_lista(segmentos, destruir_segmento_callback);
        destruir_lista(segmentos_originais, NULL);
        destruir_lista(eventos, destruir_evento);
        arvore_destruir(arvore);
        return NULL;
    }
    
    /* Ponto inicial do polígono */
    Segmento biombo = arvore_obter_primeiro(arvore);
    Ponto ultimo_ponto = NULL;
    
    if (biombo != NULL)
    {
        Ponto dir = criar_ponto(ox + 1000, oy);
        Ponto intersecao = NULL;
        
        if (intersecao_raio_segmento(origem, dir, biombo, &intersecao))
        {
            poligono_inserir_vertice(resultado, get_ponto_x(intersecao), get_ponto_y(intersecao));
            ultimo_ponto = intersecao;
            
            /* Registra biombo inicial como visível */
            if (segmentos_visiveis != NULL && !eh_segmento_bbox(biombo))
            {
                /* Encontra o segmento original correspondente */
                int idx = 0;
                No ns = obter_primeiro(segmentos_entrada);
                while (ns != NULL)
                {
                    Segmento orig = (Segmento)obter_elemento(ns);
                    if (get_segmento_id(orig) == get_segmento_id(biombo) &&
                        !segmento_na_lista(segmentos_visiveis, orig))
                    {
                        inserir_fim(segmentos_visiveis, orig);
                        break;
                    }
                    ns = obter_proximo(ns);
                    idx++;
                }
            }
        }
        destruir_ponto(dir);
    }
    
    /* Loop principal de varredura */
    atual = obter_primeiro(eventos);
    while (atual != NULL)
    {
        Evento *evento = (Evento*)obter_elemento(atual);
        arvore_definir_angulo(arvore, evento->angulo);
        
        if (evento->tipo == EVENTO_INICIO)
        {
            arvore_inserir(arvore, evento->segmento);
            Segmento novo_biombo = arvore_obter_primeiro(arvore);
            
            if (novo_biombo == evento->segmento && biombo != evento->segmento)
            {
                if (biombo != NULL && ultimo_ponto != NULL)
                {
                    Ponto intersecao = NULL;
                    if (intersecao_raio_segmento(origem, evento->ponto, biombo, &intersecao))
                    {
                        if (!ponto_igual(ultimo_ponto, intersecao))
                        {
                            poligono_inserir_vertice(resultado, get_ponto_x(intersecao), get_ponto_y(intersecao));
                            destruir_ponto(ultimo_ponto);
                            ultimo_ponto = intersecao;
                        }
                        else
                        {
                            destruir_ponto(intersecao);
                        }
                    }
                }
                
                Ponto pt = evento->ponto;
                if (!ultimo_ponto || !ponto_igual(ultimo_ponto, pt))
                {
                    poligono_inserir_vertice(resultado, get_ponto_x(pt), get_ponto_y(pt));
                    if(ultimo_ponto) destruir_ponto(ultimo_ponto);
                    ultimo_ponto = clonar_ponto(pt);
                }
                
                /* Registra novo biombo como visível */
                if (segmentos_visiveis != NULL && !eh_segmento_bbox(novo_biombo))
                {
                    No ns = obter_primeiro(segmentos_entrada);
                    while (ns != NULL)
                    {
                        Segmento orig = (Segmento)obter_elemento(ns);
                        if (get_segmento_id(orig) == get_segmento_id(novo_biombo) &&
                            !segmento_na_lista(segmentos_visiveis, orig))
                        {
                            inserir_fim(segmentos_visiveis, orig);
                            break;
                        }
                        ns = obter_proximo(ns);
                    }
                }
                
                biombo = novo_biombo;
            }
        }
        else /* EVENTO_FIM */
        {
            if (evento->segmento == biombo)
            {
                Ponto pt = evento->ponto;
                if (!ultimo_ponto || !ponto_igual(ultimo_ponto, pt))
                {
                    poligono_inserir_vertice(resultado, get_ponto_x(pt), get_ponto_y(pt));
                    if(ultimo_ponto) destruir_ponto(ultimo_ponto);
                    ultimo_ponto = clonar_ponto(pt);
                }
                
                arvore_remover(arvore, evento->segmento);
                Segmento novo_biombo = arvore_obter_primeiro(arvore);
                
                if (novo_biombo != NULL)
                {
                    Ponto intersecao = NULL;
                    if (intersecao_raio_segmento(origem, evento->ponto, novo_biombo, &intersecao))
                    {
                        if (!ultimo_ponto || !ponto_igual(ultimo_ponto, intersecao))
                        {
                             poligono_inserir_vertice(resultado, get_ponto_x(intersecao), get_ponto_y(intersecao));
                             if(ultimo_ponto) destruir_ponto(ultimo_ponto);
                             ultimo_ponto = intersecao;
                        }
                        else
                        {
                            destruir_ponto(intersecao);
                        }
                    }
                    
                    /* Registra novo biombo como visível */
                    if (segmentos_visiveis != NULL && !eh_segmento_bbox(novo_biombo))
                    {
                        No ns = obter_primeiro(segmentos_entrada);
                        while (ns != NULL)
                        {
                            Segmento orig = (Segmento)obter_elemento(ns);
                            if (get_segmento_id(orig) == get_segmento_id(novo_biombo) &&
                                !segmento_na_lista(segmentos_visiveis, orig))
                            {
                                inserir_fim(segmentos_visiveis, orig);
                                break;
                            }
                            ns = obter_proximo(ns);
                        }
                    }
                }
                biombo = novo_biombo;
            }
            else
            {
                arvore_remover(arvore, evento->segmento);
            }
        }
        
        atual = obter_proximo(atual);
    }
    
    if (ultimo_ponto) destruir_ponto(ultimo_ponto);
    
    arvore_destruir(arvore);
    destruir_lista(segmentos, destruir_segmento_callback);
    destruir_lista(segmentos_originais, NULL);
    destruir_lista(eventos, destruir_evento);
    
    return (PoligonoVisibilidade)resultado;
}

/* ============================================================================
 * Funções do Polígono
 * ============================================================================ */

/* ============================================================================
 * Funções do Polígono (Delegators)
 * ============================================================================ */

void destruir_poligono_visibilidade(PoligonoVisibilidade poligono)
{
    poligono_destruir((Poligono)poligono);
}

int poligono_num_vertices(PoligonoVisibilidade poligono)
{
    return poligono_qtd_vertices((Poligono)poligono);
}

Ponto poligono_obter_vertice(PoligonoVisibilidade poligono, int indice)
{
    return poligono_get_vertice((Poligono)poligono, indice);
}

Lista poligono_obter_vertices(PoligonoVisibilidade poligono)
{
    return poligono_obter_lista((Poligono)poligono);
}

/* ============================================================================
 * Conversão de Formas para Segmentos
 * ============================================================================ */

int converter_formas_para_segmentos(Lista lista_formas, Lista lista_segmentos, char orientacao)
{
    if (lista_formas == NULL || lista_segmentos == NULL) return 0;
    
    int contador = 0;
    
    No atual = obter_primeiro(lista_formas);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        TipoForma tipo = getFormaTipo(forma);
        void *dados = getFormaDados(forma);
        int id = getFormaId(forma);
        
        switch (tipo)
        {
            case TIPO_RETANGULO:
            {
                Retangulo r = (Retangulo)dados;
                double x = getRetanguloX(r);
                double y = getRetanguloY(r);
                double w = getRetanguloLargura(r);
                double h = getRetanguloAltura(r);
                const char *cor = getRetanguloCorBorda(r);
                
                /* 4 segmentos do retângulo */
                inserir_fim(lista_segmentos, criar_segmento(id, id, x, y, x+w, y, cor));         /* baixo */
                inserir_fim(lista_segmentos, criar_segmento(id, id, x+w, y, x+w, y+h, cor));     /* direita */
                inserir_fim(lista_segmentos, criar_segmento(id, id, x+w, y+h, x, y+h, cor));     /* cima */
                inserir_fim(lista_segmentos, criar_segmento(id, id, x, y+h, x, y, cor));         /* esquerda */
                contador += 4;
                break;
            }
            
            case TIPO_CIRCULO:
            {
                Circulo c = (Circulo)dados;
                double cx = getCirculoX(c);
                double cy = getCirculoY(c);
                double r = getCirculoRaio(c);
                const char *cor = getCirculoCorBorda(c);
                
                /* Segmento diametral */
                if (orientacao == 'v')
                {
                    inserir_fim(lista_segmentos, criar_segmento(id, id, cx, cy-r, cx, cy+r, cor));
                }
                else /* 'h' */
                {
                    inserir_fim(lista_segmentos, criar_segmento(id, id, cx-r, cy, cx+r, cy, cor));
                }
                contador++;
                break;
            }
            
            case TIPO_LINHA:
            {
                Linha l = (Linha)dados;
                double x1 = getLinhaX1(l);
                double y1 = getLinhaY1(l);
                double x2 = getLinhaX2(l);
                double y2 = getLinhaY2(l);
                const char *cor = getLinhaCor(l);
                
                inserir_fim(lista_segmentos, criar_segmento(id, id, x1, y1, x2, y2, cor));
                contador++;
                break;
            }
            
            case TIPO_TEXTO:
            {
                Texto t = (Texto)dados;
                double x = getTextoX(t);
                double y = getTextoY(t);
                const char *conteudo = getTextoConteudo(t);
                const char *ancora = getTextoAncora(t);
                const char *cor = getTextoCorBorda(t);
                
                double comprimento = 10.0 * strlen(conteudo);
                double x1, x2;
                
                if (ancora[0] == 'i') /* início */
                {
                    x1 = x;
                    x2 = x + comprimento;
                }
                else if (ancora[0] == 'f') /* fim */
                {
                    x1 = x - comprimento;
                    x2 = x;
                }
                else /* meio */
                {
                    x1 = x - comprimento / 2;
                    x2 = x + comprimento / 2;
                }
                
                inserir_fim(lista_segmentos, criar_segmento(id, id, x1, y, x2, y, cor));
                contador++;
                break;
            }
        }
        
        atual = obter_proximo(atual);
    }
    
    return contador;
}
