/* cmd_a.c
 *
 * Implementação do comando 'a' (anteparo)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_a.h"
#include "lista.h"
#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "segmento.h"

/* ============================================================================
 * Funções Auxiliares - Conversão de Formas para Segmentos
 * ============================================================================ */

/**
 * Converte um retângulo em 4 segmentos.
 */
static int converter_retangulo(Retangulo r, int id, Lista lista_segmentos)
{
    double x = getRetanguloX(r);
    double y = getRetanguloY(r);
    double w = getRetanguloLargura(r);
    double h = getRetanguloAltura(r);
    
    /* 4 lados do retângulo */
    inserir_fim(lista_segmentos, criar_segmento(id, x, y, x+w, y));         /* baixo */
    inserir_fim(lista_segmentos, criar_segmento(id, x+w, y, x+w, y+h));     /* direita */
    inserir_fim(lista_segmentos, criar_segmento(id, x+w, y+h, x, y+h));     /* cima */
    inserir_fim(lista_segmentos, criar_segmento(id, x, y+h, x, y));         /* esquerda */
    
    return 4;
}

/**
 * Converte um círculo em 1 segmento diametral.
 */
static int converter_circulo(Circulo c, int id, Lista lista_segmentos, char orientacao)
{
    double cx = getCirculoX(c);
    double cy = getCirculoY(c);
    double r = getCirculoRaio(c);
    
    if (orientacao == 'v')
    {
        /* Segmento vertical */
        inserir_fim(lista_segmentos, criar_segmento(id, cx, cy-r, cx, cy+r));
    }
    else
    {
        /* Segmento horizontal (default) */
        inserir_fim(lista_segmentos, criar_segmento(id, cx-r, cy, cx+r, cy));
    }
    
    return 1;
}

/**
 * Converte uma linha em 1 segmento.
 */
static int converter_linha(Linha l, int id, Lista lista_segmentos)
{
    double x1 = getLinhaX1(l);
    double y1 = getLinhaY1(l);
    double x2 = getLinhaX2(l);
    double y2 = getLinhaY2(l);
    
    inserir_fim(lista_segmentos, criar_segmento(id, x1, y1, x2, y2));
    
    return 1;
}

/**
 * Converte um texto em 1 segmento horizontal.
 */
static int converter_texto(Texto t, int id, Lista lista_segmentos)
{
    double x = getTextoX(t);
    double y = getTextoY(t);
    const char *conteudo = getTextoConteudo(t);
    const char *ancora = getTextoAncora(t);
    
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
        x1 = x - comprimento / 2.0;
        x2 = x + comprimento / 2.0;
    }
    
    inserir_fim(lista_segmentos, criar_segmento(id, x1, y, x2, y));
    
    return 1;
}

/* ============================================================================
 * Implementação
 * ============================================================================ */

int executar_cmd_a(Lista lista_formas,
                   Lista lista_anteparos,
                   int id_inicio,
                   int id_fim,
                   char orientacao)
{
    if (lista_formas == NULL || lista_anteparos == NULL)
    {
        return 0;
    }
    
    int contador = 0;
    
    No atual = obter_primeiro(lista_formas);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        int id = getFormaId(forma);
        
        /* Verifica se está na faixa de IDs */
        if (id >= id_inicio && id <= id_fim)
        {
            TipoForma tipo = getFormaTipo(forma);
            void *dados = getFormaDados(forma);
            
            switch (tipo)
            {
                case TIPO_RETANGULO:
                    contador += converter_retangulo((Retangulo)dados, id, lista_anteparos);
                    break;
                    
                case TIPO_CIRCULO:
                    contador += converter_circulo((Circulo)dados, id, lista_anteparos, orientacao);
                    break;
                    
                case TIPO_LINHA:
                    contador += converter_linha((Linha)dados, id, lista_anteparos);
                    break;
                    
                case TIPO_TEXTO:
                    contador += converter_texto((Texto)dados, id, lista_anteparos);
                    break;
            }
        }
        
        atual = obter_proximo(atual);
    }
    
    return contador;
}
