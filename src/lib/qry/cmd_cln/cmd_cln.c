/* cmd_cln.c
 *
 * Implementação do comando 'cln' (bomba de clonagem)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_cln.h"
#include "lista.h"
#include "ponto.h"
#include "segmento.h"
#include "visibilidade.h"
#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

#define MAX_CAMINHO 1024

/* ============================================================================
 * Funções Auxiliares
 * ============================================================================ */

/**
 * Verifica se uma forma está dentro do polígono de visibilidade.
 */
static int forma_visivel(Forma forma, PoligonoVisibilidade poligono)
{
    /* TODO: Implementar verificação precisa */
    (void)forma;
    (void)poligono;
    return 1;
}

/**
 * Clona uma forma e aplica deslocamento.
 * O novo ID é passado mas não usado diretamente (forma tem ID interno).
 */
static Forma clonar_forma_com_deslocamento(Forma forma, double dx, double dy, int novo_id)
{
    TipoForma tipo = getFormaTipo(forma);
    void *dados = getFormaDados(forma);
    void *clone_dados = NULL;
    
    (void)novo_id; /* ID não é modificável diretamente nas formas clonadas */
    
    switch (tipo)
    {
        case TIPO_CIRCULO:
        {
            Circulo c = (Circulo)dados;
            clone_dados = clonaCirculo(c);
            if (clone_dados)
            {
                setCirculoPosicao((Circulo)clone_dados,
                                   getCirculoX(c) + dx,
                                   getCirculoY(c) + dy);
            }
            break;
        }
        
        case TIPO_RETANGULO:
        {
            Retangulo r = (Retangulo)dados;
            clone_dados = clonaRetangulo(r);
            if (clone_dados)
            {
                setRetanguloPosicao((Retangulo)clone_dados,
                                     getRetanguloX(r) + dx,
                                     getRetanguloY(r) + dy);
            }
            break;
        }
        
        case TIPO_LINHA:
        {
            Linha l = (Linha)dados;
            clone_dados = clonaLinha(l);
            if (clone_dados)
            {
                setLinhaPosicao((Linha)clone_dados, dx, dy);
            }
            break;
        }
        
        case TIPO_TEXTO:
        {
            Texto t = (Texto)dados;
            clone_dados = clonaTexto(t);
            if (clone_dados)
            {
                setTextoPosicao((Texto)clone_dados,
                                 getTextoX(t) + dx,
                                 getTextoY(t) + dy);
            }
            break;
        }
    }
    
    if (clone_dados == NULL) return NULL;
    
    return criaForma(tipo, clone_dados);
}

/**
 * Gera o arquivo de relatório TXT.
 */
static void gerar_relatorio_txt(const char *caminho, Lista formas_clonadas, 
                                 double dx, double dy)
{
    FILE *arquivo = fopen(caminho, "a");
    if (arquivo == NULL) return;
    
    fprintf(arquivo, "cln: dx=%.2f dy=%.2f\n", dx, dy);
    
    No atual = obter_primeiro(formas_clonadas);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        int id = getFormaId(forma);
        TipoForma tipo = getFormaTipo(forma);
        
        const char *tipo_str;
        switch (tipo)
        {
            case TIPO_CIRCULO: tipo_str = "circulo"; break;
            case TIPO_RETANGULO: tipo_str = "retangulo"; break;
            case TIPO_LINHA: tipo_str = "linha"; break;
            case TIPO_TEXTO: tipo_str = "texto"; break;
            default: tipo_str = "desconhecido";
        }
        
        fprintf(arquivo, "  %d %s (clone)\n", id, tipo_str);
        atual = obter_proximo(atual);
    }
    
    fclose(arquivo);
}

/* ============================================================================
 * Implementação
 * ============================================================================ */

int executar_cmd_cln(Ponto origem,
                     Lista lista_formas,
                     Lista lista_anteparos,
                     double dx,
                     double dy,
                     const char *dir_saida,
                     const char *nome_base,
                     const char *sufixo,
                     double bbox[4],
                     int *proximo_id)
{
    if (origem == NULL || lista_formas == NULL || proximo_id == NULL)
    {
        return 0;
    }
    
    /* Calcula polígono de visibilidade */
    PoligonoVisibilidade poligono = calcular_visibilidade(
        origem, lista_anteparos,
        bbox[0], bbox[1], bbox[2], bbox[3]
    );
    
    if (poligono == NULL)
    {
        fprintf(stderr, "Aviso: falha ao calcular visibilidade\n");
        return 0;
    }
    
    /* Primeiro, coleta formas visíveis (não modifica lista durante iteração) */
    Lista formas_para_clonar = criar_lista();
    
    No atual = obter_primeiro(lista_formas);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        
        if (forma_visivel(forma, poligono))
        {
            inserir_fim(formas_para_clonar, forma);
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Clona e adiciona à lista principal */
    Lista clones = criar_lista();
    int contador = 0;
    
    atual = obter_primeiro(formas_para_clonar);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        
        Forma clone = clonar_forma_com_deslocamento(forma, dx, dy, *proximo_id);
        if (clone != NULL)
        {
            inserir_fim(lista_formas, clone);
            inserir_fim(clones, clone);
            (*proximo_id)++;
            contador++;
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Gera relatório */
    char caminho_txt[MAX_CAMINHO];
    snprintf(caminho_txt, MAX_CAMINHO, "%s/%s-consultas.txt", dir_saida, nome_base);
    gerar_relatorio_txt(caminho_txt, clones, dx, dy);
    
    /* Limpa listas temporárias (não destrói formas) */
    destruir_lista(formas_para_clonar, NULL);
    destruir_lista(clones, NULL);
    destruir_poligono_visibilidade(poligono);
    
    (void)sufixo;
    
    return contador;
}
