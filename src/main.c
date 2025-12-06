/* main.c
 *
 * Ponto de entrada do programa.
 * Trabalho 2 - Estrutura de Dados (Região de Visibilidade)
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
#include "parser_geo.h"
#include "svg.h"

/* Tamanho máximo para caminhos de arquivo */
#define MAX_CAMINHO 1024

/* Margem extra no viewBox do SVG */
#define MARGEM_SVG 10.0

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
 * Extrai o nome base do arquivo (sem diretório e sem extensão).
 * Exemplo: "../testes/mapa.geo" -> "mapa"
 */
static void extrair_nome_base(const char *caminho_arquivo, char *nome_base, int tamanho)
{
    if (caminho_arquivo == NULL || nome_base == NULL)
    {
        return;
    }

    /* Encontra o último separador de diretório */
    const char *nome = strrchr(caminho_arquivo, '/');
    if (nome != NULL)
    {
        nome++; /* Pula o '/' */
    }
    else
    {
        nome = caminho_arquivo; /* Não tem diretório */
    }

    /* Copia o nome do arquivo */
    strncpy(nome_base, nome, tamanho - 1);
    nome_base[tamanho - 1] = '\0';

    /* Remove a extensão */
    char *ponto = strrchr(nome_base, '.');
    if (ponto != NULL)
    {
        *ponto = '\0';
    }
}

/**
 * Função para destruir uma forma (usada na destruição da lista).
 */
static void destruir_forma_callback(void *elemento)
{
    Forma forma = (Forma)elemento;
    if (forma != NULL)
    {
        destroiForma(forma);
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
     * 4. Criar lista de formas
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
     * 5. Ler arquivo .geo
     * ======================================== */
    printf("\n[5] Leitura do arquivo .geo...\n");
    
    int formas_lidas = ler_arquivo_geo(caminho_geo, lista_formas);
    if (formas_lidas < 0)
    {
        fprintf(stderr, "Erro: falha ao ler arquivo .geo\n");
        destruir_lista(lista_formas, destruir_forma_callback);
        destruir_argumentos(args);
        return 1;
    }
    
    printf("    Total de formas na lista: %d\n", obter_tamanho(lista_formas));

    /* ========================================
     * 6. Calcular dimensões do cenário
     * ======================================== */
    printf("\n[6] Calculando dimensões do cenário...\n");
    
    double min_x, min_y, max_x, max_y;
    if (!obter_dimensoes_cenario(lista_formas, &min_x, &min_y, &max_x, &max_y))
    {
        fprintf(stderr, "Aviso: não foi possível calcular dimensões (lista vazia?)\n");
        min_x = 0; min_y = 0;
        max_x = 800; max_y = 600;
    }
    
    /* Adiciona margem */
    min_x -= MARGEM_SVG;
    min_y -= MARGEM_SVG;
    double largura = (max_x - min_x) + 2 * MARGEM_SVG;
    double altura = (max_y - min_y) + 2 * MARGEM_SVG;
    
    printf("    Bounding Box: (%.2f, %.2f) até (%.2f, %.2f)\n", 
           min_x + MARGEM_SVG, min_y + MARGEM_SVG, max_x, max_y);
    printf("    ViewBox: %.2f %.2f %.2f %.2f\n", min_x, min_y, largura, altura);

    /* ========================================
     * 7. Gerar SVG inicial
     * ======================================== */
    printf("\n[7] Gerando SVG inicial...\n");
    
    SvgContexto svg = criar_svg_viewbox(caminho_svg, min_x, min_y, largura, altura);
    if (svg == NULL)
    {
        fprintf(stderr, "Erro: falha ao criar arquivo SVG\n");
        destruir_lista(lista_formas, destruir_forma_callback);
        destruir_argumentos(args);
        return 1;
    }
    
    svg_desenhar_lista(svg, lista_formas);
    finalizar_svg(svg);
    
    printf("    [OK] SVG gerado: %s\n", caminho_svg);

    /* ========================================
     * 8. Processar arquivo .qry (se existir)
     * ======================================== */
    if (tem_qry)
    {
        printf("\n[8] Processamento de consultas (.qry)...\n");
        printf("    [TODO] Módulo de parser .qry ainda não implementado.\n");
        printf("    Arquivo a processar: %s\n", caminho_qry);

        /*
         * Aqui entraria o código para:
         * - Ler cada comando do .qry (a, d, P, cln)
         * - Para comandos de bomba: executar algoritmo de visibilidade
         * - Gerar relatórios e SVGs conforme necessário
         */
        
        /* Nome do SVG com consultas */
        char caminho_svg_qry[MAX_CAMINHO];
        snprintf(caminho_svg_qry, MAX_CAMINHO, "%s/%s-consultas.svg", 
                 obter_diretorio_saida(args), nome_base);
        printf("    SVG de consultas (futuro): %s\n", caminho_svg_qry);
    }
    else
    {
        printf("\n[8] Sem arquivo .qry - pulando processamento de consultas.\n");
    }

    /* ========================================
     * 9. Limpeza
     * ======================================== */
    printf("\n[9] Finalizando...\n");
    
    destruir_lista(lista_formas, destruir_forma_callback);
    printf("    [OK] Lista de formas liberada.\n");

    destruir_argumentos(args);
    printf("    [OK] Argumentos liberados.\n");

    printf("\n=========================================\n");
    printf("  Execução concluída com sucesso!       \n");
    printf("=========================================\n");

    return 0;
}
