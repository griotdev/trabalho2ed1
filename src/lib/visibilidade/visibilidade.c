/* visibilidade.c
 *
 * Implementação do Algoritmo de Região de Visibilidade
 * Varredura Angular (Angular Plane Sweep)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

/* Estrutura do polígono de visibilidade */
typedef struct poligono_internal
{
    Lista vertices;     /* Lista de Ponto */
    int num_vertices;
} PoligonoInternal;

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
    inserir_fim(segmentos, criar_segmento(-1, min_x, min_y, max_x, min_y, "none")); /* baixo */
    inserir_fim(segmentos, criar_segmento(-2, max_x, min_y, max_x, max_y, "none")); /* direita */
    inserir_fim(segmentos, criar_segmento(-3, max_x, max_y, min_x, max_y, "none")); /* cima */
    inserir_fim(segmentos, criar_segmento(-4, min_x, max_y, min_x, min_y, "none")); /* esquerda */
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
static void ordenar_eventos(Lista eventos)
{
    int n = obter_tamanho(eventos);
    if (n <= 1) return;
    
    /* Converte para array para usar qsort */
    Evento **arr = (Evento**)malloc(n * sizeof(Evento*));
    if (arr == NULL) return;
    
    No atual = obter_primeiro(eventos);
    for (int i = 0; i < n && atual != NULL; i++)
    {
        arr[i] = (Evento*)obter_elemento(atual);
        atual = obter_proximo(atual);
    }
    
    qsort(arr, n, sizeof(Evento*), comparar_eventos);
    
    /* Reconstrói a lista na ordem correta */
    atual = obter_primeiro(eventos);
    for (int i = 0; i < n && atual != NULL; i++)
    {
        /* Troca o elemento no nó (hack para reordenar sem realocar nós) */
        /* Por simplicidade, vamos reconstruir a lista */
        atual = obter_proximo(atual);
    }
    
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
                                            double max_x, double max_y)
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
    
    /* Adiciona bounding box */
    criar_bounding_box(segmentos, min_x, min_y, max_x, max_y);
    
    /* Extrai eventos (vértices) */
    Lista eventos = extrair_eventos(segmentos, origem);
    if (eventos == NULL || lista_vazia(eventos))
    {
        destruir_lista(segmentos, destruir_segmento_callback);
        if (eventos) destruir_lista(eventos, destruir_evento);
        return NULL;
    }
    
    /* Ordena eventos por ângulo */
    ordenar_eventos(eventos);
    
    /* Cria árvore de segmentos ativos */
    ArvoreSegmentos arvore = arvore_criar(origem);
    if (arvore == NULL)
    {
        destruir_lista(segmentos, destruir_segmento_callback);
        destruir_lista(eventos, destruir_evento);
        return NULL;
    }
    
    /* Cria polígono de saída */
    PoligonoInternal *resultado = (PoligonoInternal*)malloc(sizeof(PoligonoInternal));
    if (resultado == NULL)
    {
        arvore_destruir(arvore);
        destruir_lista(segmentos, destruir_segmento_callback);
        destruir_lista(eventos, destruir_evento);
        return NULL;
    }
    
    resultado->vertices = criar_lista();
    resultado->num_vertices = 0;
    
    /* Inicializa árvore com segmentos que cruzam ângulo 0 */
    No atual = obter_primeiro(segmentos);
    while (atual != NULL)
    {
        Segmento seg = (Segmento)obter_elemento(atual);
        
        /* Verifica se o segmento cruza o raio horizontal (ângulo 0) */
        double dist = distancia_raio_segmento(origem, 0.0, seg);
        if (dist < 1e9)
        {
            arvore_inserir(arvore, seg);
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Ponto inicial do polígono (interseção com biombo inicial) */
    Segmento biombo = arvore_obter_primeiro(arvore);
    Ponto ultimo_ponto = NULL;
    
    if (biombo != NULL)
    {
        Ponto dir = criar_ponto(get_ponto_x(origem) + 1000, get_ponto_y(origem));
        Ponto intersecao = NULL;
        
        if (intersecao_raio_segmento(origem, dir, biombo, &intersecao))
        {
            inserir_fim(resultado->vertices, intersecao);
            resultado->num_vertices++;
            ultimo_ponto = intersecao;
        }
        
        destruir_ponto(dir);
    }
    
    /* Loop principal de varredura */
    atual = obter_primeiro(eventos);
    while (atual != NULL)
    {
        Evento *evento = (Evento*)obter_elemento(atual);
        
        /* Atualiza ângulo da árvore */
        arvore_definir_angulo(arvore, evento->angulo);
        
        if (evento->tipo == EVENTO_INICIO)
        {
            /* Insere segmento na árvore */
            arvore_inserir(arvore, evento->segmento);
            
            /* Verifica se é o novo biombo (mais perto) */
            Segmento novo_biombo = arvore_obter_primeiro(arvore);
            
            if (novo_biombo == evento->segmento && biombo != evento->segmento)
            {
                /* O novo segmento é agora o biombo */
                
                /* Calcula interseção com biombo anterior */
                if (biombo != NULL && ultimo_ponto != NULL)
                {
                    Ponto intersecao = NULL;
                    if (intersecao_raio_segmento(origem, evento->ponto, biombo, &intersecao))
                    {
                        if (!ponto_igual(ultimo_ponto, intersecao))
                        {
                            inserir_fim(resultado->vertices, intersecao);
                            resultado->num_vertices++;
                            ultimo_ponto = intersecao;
                        }
                        else
                        {
                            destruir_ponto(intersecao);
                        }
                    }
                }
                
                /* Adiciona o ponto do evento */
                Ponto ponto_copia = clonar_ponto(evento->ponto);
                if (!ponto_igual(ultimo_ponto, ponto_copia))
                {
                    inserir_fim(resultado->vertices, ponto_copia);
                    resultado->num_vertices++;
                    ultimo_ponto = ponto_copia;
                }
                else
                {
                    destruir_ponto(ponto_copia);
                }
                
                biombo = novo_biombo;
            }
        }
        else /* EVENTO_FIM */
        {
            /* Verifica se é o fim do biombo atual */
            if (evento->segmento == biombo)
            {
                /* Adiciona o ponto final do biombo */
                Ponto ponto_copia = clonar_ponto(evento->ponto);
                if (ultimo_ponto == NULL || !ponto_igual(ultimo_ponto, ponto_copia))
                {
                    inserir_fim(resultado->vertices, ponto_copia);
                    resultado->num_vertices++;
                    ultimo_ponto = ponto_copia;
                }
                else
                {
                    destruir_ponto(ponto_copia);
                }
                
                /* Remove o segmento */
                arvore_remover(arvore, evento->segmento);
                
                /* Encontra novo biombo */
                Segmento novo_biombo = arvore_obter_primeiro(arvore);
                
                if (novo_biombo != NULL)
                {
                    /* Calcula interseção com novo biombo */
                    Ponto intersecao = NULL;
                    if (intersecao_raio_segmento(origem, evento->ponto, novo_biombo, &intersecao))
                    {
                        if (!ponto_igual(ultimo_ponto, intersecao))
                        {
                            inserir_fim(resultado->vertices, intersecao);
                            resultado->num_vertices++;
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
                /* Remove segmento que não é biombo */
                arvore_remover(arvore, evento->segmento);
            }
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Limpa recursos temporários */
    arvore_destruir(arvore);
    destruir_lista(segmentos, destruir_segmento_callback);
    destruir_lista(eventos, destruir_evento);
    
    return (PoligonoVisibilidade)resultado;
}

/* ============================================================================
 * Funções do Polígono
 * ============================================================================ */

void destruir_poligono_visibilidade(PoligonoVisibilidade poligono)
{
    PoligonoInternal *p = (PoligonoInternal*)poligono;
    if (p == NULL) return;
    
    if (p->vertices != NULL)
    {
        destruir_lista(p->vertices, (FuncaoDestruir)destruir_ponto);
    }
    
    free(p);
}

int poligono_num_vertices(PoligonoVisibilidade poligono)
{
    PoligonoInternal *p = (PoligonoInternal*)poligono;
    return p ? p->num_vertices : 0;
}

Ponto poligono_obter_vertice(PoligonoVisibilidade poligono, int indice)
{
    PoligonoInternal *p = (PoligonoInternal*)poligono;
    if (p == NULL || indice < 0 || indice >= p->num_vertices) return NULL;
    
    No atual = obter_primeiro(p->vertices);
    for (int i = 0; i < indice && atual != NULL; i++)
    {
        atual = obter_proximo(atual);
    }
    
    return atual ? (Ponto)obter_elemento(atual) : NULL;
}

Lista poligono_obter_vertices(PoligonoVisibilidade poligono)
{
    PoligonoInternal *p = (PoligonoInternal*)poligono;
    return p ? p->vertices : NULL;
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
                inserir_fim(lista_segmentos, criar_segmento(id, x, y, x+w, y, cor));         /* baixo */
                inserir_fim(lista_segmentos, criar_segmento(id, x+w, y, x+w, y+h, cor));     /* direita */
                inserir_fim(lista_segmentos, criar_segmento(id, x+w, y+h, x, y+h, cor));     /* cima */
                inserir_fim(lista_segmentos, criar_segmento(id, x, y+h, x, y, cor));         /* esquerda */
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
                    inserir_fim(lista_segmentos, criar_segmento(id, cx, cy-r, cx, cy+r, cor));
                }
                else /* 'h' */
                {
                    inserir_fim(lista_segmentos, criar_segmento(id, cx-r, cy, cx+r, cy, cor));
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
                
                inserir_fim(lista_segmentos, criar_segmento(id, x1, y1, x2, y2, cor));
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
                
                inserir_fim(lista_segmentos, criar_segmento(id, x1, y, x2, y, cor));
                contador++;
                break;
            }
        }
        
        atual = obter_proximo(atual);
    }
    
    return contador;
}
