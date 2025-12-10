/* cmd_cln.h
 *
 * Comando 'cln' - Bomba de clonagem
 * Sintaxe: cln x y dx dy sfx
 */

#ifndef CMD_CLN_H
#define CMD_CLN_H

#include "lista.h"
#include "ponto.h"

/**
 * Executa o comando 'cln' (clonagem).
 * Calcula visibilidade e clona formas visíveis com deslocamento.
 * 
 * @param origem Ponto de vista (x, y)
 * @param lista_formas Lista de formas do cenário (clones serão adicionados)
 * @param lista_anteparos Lista de segmentos bloqueantes
 * @param dx Deslocamento X para os clones
 * @param dy Deslocamento Y para os clones
 * @param dir_saida Diretório de saída
 * @param nome_base Nome base do arquivo
 * @param sufixo Sufixo para o arquivo de saída
 * @param bbox Bounding box [min_x, min_y, max_x, max_y]
 * @param proximo_id Ponteiro para o próximo ID disponível
 * @param algoritmo_ordenacao Algoritmo de ordenação
 * @return Número de formas clonadas
 */
int executar_cmd_cln(Ponto origem,
                     Lista lista_formas,
                     Lista lista_anteparos,
                     double dx,
                     double dy,
                     const char *dir_saida,
                     const char *nome_base,
                     const char *sufixo,
                     double bbox[4],
                     int *proximo_id,
                     const char *algoritmo_ordenacao);

#endif /* CMD_CLN_H */
