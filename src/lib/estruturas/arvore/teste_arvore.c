/* teste_arvore.c
 * Teste unitário para Árvore de Segmentos Ativos
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "arvore.h"
#include "ponto.h"
#include "segmento.h"

void teste_basico() {
    printf("Teste Básico: ");
    Ponto origem = criar_ponto(0, 0);
    ArvoreSegmentos arv = arvore_criar(origem);
    
    assert(arv != NULL);
    assert(arvore_vazia(arv));
    assert(arvore_tamanho(arv) == 0);
    
    arvore_destruir(arv);
    destruir_ponto(origem);
    printf("OK\n");
}

void teste_insercao_remocao() {
    printf("Teste Inserção e Remoção: ");
    Ponto origem = criar_ponto(0, 0);
    ArvoreSegmentos arv = arvore_criar(origem);
    
    /* Segmentos fictícios */
    /* s1: y=2 */
    Segmento s1 = criar_segmento(1, 1, -10, 2, 10, 2, "red");
    /* s2: y=4 (mais longe) */
    Segmento s2 = criar_segmento(2, 2, -10, 4, 10, 4, "blue");
    
    arvore_inserir(arv, s1);
    assert(arvore_tamanho(arv) == 1);
    assert(!arvore_vazia(arv));
    assert(arvore_obter_primeiro(arv) == s1); /* Mais perto */
    
    arvore_inserir(arv, s2);
    assert(arvore_tamanho(arv) == 2);
    assert(arvore_obter_primeiro(arv) == s1); /* s1 ainda é mais perto */
    
    /* Teste próximo */
    assert(arvore_obter_proximo(arv, s1) == s2);
    
    /* Remoção */
    arvore_remover(arv, s1);
    assert(arvore_tamanho(arv) == 1);
    assert(arvore_obter_primeiro(arv) == s2);
    
    arvore_remover(arv, s2);
    assert(arvore_vazia(arv));
    
    arvore_destruir(arv);
    
    destruir_segmento(s1);
    destruir_segmento(s2);
    destruir_ponto(origem);
    printf("OK\n");
}

int main() {
    printf("=== Testes Unitários: Árvore ===\n");
    teste_basico();
    teste_insercao_remocao();
    printf("Todos os testes passaram!\n");
    return 0;
}
