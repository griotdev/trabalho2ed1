/* cmd_a.h
 *
 * Comando 'a' - Transforma formas em anteparos (segmentos bloqueantes)
 * Sintaxe: a i j [v|h]
 */

#ifndef CMD_A_H
#define CMD_A_H

#include "lista.h"

/**
 * Executa o comando 'a' (anteparo).
 * Transforma formas com ID de i até j em segmentos bloqueantes.
 * 
 * @param lista_formas Lista de formas do cenário
 * @param lista_anteparos Lista onde os segmentos serão inseridos
 * @param id_inicio ID inicial (inclusive)
 * @param id_fim ID final (inclusive)
 * @param orientacao 'h' para horizontal, 'v' para vertical (para círculos)
 * @return Número de formas convertidas
 */
int executar_cmd_a(Lista lista_formas,
                   Lista lista_anteparos,
                   int id_inicio,
                   int id_fim,
                   char orientacao,
                   int *proximo_id,
                   const char *dir_saida,
                   const char *nome_base,
                   const char *sufixo);

#endif /* CMD_A_H */
