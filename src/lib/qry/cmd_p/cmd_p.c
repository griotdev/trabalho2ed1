/* cmd_p.c
 *
 * Implementação do comando 'P' (bomba de pintura)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_p.h"
#include "lista.h"
#include "ponto.h"
#include "segmento.h"
#include "visibilidade.h"
#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "svg.h"

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
 * Altera a cor de uma forma.
 */
static void pintar_forma(Forma forma, const char *cor)
{
    TipoForma tipo = getFormaTipo(forma);
    void *dados = getFormaDados(forma);
    
    switch (tipo)
    {
        case TIPO_CIRCULO:
            setCirculoCores((Circulo)dados, cor, cor);
            break;
            
        case TIPO_RETANGULO:
            setRetanguloCores((Retangulo)dados, cor, cor);
            break;
            
        case TIPO_LINHA:
            /* Linha não tem setter de cor no header, ignora */
            (void)cor;
            break;
            
        case TIPO_TEXTO:
            setTextoCores((Texto)dados, cor, cor);
            break;
    }
}

/**
 * Gera o arquivo de relatório TXT.
 */
static void gerar_relatorio_txt(const char *caminho, Lista formas_pintadas, const char *cor)
{
    FILE *arquivo = fopen(caminho, "a");
    if (arquivo == NULL) return;
    
    fprintf(arquivo, "P: cor=%s\n", cor);
    
    No atual = obter_primeiro(formas_pintadas);
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
        
        fprintf(arquivo, "  %d %s\n", id, tipo_str);
        atual = obter_proximo(atual);
    }
    
    fclose(arquivo);
}

/* ============================================================================
 * Implementação
 * ============================================================================ */

int executar_cmd_p(Ponto origem,
                   Lista lista_formas,
                   Lista lista_anteparos,
                   const char *cor,
                   const char *dir_saida,
                   const char *nome_base,
                   const char *sufixo,
                   double bbox[4])
{
    if (origem == NULL || lista_formas == NULL || cor == NULL)
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
    
    /* Identifica e pinta formas visíveis */
    Lista formas_pintadas = criar_lista();
    int contador = 0;
    
    No atual = obter_primeiro(lista_formas);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        
        if (forma_visivel(forma, poligono))
        {
            pintar_forma(forma, cor);
            inserir_fim(formas_pintadas, forma);
            contador++;
        }
        
        atual = obter_proximo(atual);
    }
    
    /* Gera relatório */
    char caminho_txt[MAX_CAMINHO];
    snprintf(caminho_txt, MAX_CAMINHO, "%s/%s-consultas.txt", dir_saida, nome_base);
    gerar_relatorio_txt(caminho_txt, formas_pintadas, cor);
    
    /* Limpa */
    destruir_lista(formas_pintadas, NULL);
    destruir_poligono_visibilidade(poligono);
    
    (void)sufixo; /* Usado para SVG se necessário */
    
    return contador;
}
