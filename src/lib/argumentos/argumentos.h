/* argumentos.h
 *
 * TAD para tratamento de argumentos de linha de comando.
 * Processa as flags do programa: -e (diretório entrada), -f (arquivo geo),
 * -o (diretório saída), -q (arquivo qry).
 */

#ifndef ARGUMENTOS_H
#define ARGUMENTOS_H

/* ============================================================================
 * Tipo Opaco
 * ============================================================================ */

/**
 * Tipo opaco para os argumentos do programa.
 * A estrutura interna é definida apenas no arquivo .c
 */
typedef void* Argumentos;

/* ============================================================================
 * Funções de Criação e Destruição
 * ============================================================================ */

/**
 * Processa os argumentos da linha de comando e cria a estrutura de argumentos.
 * 
 * Flags suportadas:
 *   -e <diretório>   Diretório base de entrada (onde estão os arquivos .geo e .qry)
 *   -f <arquivo>     Nome do arquivo .geo (sem o diretório)
 *   -o <diretório>   Diretório de saída (onde serão gerados os .svg e .txt)
 *   -q <arquivo>     Nome do arquivo .qry (opcional, sem o diretório)
 *   -oa <algoritmo>  Algoritmo de ordenação: qsort ou mergesort (opcional)
 * 
 * @param argc Número de argumentos (recebido do main)
 * @param argv Vetor de strings com os argumentos (recebido do main)
 * @return Ponteiro para estrutura Argumentos, ou NULL se houver erro
 * 
 * @pre argc >= 1
 * @pre argv != NULL
 * 
 * Exemplo de uso:
 *   ./t2 -e ./dados -f mapa.geo -o ./saida -q consulta.qry
 */
Argumentos criar_argumentos(int argc, char *argv[]);

/**
 * Libera a memória alocada para a estrutura de argumentos.
 * 
 * @param args Ponteiro para a estrutura Argumentos
 * @post A memória é liberada e o ponteiro não deve mais ser usado
 */
void destruir_argumentos(Argumentos args);

/* ============================================================================
 * Getters - Acesso aos valores dos argumentos
 * ============================================================================ */

/**
 * Obtém o diretório de entrada (-e).
 * 
 * @param args Ponteiro para a estrutura Argumentos
 * @return String com o caminho do diretório, ou NULL se não definido
 */
const char* obter_diretorio_entrada(Argumentos args);

/**
 * Obtém o nome do arquivo .geo (-f).
 * 
 * @param args Ponteiro para a estrutura Argumentos
 * @return String com o nome do arquivo, ou NULL se não definido
 */
const char* obter_arquivo_geo(Argumentos args);

/**
 * Obtém o diretório de saída (-o).
 * 
 * @param args Ponteiro para a estrutura Argumentos
 * @return String com o caminho do diretório, ou NULL se não definido
 */
const char* obter_diretorio_saida(Argumentos args);

/**
 * Obtém o nome do arquivo .qry (-q).
 * 
 * @param args Ponteiro para a estrutura Argumentos
 * @return String com o nome do arquivo, ou NULL se não houver consulta
 */
const char* obter_arquivo_qry(Argumentos args);

/**
 * Obtém o algoritmo de ordenação (-oa).
 * 
 * @param args Ponteiro para a estrutura Argumentos
 * @return String com o nome do algoritmo (default: "qsort")
 */
const char* obter_algoritmo_ordenacao(Argumentos args);

/* ============================================================================
 * Funções Auxiliares
 * ============================================================================ */

/**
 * Verifica se os argumentos obrigatórios foram fornecidos.
 * Os argumentos obrigatórios são: -e, -f, -o
 * 
 * @param args Ponteiro para a estrutura Argumentos
 * @return 1 se todos os obrigatórios estão presentes, 0 caso contrário
 */
int argumentos_validos(Argumentos args);

/**
 * Exibe mensagem de uso do programa (help).
 * Útil para quando o usuário não fornece os argumentos corretamente.
 * 
 * @param nome_programa Nome do executável (argv[0])
 */
void exibir_uso(const char *nome_programa);

/**
 * Constrói o caminho completo do arquivo .geo.
 * Concatena o diretório de entrada com o nome do arquivo.
 * 
 * @param args Ponteiro para a estrutura Argumentos
 * @param buffer Buffer onde o caminho será escrito
 * @param tamanho_buffer Tamanho máximo do buffer
 * @return 1 se sucesso, 0 se erro (buffer pequeno ou args inválido)
 */
int construir_caminho_geo(Argumentos args, char *buffer, int tamanho_buffer);

/**
 * Constrói o caminho completo do arquivo .qry.
 * Concatena o diretório de entrada com o nome do arquivo.
 * 
 * @param args Ponteiro para a estrutura Argumentos
 * @param buffer Buffer onde o caminho será escrito
 * @param tamanho_buffer Tamanho máximo do buffer
 * @return 1 se sucesso, 0 se erro (sem qry, buffer pequeno ou args inválido)
 */
int construir_caminho_qry(Argumentos args, char *buffer, int tamanho_buffer);

#endif /* ARGUMENTOS_H */
