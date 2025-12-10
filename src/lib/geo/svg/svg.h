/* svg.h
 *
 * TAD para geração de arquivos SVG.
 * Desenha formas geométricas em formato SVG.
 */

#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include "lista.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "formas.h"

/* ============================================================================
 * Tipo Opaco
 * ============================================================================ */

/**
 * Tipo opaco para o contexto SVG.
 * Mantém informações sobre o arquivo e dimensões.
 */
typedef void* SvgContexto;

/* ============================================================================
 * Funções de Criação e Destruição
 * ============================================================================ */

/**
 * Cria um novo contexto SVG e abre o arquivo para escrita.
 * 
 * @param caminho_arquivo Caminho do arquivo SVG a ser criado
 * @param largura Largura do canvas (viewBox)
 * @param altura Altura do canvas (viewBox)
 * @return Contexto SVG, ou NULL em caso de erro
 * 
 * @post O arquivo é criado e o header SVG é escrito
 */
SvgContexto criar_svg(const char *caminho_arquivo, double largura, double altura);

/**
 * Cria um contexto SVG com viewBox personalizado.
 * Útil quando se conhece o bounding box do cenário.
 * 
 * @param caminho_arquivo Caminho do arquivo SVG
 * @param min_x X mínimo do viewBox
 * @param min_y Y mínimo do viewBox
 * @param largura Largura do viewBox
 * @param altura Altura do viewBox
 * @return Contexto SVG, ou NULL em caso de erro
 */
SvgContexto criar_svg_viewbox(const char *caminho_arquivo,
                               double min_x, double min_y,
                               double largura, double altura);

/**
 * Finaliza o SVG e fecha o arquivo.
 * 
 * @param ctx Contexto SVG
 * @post O arquivo é fechado e o contexto é liberado
 */
void finalizar_svg(SvgContexto ctx);

/* ============================================================================
 * Funções de Desenho Individual
 * ============================================================================ */

/**
 * Desenha um círculo no SVG.
 */
void svg_desenhar_circulo(SvgContexto ctx, Circulo c);

/**
 * Desenha um retângulo no SVG.
 */
void svg_desenhar_retangulo(SvgContexto ctx, Retangulo r);

/**
 * Desenha uma linha no SVG.
 */
void svg_desenhar_linha(SvgContexto ctx, Linha l);

/**
 * Desenha um texto no SVG.
 */
void svg_desenhar_texto(SvgContexto ctx, Texto t);

/**
 * Desenha uma forma genérica (detecta o tipo automaticamente).
 */
void svg_desenhar_forma(SvgContexto ctx, Forma forma);

/* ============================================================================
 * Funções de Alto Nível
 * ============================================================================ */

/**
 * Desenha todas as formas de uma lista no SVG.
 * 
 * @param ctx Contexto SVG
 * @param lista Lista de formas a serem desenhadas
 */
void svg_desenhar_lista(SvgContexto ctx, Lista lista);

/**
 * Desenha um polígono (futuro: região de visibilidade).
 * 
 * @param ctx Contexto SVG
 * @param pontos Lista de pontos (alternando x, y)
 * @param num_pontos Número de pontos
 * @param cor_borda Cor da borda
 * @param cor_preenchimento Cor de preenchimento
 * @param opacidade Opacidade (0.0 a 1.0)
 */
void svg_desenhar_poligono(SvgContexto ctx, double *pontos, int num_pontos,
                           const char *cor_borda, const char *cor_preenchimento,
                           double opacidade);

/**
 * Adiciona um comentário ao SVG (útil para debug).
 */
void svg_comentario(SvgContexto ctx, const char *texto);

/* ============================================================================
 * Funções para Visibilidade
 * ============================================================================ */

/**
 * Desenha um segmento (anteparo) no SVG.
 * @param ctx Contexto SVG
 * @param seg Segmento a desenhar
 * @param cor Cor do segmento
 * @param largura Largura da linha
 */
void svg_desenhar_segmento(SvgContexto ctx, void *seg, 
                           const char *cor, double largura);

/**
 * Desenha uma lista de segmentos (anteparos) no SVG.
 * @param ctx Contexto SVG
 * @param lista Lista de segmentos
 */
void svg_desenhar_lista_segmentos(SvgContexto ctx, Lista lista);

/**
 * Desenha uma bomba (marcador de origem de visibilidade).
 * @param ctx Contexto SVG
 * @param x Coordenada X
 * @param y Coordenada Y
 * @param raio Raio do marcador
 * @param cor Cor do marcador
 */
void svg_desenhar_bomba(SvgContexto ctx, double x, double y, 
                        double raio, const char *cor);

/**
 * Desenha o polígono de visibilidade a partir de seus vértices.
 * @param ctx Contexto SVG
 * @param poligono Polígono de visibilidade (PoligonoVisibilidade)
 * @param cor_borda Cor da borda
 * @param cor_preenchimento Cor do preenchimento
 * @param opacidade Opacidade (0.0 a 1.0)
 */
void svg_desenhar_poligono_visibilidade(SvgContexto ctx, void *poligono,
                                         const char *cor_borda,
                                         const char *cor_preenchimento,
                                         double opacidade);

#endif /* SVG_H */
