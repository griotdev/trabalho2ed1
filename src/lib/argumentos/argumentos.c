/* argumentos.c
 *
 * Implementação do TAD para tratamento de argumentos de linha de comando.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argumentos.h"

/* ============================================================================
 * Estrutura Interna (Ponteiro Opaco)
 * ============================================================================ */

typedef struct argumentos_internal
{
    char *diretorio_entrada;  /* -e: diretório base de entrada */
    char *arquivo_geo;        /* -f: nome do arquivo .geo */
    char *diretorio_saida;    /* -o: diretório de saída */
    char *arquivo_qry;        /* -q: nome do arquivo .qry (opcional) */
} ArgumentosInternal;

/* ============================================================================
 * Funções Auxiliares Internas
 * ============================================================================ */

/**
 * Duplica uma string alocando memória.
 */
static char* duplicar_string(const char *str)
{
    if (str == NULL)
    {
        return NULL;
    }
    
    size_t len = strlen(str);
    char *copia = (char*)malloc(len + 1);
    if (copia == NULL)
    {
        return NULL;
    }
    
    strcpy(copia, str);
    return copia;
}

/* ============================================================================
 * Implementação das Funções Públicas
 * ============================================================================ */

Argumentos criar_argumentos(int argc, char *argv[])
{
    if (argc < 2 || argv == NULL)
    {
        return NULL;
    }

    ArgumentosInternal *args = (ArgumentosInternal*)malloc(sizeof(ArgumentosInternal));
    if (args == NULL)
    {
        fprintf(stderr, "Erro: falha ao alocar memória para argumentos.\n");
        return NULL;
    }

    /* Inicializa todos os campos como NULL */
    args->diretorio_entrada = NULL;
    args->arquivo_geo = NULL;
    args->diretorio_saida = NULL;
    args->arquivo_qry = NULL;

    /* Processa os argumentos */
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc)
        {
            args->diretorio_entrada = duplicar_string(argv[++i]);
        }
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
        {
            args->arquivo_geo = duplicar_string(argv[++i]);
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
        {
            args->diretorio_saida = duplicar_string(argv[++i]);
        }
        else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc)
        {
            args->arquivo_qry = duplicar_string(argv[++i]);
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            exibir_uso(argv[0]);
            destruir_argumentos((Argumentos)args);
            return NULL;
        }
    }

    return (Argumentos)args;
}

void destruir_argumentos(Argumentos argumentos)
{
    ArgumentosInternal *args = (ArgumentosInternal*)argumentos;
    if (args == NULL)
    {
        return;
    }

    free(args->diretorio_entrada);
    free(args->arquivo_geo);
    free(args->diretorio_saida);
    free(args->arquivo_qry);
    free(args);
}

const char* obter_diretorio_entrada(Argumentos argumentos)
{
    ArgumentosInternal *args = (ArgumentosInternal*)argumentos;
    return args ? args->diretorio_entrada : NULL;
}

const char* obter_arquivo_geo(Argumentos argumentos)
{
    ArgumentosInternal *args = (ArgumentosInternal*)argumentos;
    return args ? args->arquivo_geo : NULL;
}

const char* obter_diretorio_saida(Argumentos argumentos)
{
    ArgumentosInternal *args = (ArgumentosInternal*)argumentos;
    return args ? args->diretorio_saida : NULL;
}

const char* obter_arquivo_qry(Argumentos argumentos)
{
    ArgumentosInternal *args = (ArgumentosInternal*)argumentos;
    return args ? args->arquivo_qry : NULL;
}

int argumentos_validos(Argumentos argumentos)
{
    ArgumentosInternal *args = (ArgumentosInternal*)argumentos;
    if (args == NULL)
    {
        return 0;
    }

    /* Verifica se os argumentos obrigatórios foram fornecidos */
    if (args->diretorio_entrada == NULL)
    {
        fprintf(stderr, "Erro: diretório de entrada (-e) não especificado.\n");
        return 0;
    }
    if (args->arquivo_geo == NULL)
    {
        fprintf(stderr, "Erro: arquivo .geo (-f) não especificado.\n");
        return 0;
    }
    if (args->diretorio_saida == NULL)
    {
        fprintf(stderr, "Erro: diretório de saída (-o) não especificado.\n");
        return 0;
    }

    return 1;
}

void exibir_uso(const char *nome_programa)
{
    printf("Uso: %s -e <dir_entrada> -f <arquivo.geo> -o <dir_saida> [-q <arquivo.qry>]\n\n", 
           nome_programa ? nome_programa : "t2");
    printf("Opções:\n");
    printf("  -e <diretório>   Diretório base de entrada (arquivos .geo e .qry)\n");
    printf("  -f <arquivo>     Nome do arquivo de geometria (.geo)\n");
    printf("  -o <diretório>   Diretório de saída (arquivos .svg e .txt)\n");
    printf("  -q <arquivo>     Nome do arquivo de consultas (.qry) [opcional]\n");
    printf("  -h, --help       Exibe esta mensagem de ajuda\n\n");
    printf("Exemplo:\n");
    printf("  %s -e ./dados -f mapa.geo -o ./saida -q consulta.qry\n",
           nome_programa ? nome_programa : "t2");
}

int construir_caminho_geo(Argumentos argumentos, char *buffer, int tamanho_buffer)
{
    ArgumentosInternal *args = (ArgumentosInternal*)argumentos;
    if (args == NULL || buffer == NULL || tamanho_buffer <= 0)
    {
        return 0;
    }
    if (args->diretorio_entrada == NULL || args->arquivo_geo == NULL)
    {
        return 0;
    }

    int necessario = snprintf(buffer, tamanho_buffer, "%s/%s", 
                              args->diretorio_entrada, args->arquivo_geo);
    
    if (necessario >= tamanho_buffer)
    {
        fprintf(stderr, "Erro: buffer muito pequeno para o caminho do arquivo .geo\n");
        return 0;
    }

    return 1;
}

int construir_caminho_qry(Argumentos argumentos, char *buffer, int tamanho_buffer)
{
    ArgumentosInternal *args = (ArgumentosInternal*)argumentos;
    if (args == NULL || buffer == NULL || tamanho_buffer <= 0)
    {
        return 0;
    }
    if (args->diretorio_entrada == NULL || args->arquivo_qry == NULL)
    {
        return 0;
    }

    int necessario = snprintf(buffer, tamanho_buffer, "%s/%s", 
                              args->diretorio_entrada, args->arquivo_qry);
    
    if (necessario >= tamanho_buffer)
    {
        fprintf(stderr, "Erro: buffer muito pequeno para o caminho do arquivo .qry\n");
        return 0;
    }

    return 1;
}

/* ============================================================================
 * Main de Teste (compilar com -DTESTE_ARGUMENTOS)
 * ============================================================================ */

#ifdef TESTE_ARGUMENTOS

int main(int argc, char *argv[])
{
    printf("=== Teste do Módulo Argumentos ===\n\n");

    if (argc < 2)
    {
        printf("Uso para teste: %s -e <dir> -f <arquivo.geo> -o <dir_saida> [-q <arquivo.qry>]\n", argv[0]);
        printf("\nTestando com argumentos simulados...\n\n");
        
        /* Simula argumentos para teste */
        char *args_teste[] = {"t2", "-e", "./entrada", "-f", "mapa.geo", "-o", "./saida", "-q", "consulta.qry"};
        argc = 9;
        argv = args_teste;
    }

    Argumentos args = criar_argumentos(argc, argv);
    
    if (args == NULL)
    {
        printf("Falha ao criar argumentos.\n");
        return 1;
    }

    printf("Argumentos processados:\n");
    printf("  Diretório entrada: %s\n", obter_diretorio_entrada(args) ? obter_diretorio_entrada(args) : "(não definido)");
    printf("  Arquivo .geo:      %s\n", obter_arquivo_geo(args) ? obter_arquivo_geo(args) : "(não definido)");
    printf("  Diretório saída:   %s\n", obter_diretorio_saida(args) ? obter_diretorio_saida(args) : "(não definido)");
    printf("  Arquivo .qry:      %s\n", obter_arquivo_qry(args) ? obter_arquivo_qry(args) : "(não definido)");

    printf("\nArgumentos válidos: %s\n", argumentos_validos(args) ? "SIM" : "NÃO");

    char caminho[256];
    if (construir_caminho_geo(args, caminho, sizeof(caminho)))
    {
        printf("Caminho .geo: %s\n", caminho);
    }
    if (construir_caminho_qry(args, caminho, sizeof(caminho)))
    {
        printf("Caminho .qry: %s\n", caminho);
    }

    destruir_argumentos(args);
    printf("\n=== Teste concluído com sucesso! ===\n");
    
    return 0;
}

#endif /* TESTE_ARGUMENTOS */
