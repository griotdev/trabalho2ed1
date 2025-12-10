/* teste_lista.c
 * Teste unitário para o módulo Lista
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lista.h"

/* Helper para inteiros */
int* criar_int(int valor) {
    int *p = malloc(sizeof(int));
    *p = valor;
    return p;
}

int cmp_int(const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    return va - vb;
}

void print_int(void *el, void *extra) {
    (void)extra;
    printf("%d ", *(int*)el);
}

void testar_criacao() {
    printf("Teste Criação: ");
    Lista l = criar_lista();
    assert(l != NULL);
    assert(lista_vazia(l));
    assert(obter_tamanho(l) == 0);
    destruir_lista(l, NULL);
    printf("OK\n");
}

void testar_insercao() {
    printf("Teste Inserção: ");
    Lista l = criar_lista();
    
    /* Inserir Fim: [10, 20] */
    inserir_fim(l, criar_int(10));
    inserir_fim(l, criar_int(20));
    
    /* Inserir Inicio: [5, 10, 20] */
    inserir_inicio(l, criar_int(5));
    
    assert(obter_tamanho(l) == 3);
    
    No n = obter_primeiro(l);
    assert(*(int*)obter_elemento(n) == 5);
    n = obter_proximo(n);
    assert(*(int*)obter_elemento(n) == 10);
    n = obter_proximo(n);
    assert(*(int*)obter_elemento(n) == 20);
    
    destruir_lista(l, free);
    printf("OK\n");
}

void testar_remocao() {
    printf("Teste Remoção: ");
    Lista l = criar_lista();
    inserir_fim(l, criar_int(1));
    inserir_fim(l, criar_int(2));
    inserir_fim(l, criar_int(3));
    
    int *p = remover_inicio(l); /* Remove 1 */
    assert(*p == 1);
    free(p);
    
    p = remover_fim(l); /* Remove 3 */
    assert(*p == 3);
    free(p);
    
    assert(obter_tamanho(l) == 1);
    p = remover_inicio(l); /* Remove 2 */
    assert(*p == 2);
    free(p);
    
    assert(lista_vazia(l));
    destruir_lista(l, NULL);
    printf("OK\n");
}

/* Novo teste de ordenação - REQUER integração com sort */
void testar_ordenacao() {
    printf("Teste Ordenação: ");
    Lista l = criar_lista();
    int valores[] = {5, 2, 8, 1, 9, 3};
    for(int i=0; i<6; i++) inserir_fim(l, criar_int(valores[i]));
    
    /* Ordena */
    ordenar_lista(l, cmp_int);
    
    No n = obter_primeiro(l);
    int anterior = -999;
    while(n) {
        int v = *(int*)obter_elemento(n);
        assert(v >= anterior);
        anterior = v;
        n = obter_proximo(n);
    }
    
    destruir_lista(l, free);
    printf("OK\n");
}

int main() {
    printf("=== Testes Unitários: Lista ===\n");
    testar_criacao();
    testar_insercao();
    testar_remocao();
    testar_ordenacao();
    printf("Todos os testes passaram!\n");
    return 0;
}
