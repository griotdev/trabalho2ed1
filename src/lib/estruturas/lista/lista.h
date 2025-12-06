/* lista.h
 *
 * TAD para Lista Duplamente Encadeada Genérica.
 * Armazena elementos de qualquer tipo através de ponteiros void*.
 * Suporta operações de inserção, remoção, busca e iteração.
 */

#ifndef LISTA_H
#define LISTA_H

/* ============================================================================
 * Tipos Opacos
 * ============================================================================ */

/**
 * Tipo opaco para a lista duplamente encadeada.
 * A estrutura interna é definida apenas no arquivo .c
 */
typedef void* Lista;

/**
 * Tipo opaco para um nó da lista.
 * Usado para iteração sobre os elementos.
 */
typedef void* No;

/* ============================================================================
 * Tipos de Funções (Callbacks)
 * ============================================================================ */

/**
 * Função de comparação entre dois elementos.
 * Usada para busca e ordenação.
 * 
 * @param a Primeiro elemento
 * @param b Segundo elemento
 * @return < 0 se a < b, 0 se a == b, > 0 se a > b
 */
typedef int (*FuncaoComparar)(const void *a, const void *b);

/**
 * Função para destruir/liberar um elemento.
 * Chamada quando o elemento é removido ou quando a lista é destruída.
 * 
 * @param elemento Ponteiro para o elemento a ser liberado
 */
typedef void (*FuncaoDestruir)(void *elemento);

/**
 * Função para aplicar a cada elemento (map/foreach).
 * 
 * @param elemento Ponteiro para o elemento
 * @param dados_usuario Dados extras passados pelo usuário
 */
typedef void (*FuncaoAplicar)(void *elemento, void *dados_usuario);

/* ============================================================================
 * Funções de Criação e Destruição
 * ============================================================================ */

/**
 * Cria uma nova lista duplamente encadeada vazia.
 * 
 * @return Ponteiro para a lista criada, ou NULL em caso de erro de alocação
 * 
 * Exemplo:
 *   Lista minhaLista = criar_lista();
 */
Lista criar_lista(void);

/**
 * Destrói a lista e libera toda a memória alocada.
 * Se uma função de destruição for fornecida, ela será chamada para cada elemento.
 * 
 * @param lista Ponteiro para a lista
 * @param destruir Função para liberar cada elemento (pode ser NULL)
 * 
 * @post A lista é completamente liberada
 * @post O ponteiro não deve mais ser usado após esta chamada
 */
void destruir_lista(Lista lista, FuncaoDestruir destruir);

/* ============================================================================
 * Funções de Inserção
 * ============================================================================ */

/**
 * Insere um elemento no início da lista.
 * 
 * @param lista Ponteiro para a lista
 * @param elemento Ponteiro para o elemento a ser inserido
 * @return Ponteiro para o nó criado, ou NULL em caso de erro
 * 
 * @pre lista != NULL
 * @post O elemento se torna o primeiro da lista
 */
No inserir_inicio(Lista lista, void *elemento);

/**
 * Insere um elemento no final da lista.
 * 
 * @param lista Ponteiro para a lista
 * @param elemento Ponteiro para o elemento a ser inserido
 * @return Ponteiro para o nó criado, ou NULL em caso de erro
 * 
 * @pre lista != NULL
 * @post O elemento se torna o último da lista
 */
No inserir_fim(Lista lista, void *elemento);

/**
 * Insere um elemento após um nó específico.
 * 
 * @param lista Ponteiro para a lista
 * @param no Nó após o qual inserir
 * @param elemento Ponteiro para o elemento a ser inserido
 * @return Ponteiro para o nó criado, ou NULL em caso de erro
 * 
 * @pre lista != NULL
 * @pre no != NULL e deve pertencer à lista
 */
No inserir_apos(Lista lista, No no, void *elemento);

/**
 * Insere um elemento antes de um nó específico.
 * 
 * @param lista Ponteiro para a lista
 * @param no Nó antes do qual inserir
 * @param elemento Ponteiro para o elemento a ser inserido
 * @return Ponteiro para o nó criado, ou NULL em caso de erro
 * 
 * @pre lista != NULL
 * @pre no != NULL e deve pertencer à lista
 */
No inserir_antes(Lista lista, No no, void *elemento);

/* ============================================================================
 * Funções de Remoção
 * ============================================================================ */

/**
 * Remove um nó específico da lista e retorna seu elemento.
 * A memória do nó é liberada, mas o elemento NÃO é destruído.
 * 
 * @param lista Ponteiro para a lista
 * @param no Nó a ser removido
 * @return Ponteiro para o elemento que estava no nó, ou NULL se erro
 * 
 * @pre lista != NULL
 * @pre no != NULL e deve pertencer à lista
 * @post O nó é removido e sua memória liberada
 */
void* remover_no(Lista lista, No no);

/**
 * Remove o primeiro elemento da lista.
 * 
 * @param lista Ponteiro para a lista
 * @return Ponteiro para o elemento removido, ou NULL se lista vazia
 */
void* remover_inicio(Lista lista);

/**
 * Remove o último elemento da lista.
 * 
 * @param lista Ponteiro para a lista
 * @return Ponteiro para o elemento removido, ou NULL se lista vazia
 */
void* remover_fim(Lista lista);

/* ============================================================================
 * Funções de Acesso e Iteração
 * ============================================================================ */

/**
 * Obtém o primeiro nó da lista.
 * 
 * @param lista Ponteiro para a lista
 * @return Primeiro nó, ou NULL se lista vazia
 */
No obter_primeiro(Lista lista);

/**
 * Obtém o último nó da lista.
 * 
 * @param lista Ponteiro para a lista
 * @return Último nó, ou NULL se lista vazia
 */
No obter_ultimo(Lista lista);

/**
 * Obtém o próximo nó (sucessor).
 * 
 * @param no Nó atual
 * @return Próximo nó, ou NULL se for o último
 */
No obter_proximo(No no);

/**
 * Obtém o nó anterior (predecessor).
 * 
 * @param no Nó atual
 * @return Nó anterior, ou NULL se for o primeiro
 */
No obter_anterior(No no);

/**
 * Obtém o elemento armazenado em um nó.
 * 
 * @param no Ponteiro para o nó
 * @return Ponteiro para o elemento, ou NULL se nó inválido
 */
void* obter_elemento(No no);

/**
 * Obtém o número de elementos na lista.
 * 
 * @param lista Ponteiro para a lista
 * @return Número de elementos (0 se lista vazia ou inválida)
 */
int obter_tamanho(Lista lista);

/**
 * Verifica se a lista está vazia.
 * 
 * @param lista Ponteiro para a lista
 * @return 1 se vazia, 0 caso contrário
 */
int lista_vazia(Lista lista);

/* ============================================================================
 * Funções de Busca
 * ============================================================================ */

/**
 * Busca um elemento na lista usando função de comparação.
 * 
 * @param lista Ponteiro para a lista
 * @param chave Chave de busca (passada como primeiro argumento para comparar)
 * @param comparar Função de comparação
 * @return Nó contendo o elemento, ou NULL se não encontrado
 */
No buscar(Lista lista, const void *chave, FuncaoComparar comparar);

/* ============================================================================
 * Funções de Mapeamento
 * ============================================================================ */

/**
 * Aplica uma função a todos os elementos da lista.
 * 
 * @param lista Ponteiro para a lista
 * @param aplicar Função a ser aplicada a cada elemento
 * @param dados_usuario Dados extras a serem passados para a função
 */
void percorrer(Lista lista, FuncaoAplicar aplicar, void *dados_usuario);

/* ============================================================================
 * Funções de Ordenação
 * ============================================================================ */

/**
 * Ordena a lista usando o algoritmo de ordenação configurado.
 * Por padrão usa MergeSort com otimização para InsertionSort em sub-listas pequenas.
 * 
 * @param lista Ponteiro para a lista
 * @param comparar Função de comparação entre elementos
 * 
 * @post A lista é ordenada em ordem crescente segundo a função de comparação
 */
void ordenar_lista(Lista lista, FuncaoComparar comparar);

#endif /* LISTA_H */
