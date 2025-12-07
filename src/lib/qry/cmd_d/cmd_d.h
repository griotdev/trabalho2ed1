/* cmd_d.h
 *
 * Comando 'd' - Bomba de destruição
 * Sintaxe: d x y sfx
 */

#ifndef CMD_D_H
#define CMD_D_H

#include "lista.h"
#include "ponto.h"

/**
 * Executa o comando 'd' (destruição).
 * Calcula visibilidade e "destrói" formas visíveis.
 * 
 * @param origem Ponto de vista (x, y)
 * @param lista_formas Lista de formas do cenário
 * @param lista_anteparos Lista de segmentos bloqueantes
 * @param dir_saida Diretório de saída
 * @param nome_base Nome base do arquivo
 * @param sufixo Sufixo para o arquivo de saída
 * @param bbox Bounding box [min_x, min_y, max_x, max_y]
 * @return Número de formas destruídas
 */
int executar_cmd_d(Ponto origem,
                   Lista lista_formas,
                   Lista lista_anteparos,
                   const char *dir_saida,
                   const char *nome_base,
                   const char *sufixo,
                   double bbox[4]);

#endif /* CMD_D_H */
