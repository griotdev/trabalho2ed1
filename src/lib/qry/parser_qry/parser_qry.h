/* parser_qry.h
 *
 * Parser de arquivos .qry
 * Lê o arquivo e despacha cada comando para o módulo apropriado.
 */

#ifndef PARSER_QRY_H
#define PARSER_QRY_H

#include "lista.h"

/**
 * Processa um arquivo .qry completo.
 * 
 * @param caminho_qry Caminho do arquivo .qry
 * @param lista_formas Lista de formas do cenário (pode ser modificada)
 * @param lista_anteparos Lista de segmentos bloqueantes (será populada)
 * @param dir_saida Diretório para arquivos de saída
 * @param nome_base Nome base para arquivos de saída
 * @param bbox Bounding box do cenário [min_x, min_y, max_x, max_y]
 * @param algoritmo_ordenacao Algoritmo a ser usado
 * @return Número de comandos processados, ou -1 em caso de erro
 */
int processar_arquivo_qry(const char *caminho_qry,
                          Lista lista_formas,
                          Lista lista_anteparos,
                          const char *dir_saida,
                          const char *nome_base,
                          double bbox[4],
                          const char *algoritmo_ordenacao);

#endif /* PARSER_QRY_H */
