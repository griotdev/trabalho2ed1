/* main.c
 *
 * Ponto de entrada do programa.
 * Trabalho 2 - Estrutura de Dados (Região de Visibilidade)
 *
 * Este é um main PROVISÓRIO para testar o fluxo básico do programa.
 * Conforme os módulos forem implementados, este arquivo será expandido.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Módulos do projeto */
#include "argumentos.h"
#include "lista.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "formas.h"

/* Tamanho máximo para caminhos de arquivo */
#define MAX_CAMINHO 512

/* ============================================================================
 * Funções Auxiliares
 * ============================================================================ */

/**
 * Verifica se um arquivo existe e pode ser lido.
 */
static int arquivo_existe(const char *caminho)
{
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo != NULL)
    {
        fclose(arquivo);
        return 1;
    }
    return 0;
}

/**
 * Extrai o nome base do arquivo (sem extensão).
 * Exemplo: "mapa.geo" -> "mapa"
 */
static void extrair_nome_base(const char *nome_arquivo, char *nome_base, int tamanho)
{
    if (nome_arquivo == NULL || nome_base == NULL)
    {
        return;
    }

    /* Copia o nome do arquivo */
    strncpy(nome_base, nome_arquivo, tamanho - 1);
    nome_base[tamanho - 1] = '\0';

    /* Remove a extensão */
    char *ponto = strrchr(nome_base, '.');
    if (ponto != NULL)
    {
        *ponto = '\0';
    }
}

/* ============================================================================
 * Função Principal
 * ============================================================================ */

int main(int argc, char *argv[])
{
    printf("=========================================\n");
    printf("  Trabalho 2 - Região de Visibilidade   \n");
    printf("  Estrutura de Dados - C99              \n");
    printf("=========================================\n\n");

    /* ========================================
     * 1. Processar argumentos de linha de comando
     * ======================================== */
    printf("[1] Processando argumentos...\n");
    
    Argumentos args = criar_argumentos(argc, argv);
    if (args == NULL)
    {
        fprintf(stderr, "Erro: falha ao processar argumentos.\n");
        exibir_uso(argv[0]);
        return 1;
    }

    if (!argumentos_validos(args))
    {
        exibir_uso(argv[0]);
        destruir_argumentos(args);
        return 1;
    }

    printf("    Diretório de entrada: %s\n", obter_diretorio_entrada(args));
    printf("    Arquivo .geo:         %s\n", obter_arquivo_geo(args));
    printf("    Diretório de saída:   %s\n", obter_diretorio_saida(args));
    printf("    Arquivo .qry:         %s\n", 
           obter_arquivo_qry(args) ? obter_arquivo_qry(args) : "(não especificado)");

    /* ========================================
     * 2. Construir caminhos e verificar arquivos
     * ======================================== */
    printf("\n[2] Verificando arquivos de entrada...\n");

    char caminho_geo[MAX_CAMINHO];
    char caminho_qry[MAX_CAMINHO];
    int tem_qry = 0;

    /* Caminho do arquivo .geo */
    if (!construir_caminho_geo(args, caminho_geo, MAX_CAMINHO))
    {
        fprintf(stderr, "Erro: não foi possível construir caminho do .geo\n");
        destruir_argumentos(args);
        return 1;
    }
    printf("    Caminho .geo: %s\n", caminho_geo);

    if (!arquivo_existe(caminho_geo))
    {
        fprintf(stderr, "Erro: arquivo .geo não encontrado: %s\n", caminho_geo);
        destruir_argumentos(args);
        return 1;
    }
    printf("    [OK] Arquivo .geo encontrado!\n");

    /* Caminho do arquivo .qry (opcional) */
    if (obter_arquivo_qry(args) != NULL)
    {
        if (construir_caminho_qry(args, caminho_qry, MAX_CAMINHO))
        {
            printf("    Caminho .qry: %s\n", caminho_qry);
            
            if (arquivo_existe(caminho_qry))
            {
                printf("    [OK] Arquivo .qry encontrado!\n");
                tem_qry = 1;
            }
            else
            {
                printf("    [AVISO] Arquivo .qry não encontrado, continuando sem consultas.\n");
            }
        }
    }
    else
    {
        printf("    [INFO] Nenhum arquivo .qry especificado.\n");
    }

    /* ========================================
     * 3. Extrair nome base para saídas
     * ======================================== */
    char nome_base[MAX_CAMINHO];
    extrair_nome_base(obter_arquivo_geo(args), nome_base, MAX_CAMINHO);
    printf("\n[3] Nome base para saídas: %s\n", nome_base);

    /* Construir caminho do SVG de saída */
    char caminho_svg[MAX_CAMINHO];
    snprintf(caminho_svg, MAX_CAMINHO, "%s/%s.svg", 
             obter_diretorio_saida(args), nome_base);
    printf("    SVG de saída: %s\n", caminho_svg);

    /* ========================================
     * 4. Criar lista de formas (provisório)
     * ======================================== */
    printf("\n[4] Inicializando estruturas de dados...\n");
    
    Lista lista_formas = criar_lista();
    if (lista_formas == NULL)
    {
        fprintf(stderr, "Erro: falha ao criar lista de formas.\n");
        destruir_argumentos(args);
        return 1;
    }
    printf("    [OK] Lista de formas criada.\n");

    /* ========================================
     * 5. TODO: Ler arquivo .geo
     * ======================================== */
    printf("\n[5] Leitura do arquivo .geo...\n");
    printf("    [TODO] Módulo de parser .geo ainda não implementado.\n");
    printf("    Arquivo a processar: %s\n", caminho_geo);

    /* 
     * Aqui entraria o código para:
     * - Abrir o arquivo .geo
     * - Ler cada linha e interpretar o comando (c, r, l, t, ts)
     * - Criar as formas correspondentes
     * - Inserir na lista de formas
     */

    /* ========================================
     * 6. TODO: Gerar SVG inicial
     * ======================================== */
    printf("\n[6] Geração do SVG inicial...\n");
    printf("    [TODO] Módulo SVG ainda não implementado.\n");
    printf("    Arquivo a gerar: %s\n", caminho_svg);

    /* ========================================
     * 7. TODO: Processar arquivo .qry (se existir)
     * ======================================== */
    if (tem_qry)
    {
        printf("\n[7] Processamento de consultas (.qry)...\n");
        printf("    [TODO] Módulo de parser .qry ainda não implementado.\n");
        printf("    Arquivo a processar: %s\n", caminho_qry);

        /*
         * Aqui entraria o código para:
         * - Ler cada comando do .qry (a, d, P, cln)
         * - Para comandos de bomba: executar algoritmo de visibilidade
         * - Gerar relatórios e SVGs conforme necessário
         */
    }
    else
    {
        printf("\n[7] Sem arquivo .qry - pulando processamento de consultas.\n");
    }

    /* ========================================
     * 8. Limpeza
     * ======================================== */
    printf("\n[8] Finalizando...\n");
    
    /* Destruir lista de formas (quando tiver elementos, usar função de destruição apropriada) */
    destruir_lista(lista_formas, NULL);
    printf("    [OK] Lista de formas liberada.\n");

    destruir_argumentos(args);
    printf("    [OK] Argumentos liberados.\n");

    printf("\n=========================================\n");
    printf("  Execução concluída com sucesso!       \n");
    printf("=========================================\n");

    return 0;
}
