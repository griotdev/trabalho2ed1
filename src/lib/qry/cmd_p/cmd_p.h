/* cmd_p.h
 *
 * Comando 'P' - Bomba de pintura
 * Sintaxe: P x y cor sfx
 */

#ifndef CMD_P_H
#define CMD_P_H

#include "lista.h"
#include "ponto.h"

/**
 * Executa o comando 'P' (pintura).
 * Calcula visibilidade e pinta formas visíveis.
 * 
 * @param origem Ponto de vista (x, y)
 * @param lista_formas Lista de formas do cenário
 * @param lista_anteparos Lista de segmentos bloqueantes
 * @param cor Nova cor para as formas visíveis
 * @param dir_saida Diretório de saída
 * @param nome_base Nome base do arquivo
 * @param sufixo Sufixo para o arquivo de saída
 * @param bbox Bounding box [min_x, min_y, max_x, max_y]
 * @param algoritmo_ordenacao Algoritmo de ordenação
 * @return Número de formas pintadas
 */
int executar_cmd_p(Ponto origem,
                   Lista lista_formas,
                   Lista lista_anteparos,
                   const char *cor,
                   const char *dir_saida,
                   const char *nome_base,
                   const char *sufixo,

                   double bbox[4],
                   const char *tipo_ordenacao,
                   int limiar_insertion);

#endif /* CMD_P_H */
