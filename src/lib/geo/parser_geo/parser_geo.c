/* parser_geo.c
 *
 * Implementação do parser de arquivos .geo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser_geo.h"
#include "lista.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "formas.h"

/* Tamanho máximo de uma linha do arquivo */
#define MAX_LINHA 1024

/* ============================================================================
 * Estado Global de Estilo de Texto
 * ============================================================================ */

static char estilo_font_family[64] = "sans-serif";
static char estilo_font_weight[32] = "normal";
static double estilo_font_size = 12.0;

/* ============================================================================
 * Funções Auxiliares de Parsing
 * ============================================================================ */

/**
 * Remove espaços em branco do início e fim da string.
 */
static char* trim(char *str)
{
    if (str == NULL) return NULL;
    
    /* Remove espaços do início */
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == '\0') return str;
    
    /* Remove espaços do fim */
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    
    return str;
}

/**
 * Processa comando 'c' (círculo).
 * Formato: c id x y raio corBorda corPreenchimento
 */
static Forma* processar_circulo(char *linha)
{
    int id;
    double x, y, raio;
    char corBorda[32], corPreenchimento[32];
    
    int lidos = sscanf(linha, "c %d %lf %lf %lf %31s %31s",
                       &id, &x, &y, &raio, corBorda, corPreenchimento);
    
    if (lidos != 6)
    {
        fprintf(stderr, "Erro: formato inválido para círculo: %s\n", linha);
        return NULL;
    }
    
    Circulo c = criaCirculo(id, x, y, raio, corBorda, corPreenchimento);
    if (c == NULL) return NULL;
    
    return criaForma(TIPO_CIRCULO, c);
}

/**
 * Processa comando 'r' (retângulo).
 * Formato: r id x y largura altura corBorda corPreenchimento
 */
static Forma* processar_retangulo(char *linha)
{
    int id;
    double x, y, largura, altura;
    char corBorda[32], corPreenchimento[32];
    
    int lidos = sscanf(linha, "r %d %lf %lf %lf %lf %31s %31s",
                       &id, &x, &y, &largura, &altura, corBorda, corPreenchimento);
    
    if (lidos != 7)
    {
        fprintf(stderr, "Erro: formato inválido para retângulo: %s\n", linha);
        return NULL;
    }
    
    Retangulo r = criaRetangulo(id, x, y, largura, altura, corBorda, corPreenchimento);
    if (r == NULL) return NULL;
    
    return criaForma(TIPO_RETANGULO, r);
}

/**
 * Processa comando 'l' (linha).
 * Formato: l id x1 y1 x2 y2 cor
 */
static Forma* processar_linha(char *linha_texto)
{
    int id;
    double x1, y1, x2, y2;
    char cor[32];
    
    int lidos = sscanf(linha_texto, "l %d %lf %lf %lf %lf %31s",
                       &id, &x1, &y1, &x2, &y2, cor);
    
    if (lidos != 6)
    {
        fprintf(stderr, "Erro: formato inválido para linha: %s\n", linha_texto);
        return NULL;
    }
    
    Linha l = criaLinha(id, x1, y1, x2, y2, cor);
    if (l == NULL) return NULL;
    
    return criaForma(TIPO_LINHA, l);
}

/**
 * Processa comando 't' (texto).
 * Formato: t id x y corBorda corPreenchimento ancora texto...
 * Nota: o texto pode conter espaços e vai até o final da linha
 */
static Forma* processar_texto(char *linha)
{
    int id;
    double x, y;
    char corBorda[32], corPreenchimento[32];
    char ancora;
    char texto[MAX_LINHA];
    
    /* Primeiro, lemos os campos fixos */
    int lidos = sscanf(linha, "t %d %lf %lf %31s %31s %c",
                       &id, &x, &y, corBorda, corPreenchimento, &ancora);
    
    if (lidos != 6)
    {
        fprintf(stderr, "Erro: formato inválido para texto: %s\n", linha);
        return NULL;
    }
    
    /* Agora precisamos encontrar o texto após a âncora */
    /* Pular: "t id x y corBorda corPreenchimento ancora " */
    char *ptr = linha + 2; /* Pula "t " */
    
    /* Pula id */
    while (*ptr && !isspace((unsigned char)*ptr)) ptr++;
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    
    /* Pula x */
    while (*ptr && !isspace((unsigned char)*ptr)) ptr++;
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    
    /* Pula y */
    while (*ptr && !isspace((unsigned char)*ptr)) ptr++;
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    
    /* Pula corBorda */
    while (*ptr && !isspace((unsigned char)*ptr)) ptr++;
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    
    /* Pula corPreenchimento */
    while (*ptr && !isspace((unsigned char)*ptr)) ptr++;
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    
    /* Pula ancora (1 caractere) */
    ptr++;
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    
    /* O resto é o texto */
    strncpy(texto, ptr, MAX_LINHA - 1);
    texto[MAX_LINHA - 1] = '\0';
    trim(texto);
    
    /* Converte ancora para string */
    char ancora_str[2] = {ancora, '\0'};
    
    Texto t = criaTexto(id, x, y, corBorda, corPreenchimento, ancora_str, texto,
                        estilo_font_family, estilo_font_weight, estilo_font_size);
    if (t == NULL) return NULL;
    
    return criaForma(TIPO_TEXTO, t);
}

/**
 * Processa comando 'ts' (estilo de texto).
 * Formato: ts fontFamily fontWeight fontSize
 * Este comando altera o estado global e não cria forma.
 */
static void processar_estilo_texto(char *linha)
{
    char family[64], weight[32], size_str[32];
    
    int lidos = sscanf(linha, "ts %63s %31s %31s", family, weight, size_str);
    
    if (lidos >= 1)
    {
        strncpy(estilo_font_family, family, sizeof(estilo_font_family) - 1);
    }
    if (lidos >= 2)
    {
        strncpy(estilo_font_weight, weight, sizeof(estilo_font_weight) - 1);
    }
    if (lidos >= 3)
    {
        /* Remove 'pt' ou 'px' se presente */
        double size = atof(size_str);
        if (size > 0)
        {
            estilo_font_size = size;
        }
    }
}

/* ============================================================================
 * Implementação das Funções Públicas
 * ============================================================================ */

int ler_arquivo_geo(const char *caminho_arquivo, Lista lista_formas)
{
    if (caminho_arquivo == NULL || lista_formas == NULL)
    {
        fprintf(stderr, "Erro: parâmetros inválidos em ler_arquivo_geo\n");
        return -1;
    }
    
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro: não foi possível abrir arquivo: %s\n", caminho_arquivo);
        return -1;
    }
    
    char linha[MAX_LINHA];
    int formas_lidas = 0;
    int numero_linha = 0;
    
    while (fgets(linha, MAX_LINHA, arquivo) != NULL)
    {
        numero_linha++;
        char *linha_limpa = trim(linha);
        
        /* Ignora linhas vazias e comentários */
        if (linha_limpa[0] == '\0' || linha_limpa[0] == '#')
        {
            continue;
        }
        
        Forma *forma = NULL;
        char comando = linha_limpa[0];
        
        switch (comando)
        {
            case 'c':
                forma = processar_circulo(linha_limpa);
                break;
                
            case 'r':
                forma = processar_retangulo(linha_limpa);
                break;
                
            case 'l':
                forma = processar_linha(linha_limpa);
                break;
                
            case 't':
                if (linha_limpa[1] == 's')
                {
                    /* Comando ts (estilo de texto) */
                    processar_estilo_texto(linha_limpa);
                }
                else
                {
                    /* Comando t (texto) */
                    forma = processar_texto(linha_limpa);
                }
                break;
                
            default:
                fprintf(stderr, "Aviso: comando desconhecido na linha %d: %c\n", 
                        numero_linha, comando);
                break;
        }
        
        if (forma != NULL)
        {
            inserir_fim(lista_formas, forma);
            formas_lidas++;
        }
    }
    
    fclose(arquivo);
    
    printf("    Arquivo .geo processado: %d formas lidas.\n", formas_lidas);
    return formas_lidas;
}

int obter_dimensoes_cenario(Lista lista_formas,
                            double *min_x, double *min_y,
                            double *max_x, double *max_y)
{
    if (lista_formas == NULL || lista_vazia(lista_formas))
    {
        return 0;
    }
    
    int primeira = 1;
    double minX = 0, minY = 0, maxX = 0, maxY = 0;
    
    No atual = obter_primeiro(lista_formas);
    while (atual != NULL)
    {
        Forma *forma = (Forma*)obter_elemento(atual);
        TipoForma tipo = getFormaTipo(forma);
        void *dados = getFormaDados(forma);
        
        double fx1, fy1, fx2, fy2;
        
        switch (tipo)
        {
            case TIPO_CIRCULO:
            {
                Circulo c = (Circulo)dados;
                double cx = getCirculoX(c);
                double cy = getCirculoY(c);
                double r = getCirculoRaio(c);
                fx1 = cx - r;
                fy1 = cy - r;
                fx2 = cx + r;
                fy2 = cy + r;
                break;
            }
            
            case TIPO_RETANGULO:
            {
                Retangulo r = (Retangulo)dados;
                fx1 = getRetanguloX(r);
                fy1 = getRetanguloY(r);
                fx2 = fx1 + getRetanguloLargura(r);
                fy2 = fy1 + getRetanguloAltura(r);
                break;
            }
            
            case TIPO_LINHA:
            {
                Linha l = (Linha)dados;
                fx1 = getLinhaX1(l);
                fy1 = getLinhaY1(l);
                fx2 = getLinhaX2(l);
                fy2 = getLinhaY2(l);
                /* Garante que fx1 <= fx2 e fy1 <= fy2 */
                if (fx1 > fx2) { double t = fx1; fx1 = fx2; fx2 = t; }
                if (fy1 > fy2) { double t = fy1; fy1 = fy2; fy2 = t; }
                break;
            }
            
            case TIPO_TEXTO:
            {
                Texto t = (Texto)dados;
                fx1 = getTextoX(t);
                fy1 = getTextoY(t);
                /* Aproximação do bounding box do texto */
                fx2 = fx1 + strlen(getTextoConteudo(t)) * 10;
                fy2 = fy1 + getTextoFontSize(t);
                break;
            }
            
            default:
                atual = obter_proximo(atual);
                continue;
        }
        
        if (primeira)
        {
            minX = fx1;
            minY = fy1;
            maxX = fx2;
            maxY = fy2;
            primeira = 0;
        }
        else
        {
            if (fx1 < minX) minX = fx1;
            if (fy1 < minY) minY = fy1;
            if (fx2 > maxX) maxX = fx2;
            if (fy2 > maxY) maxY = fy2;
        }
        
        atual = obter_proximo(atual);
    }
    
    if (min_x) *min_x = minX;
    if (min_y) *min_y = minY;
    if (max_x) *max_x = maxX;
    if (max_y) *max_y = maxY;
    
    return 1;
}
