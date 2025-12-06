/* lista.c
 *
 * Implementação do TAD Lista Duplamente Encadeada Genérica.
 */

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

/* ============================================================================
 * Estruturas Internas (Ponteiros Opacos)
 * ============================================================================ */

/**
 * Estrutura interna de um nó da lista.
 */
typedef struct no_internal
{
    void *elemento;              /* Ponteiro para o dado armazenado */
    struct no_internal *proximo; /* Ponteiro para o próximo nó */
    struct no_internal *anterior; /* Ponteiro para o nó anterior */
} NoInternal;

/**
 * Estrutura interna da lista duplamente encadeada.
 */
typedef struct lista_internal
{
    NoInternal *inicio;  /* Ponteiro para o primeiro nó */
    NoInternal *fim;     /* Ponteiro para o último nó */
    int tamanho;         /* Número de elementos na lista */
} ListaInternal;

/* ============================================================================
 * Funções de Criação e Destruição
 * ============================================================================ */

Lista criar_lista(void)
{
    ListaInternal *lista = (ListaInternal*)malloc(sizeof(ListaInternal));
    if (lista == NULL)
    {
        fprintf(stderr, "Erro: falha ao alocar memória para lista.\n");
        return NULL;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;

    return (Lista)lista;
}

void destruir_lista(Lista l, FuncaoDestruir destruir)
{
    ListaInternal *lista = (ListaInternal*)l;
    if (lista == NULL)
    {
        return;
    }

    NoInternal *atual = lista->inicio;
    while (atual != NULL)
    {
        NoInternal *proximo = atual->proximo;
        
        /* Libera o elemento se função de destruição foi fornecida */
        if (destruir != NULL && atual->elemento != NULL)
        {
            destruir(atual->elemento);
        }
        
        free(atual);
        atual = proximo;
    }

    free(lista);
}

/* ============================================================================
 * Funções Auxiliares Internas
 * ============================================================================ */

/**
 * Cria um novo nó com o elemento fornecido.
 */
static NoInternal* criar_no(void *elemento)
{
    NoInternal *no = (NoInternal*)malloc(sizeof(NoInternal));
    if (no == NULL)
    {
        fprintf(stderr, "Erro: falha ao alocar memória para nó.\n");
        return NULL;
    }

    no->elemento = elemento;
    no->proximo = NULL;
    no->anterior = NULL;

    return no;
}

/* ============================================================================
 * Funções de Inserção
 * ============================================================================ */

No inserir_inicio(Lista l, void *elemento)
{
    ListaInternal *lista = (ListaInternal*)l;
    if (lista == NULL)
    {
        return NULL;
    }

    NoInternal *novo = criar_no(elemento);
    if (novo == NULL)
    {
        return NULL;
    }

    if (lista->inicio == NULL)
    {
        /* Lista vazia */
        lista->inicio = novo;
        lista->fim = novo;
    }
    else
    {
        novo->proximo = lista->inicio;
        lista->inicio->anterior = novo;
        lista->inicio = novo;
    }

    lista->tamanho++;
    return (No)novo;
}

No inserir_fim(Lista l, void *elemento)
{
    ListaInternal *lista = (ListaInternal*)l;
    if (lista == NULL)
    {
        return NULL;
    }

    NoInternal *novo = criar_no(elemento);
    if (novo == NULL)
    {
        return NULL;
    }

    if (lista->fim == NULL)
    {
        /* Lista vazia */
        lista->inicio = novo;
        lista->fim = novo;
    }
    else
    {
        novo->anterior = lista->fim;
        lista->fim->proximo = novo;
        lista->fim = novo;
    }

    lista->tamanho++;
    return (No)novo;
}

No inserir_apos(Lista l, No n, void *elemento)
{
    ListaInternal *lista = (ListaInternal*)l;
    NoInternal *no = (NoInternal*)n;
    
    if (lista == NULL || no == NULL)
    {
        return NULL;
    }

    NoInternal *novo = criar_no(elemento);
    if (novo == NULL)
    {
        return NULL;
    }

    novo->anterior = no;
    novo->proximo = no->proximo;

    if (no->proximo != NULL)
    {
        no->proximo->anterior = novo;
    }
    else
    {
        /* no era o último, atualiza fim */
        lista->fim = novo;
    }

    no->proximo = novo;
    lista->tamanho++;

    return (No)novo;
}

No inserir_antes(Lista l, No n, void *elemento)
{
    ListaInternal *lista = (ListaInternal*)l;
    NoInternal *no = (NoInternal*)n;
    
    if (lista == NULL || no == NULL)
    {
        return NULL;
    }

    NoInternal *novo = criar_no(elemento);
    if (novo == NULL)
    {
        return NULL;
    }

    novo->proximo = no;
    novo->anterior = no->anterior;

    if (no->anterior != NULL)
    {
        no->anterior->proximo = novo;
    }
    else
    {
        /* no era o primeiro, atualiza início */
        lista->inicio = novo;
    }

    no->anterior = novo;
    lista->tamanho++;

    return (No)novo;
}

/* ============================================================================
 * Funções de Remoção
 * ============================================================================ */

void* remover_no(Lista l, No n)
{
    ListaInternal *lista = (ListaInternal*)l;
    NoInternal *no = (NoInternal*)n;
    
    if (lista == NULL || no == NULL)
    {
        return NULL;
    }

    void *elemento = no->elemento;

    /* Atualiza ponteiros dos vizinhos */
    if (no->anterior != NULL)
    {
        no->anterior->proximo = no->proximo;
    }
    else
    {
        /* no era o primeiro */
        lista->inicio = no->proximo;
    }

    if (no->proximo != NULL)
    {
        no->proximo->anterior = no->anterior;
    }
    else
    {
        /* no era o último */
        lista->fim = no->anterior;
    }

    free(no);
    lista->tamanho--;

    return elemento;
}

void* remover_inicio(Lista l)
{
    ListaInternal *lista = (ListaInternal*)l;
    if (lista == NULL || lista->inicio == NULL)
    {
        return NULL;
    }

    return remover_no(l, (No)lista->inicio);
}

void* remover_fim(Lista l)
{
    ListaInternal *lista = (ListaInternal*)l;
    if (lista == NULL || lista->fim == NULL)
    {
        return NULL;
    }

    return remover_no(l, (No)lista->fim);
}

/* ============================================================================
 * Funções de Acesso e Iteração
 * ============================================================================ */

No obter_primeiro(Lista l)
{
    ListaInternal *lista = (ListaInternal*)l;
    return lista ? (No)lista->inicio : NULL;
}

No obter_ultimo(Lista l)
{
    ListaInternal *lista = (ListaInternal*)l;
    return lista ? (No)lista->fim : NULL;
}

No obter_proximo(No n)
{
    NoInternal *no = (NoInternal*)n;
    return no ? (No)no->proximo : NULL;
}

No obter_anterior(No n)
{
    NoInternal *no = (NoInternal*)n;
    return no ? (No)no->anterior : NULL;
}

void* obter_elemento(No n)
{
    NoInternal *no = (NoInternal*)n;
    return no ? no->elemento : NULL;
}

int obter_tamanho(Lista l)
{
    ListaInternal *lista = (ListaInternal*)l;
    return lista ? lista->tamanho : 0;
}

int lista_vazia(Lista l)
{
    ListaInternal *lista = (ListaInternal*)l;
    return (lista == NULL || lista->tamanho == 0);
}

/* ============================================================================
 * Funções de Busca
 * ============================================================================ */

No buscar(Lista l, const void *chave, FuncaoComparar comparar)
{
    ListaInternal *lista = (ListaInternal*)l;
    if (lista == NULL || comparar == NULL)
    {
        return NULL;
    }

    NoInternal *atual = lista->inicio;
    while (atual != NULL)
    {
        if (comparar(chave, atual->elemento) == 0)
        {
            return (No)atual;
        }
        atual = atual->proximo;
    }

    return NULL;
}

/* ============================================================================
 * Funções de Mapeamento
 * ============================================================================ */

void percorrer(Lista l, FuncaoAplicar aplicar, void *dados_usuario)
{
    ListaInternal *lista = (ListaInternal*)l;
    if (lista == NULL || aplicar == NULL)
    {
        return;
    }

    NoInternal *atual = lista->inicio;
    while (atual != NULL)
    {
        aplicar(atual->elemento, dados_usuario);
        atual = atual->proximo;
    }
}

/* ============================================================================
 * Funções de Ordenação - MergeSort com otimização InsertionSort
 * ============================================================================ */

/* Tamanho limite para usar InsertionSort */
#define LIMIAR_INSERTION 15

/**
 * InsertionSort para sub-listas pequenas.
 * Opera diretamente sobre um array de ponteiros.
 */
static void insertion_sort(void **array, int n, FuncaoComparar comparar)
{
    for (int i = 1; i < n; i++)
    {
        void *chave = array[i];
        int j = i - 1;
        
        while (j >= 0 && comparar(array[j], chave) > 0)
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = chave;
    }
}

/**
 * Função de merge para MergeSort.
 */
static void merge(void **array, int esq, int meio, int dir, FuncaoComparar comparar)
{
    int n1 = meio - esq + 1;
    int n2 = dir - meio;

    /* Arrays temporários */
    void **esquerda = (void**)malloc(n1 * sizeof(void*));
    void **direita = (void**)malloc(n2 * sizeof(void*));

    if (esquerda == NULL || direita == NULL)
    {
        free(esquerda);
        free(direita);
        return;
    }

    /* Copia para arrays temporários */
    for (int i = 0; i < n1; i++)
        esquerda[i] = array[esq + i];
    for (int j = 0; j < n2; j++)
        direita[j] = array[meio + 1 + j];

    /* Merge */
    int i = 0, j = 0, k = esq;
    while (i < n1 && j < n2)
    {
        if (comparar(esquerda[i], direita[j]) <= 0)
        {
            array[k++] = esquerda[i++];
        }
        else
        {
            array[k++] = direita[j++];
        }
    }

    /* Copia elementos restantes */
    while (i < n1)
        array[k++] = esquerda[i++];
    while (j < n2)
        array[k++] = direita[j++];

    free(esquerda);
    free(direita);
}

/**
 * MergeSort híbrido - usa InsertionSort para partições pequenas.
 */
static void merge_sort_hibrido(void **array, int esq, int dir, FuncaoComparar comparar)
{
    if (esq >= dir)
    {
        return;
    }

    /* Usa InsertionSort para sub-arrays pequenos */
    if (dir - esq + 1 <= LIMIAR_INSERTION)
    {
        insertion_sort(array + esq, dir - esq + 1, comparar);
        return;
    }

    int meio = esq + (dir - esq) / 2;
    merge_sort_hibrido(array, esq, meio, comparar);
    merge_sort_hibrido(array, meio + 1, dir, comparar);
    merge(array, esq, meio, dir, comparar);
}

void ordenar_lista(Lista l, FuncaoComparar comparar)
{
    ListaInternal *lista = (ListaInternal*)l;
    if (lista == NULL || comparar == NULL || lista->tamanho <= 1)
    {
        return;
    }

    int n = lista->tamanho;

    /* Copia elementos para array */
    void **array = (void**)malloc(n * sizeof(void*));
    if (array == NULL)
    {
        fprintf(stderr, "Erro: falha ao alocar memória para ordenação.\n");
        return;
    }

    NoInternal *atual = lista->inicio;
    for (int i = 0; i < n; i++)
    {
        array[i] = atual->elemento;
        atual = atual->proximo;
    }

    /* Ordena o array */
    merge_sort_hibrido(array, 0, n - 1, comparar);

    /* Copia de volta para a lista */
    atual = lista->inicio;
    for (int i = 0; i < n; i++)
    {
        atual->elemento = array[i];
        atual = atual->proximo;
    }

    free(array);
}

/* ============================================================================
 * Main de Teste (compilar com -DTESTE_LISTA)
 * ============================================================================ */

#ifdef TESTE_LISTA

#include <string.h>

/* Função de comparação para inteiros */
static int comparar_int(const void *a, const void *b)
{
    int ia = *(int*)a;
    int ib = *(int*)b;
    return ia - ib;
}

/* Função para imprimir elemento */
static void imprimir_int(void *elemento, void *dados)
{
    (void)dados; /* não usado */
    printf("%d ", *(int*)elemento);
}

int main(void)
{
    printf("=== Teste do Módulo Lista ===\n\n");

    /* Cria a lista */
    Lista lista = criar_lista();
    if (lista == NULL)
    {
        printf("Falha ao criar lista.\n");
        return 1;
    }
    printf("Lista criada com sucesso.\n");

    /* Aloca alguns inteiros para teste */
    int *valores[10];
    for (int i = 0; i < 10; i++)
    {
        valores[i] = (int*)malloc(sizeof(int));
        *valores[i] = (i + 1) * 10;
    }

    /* Testa inserção no fim */
    printf("\nInserindo no fim: ");
    for (int i = 0; i < 5; i++)
    {
        inserir_fim(lista, valores[i]);
        printf("%d ", *valores[i]);
    }
    printf("\n");

    /* Testa inserção no início */
    printf("Inserindo no início: ");
    for (int i = 5; i < 8; i++)
    {
        inserir_inicio(lista, valores[i]);
        printf("%d ", *valores[i]);
    }
    printf("\n");

    /* Imprime a lista */
    printf("\nLista atual (tamanho %d): ", obter_tamanho(lista));
    percorrer(lista, imprimir_int, NULL);
    printf("\n");

    /* Testa busca */
    int chave = 30;
    No encontrado = buscar(lista, &chave, comparar_int);
    printf("\nBusca por %d: %s\n", chave, encontrado ? "encontrado" : "não encontrado");

    /* Testa remoção */
    printf("\nRemovendo primeiro elemento...\n");
    int *removido = (int*)remover_inicio(lista);
    if (removido)
    {
        printf("Removido: %d\n", *removido);
        free(removido);
    }

    printf("Lista após remoção: ");
    percorrer(lista, imprimir_int, NULL);
    printf("\n");

    /* Testa ordenação */
    printf("\nOrdenando lista...\n");
    ordenar_lista(lista, comparar_int);
    printf("Lista ordenada: ");
    percorrer(lista, imprimir_int, NULL);
    printf("\n");

    /* Limpa a lista */
    printf("\nDestruindo lista...\n");
    destruir_lista(lista, free);

    /* Libera valores não inseridos */
    for (int i = 8; i < 10; i++)
    {
        free(valores[i]);
    }

    printf("\n=== Teste concluído com sucesso! ===\n");
    return 0;
}

#endif /* TESTE_LISTA */
