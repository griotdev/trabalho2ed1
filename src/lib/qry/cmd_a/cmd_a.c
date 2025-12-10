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
/**
 * Converte um retângulo em 4 segmentos.
 */
/**
 * Converte um retângulo em 4 segmentos.
 * Escreve log no arquivo se arquivo_log != NULL.
 */
static int converter_retangulo(Retangulo r, int id_original, Lista lista_segmentos, int *proximo_id, FILE *arquivo_log)
{
    double x = getRetanguloX(r);
    double y = getRetanguloY(r);
    double w = getRetanguloLargura(r);
    double h = getRetanguloAltura(r);
    const char *cor = getRetanguloCorBorda(r);
    
    int ids[4];
    ids[0] = (*proximo_id)++;
    ids[1] = (*proximo_id)++;
    ids[2] = (*proximo_id)++;
    ids[3] = (*proximo_id)++;
    
    /* 4 lados do retângulo */
    inserir_fim(lista_segmentos, criar_segmento(ids[0], id_original, x, y, x+w, y, cor));         /* baixo */
    inserir_fim(lista_segmentos, criar_segmento(ids[1], id_original, x+w, y, x+w, y+h, cor));     /* direita */
    inserir_fim(lista_segmentos, criar_segmento(ids[2], id_original, x+w, y+h, x, y+h, cor));     /* cima */
    inserir_fim(lista_segmentos, criar_segmento(ids[3], id_original, x, y+h, x, y, cor));         /* esquerda */
    
    if (arquivo_log)
    {
        fprintf(arquivo_log, "  %d (retangulo) -> %d, %d, %d, %d\n", id_original, ids[0], ids[1], ids[2], ids[3]);
    }
    
    return 4;
}

/**
 * Converte um círculo em 1 segmento diametral.
 */
static int converter_circulo(Circulo c, int id_original, Lista lista_segmentos, char orientacao, int *proximo_id, FILE *arquivo_log)
{
    double cx = getCirculoX(c);
    double cy = getCirculoY(c);
    double r = getCirculoRaio(c);
    const char *cor = getCirculoCorBorda(c);
    
    int id_seg = (*proximo_id)++;
    
    if (orientacao == 'v')
    {
        /* Segmento vertical */
        inserir_fim(lista_segmentos, criar_segmento(id_seg, id_original, cx, cy-r, cx, cy+r, cor));
    }
    else
    {
        /* Segmento horizontal (default) */
        inserir_fim(lista_segmentos, criar_segmento(id_seg, id_original, cx-r, cy, cx+r, cy, cor));
    }
    
    if (arquivo_log)
    {
        fprintf(arquivo_log, "  %d (circulo) -> %d\n", id_original, id_seg);
    }
    
    return 1;
}

/**
 * Converte uma linha em 1 segmento.
 */
static int converter_linha(Linha l, int id_original, Lista lista_segmentos, int *proximo_id, FILE *arquivo_log)
{
    double x1 = getLinhaX1(l);
    double y1 = getLinhaY1(l);
    double x2 = getLinhaX2(l);
    double y2 = getLinhaY2(l);
    const char *cor = getLinhaCor(l);
    
    int id_seg = (*proximo_id)++;
    
    inserir_fim(lista_segmentos, criar_segmento(id_seg, id_original, x1, y1, x2, y2, cor));
    
    if (arquivo_log)
    {
        fprintf(arquivo_log, "  %d (linha) -> %d\n", id_original, id_seg);
    }
    
    return 1;
}

/**
 * Converte um texto em 1 segmento horizontal.
 */
static int converter_texto(Texto t, int id_original, Lista lista_segmentos, int *proximo_id, FILE *arquivo_log)
{
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
        x1 = x - comprimento / 2.0;
        x2 = x + comprimento / 2.0;
    }
    
    int id_seg = (*proximo_id)++;
    
    inserir_fim(lista_segmentos, criar_segmento(id_seg, id_original, x1, y, x2, y, cor));
    
    if (arquivo_log)
    {
        fprintf(arquivo_log, "  %d (texto) -> %d\n", id_original, id_seg);
    }
    
    return 1;
}

/* ============================================================================
 * Implementação
 * ============================================================================ */

int executar_cmd_a(Lista lista_formas,
                   Lista lista_anteparos,
                   int id_inicio,
                   int id_fim,
                   char orientacao,
                   int *proximo_id,
                   const char *dir_saida,
                   const char *nome_base,
                   const char *sufixo)
{
    if (lista_formas == NULL || lista_anteparos == NULL || proximo_id == NULL)
    {
        return 0;
    }
    
    /* Abre arquivo de log */
    FILE *arquivo_log = NULL;
    if (dir_saida && nome_base && sufixo)
    {
        char caminho[1024];
        /* Formato do nome do arquivo txt: nomebase-sufixo.txt?
           O parser usa: snprintf(caminho_txt, 1024, "%s/%s.txt", dir_saida, sufixo_saida);
           Onde sufixo_saida é nomebase-consultas (se sufixo for NULL) ou nomebase-consultas-sufixo.
           
           Vamos seguir o padrão do parser. O sufixo passado aqui já deve ser o nome final?
           Não, o parser passa 'sufixo_saida' consolidado. 
        */
        snprintf(caminho, 1024, "%s/%s.txt", dir_saida, sufixo);
        arquivo_log = fopen(caminho, "a");
        if (arquivo_log) fprintf(arquivo_log, "a:\n");
    }
    
    int contador = 0;
    
    No atual = obter_primeiro(lista_formas);
    while (atual != NULL)
    {
        Forma forma = (Forma)obter_elemento(atual);
        int id = getFormaId(forma);
        
        /* Verifica se está na faixa de IDs */
        if (id >= id_inicio && id <= id_fim && getFormaAtiva(forma))
        {
            TipoForma tipo = getFormaTipo(forma);
            void *dados = getFormaDados(forma);
            
            switch (tipo)
            {
                case TIPO_RETANGULO:
                    contador += converter_retangulo((Retangulo)dados, id, lista_anteparos, proximo_id, arquivo_log);
                    break;
                    
                case TIPO_CIRCULO:
                    contador += converter_circulo((Circulo)dados, id, lista_anteparos, orientacao, proximo_id, arquivo_log);
                    break;
                    
                case TIPO_LINHA:
                    contador += converter_linha((Linha)dados, id, lista_anteparos, proximo_id, arquivo_log);
                    break;
                    
                case TIPO_TEXTO:
                    contador += converter_texto((Texto)dados, id, lista_anteparos, proximo_id, arquivo_log);
                    break;
            }
            
            /* Marca forma como inativa (não será mais desenhada no SVG) */
            setFormaAtiva(forma, 0);
        }
        
        atual = obter_proximo(atual);
    }
    
    if (arquivo_log)
    {
        fclose(arquivo_log);
    }
    
    return contador;
}
