/* include/texto.h
 *
 * TAD para representar textos.
 * Texto é definido por: id, posição (x, y), cores, âncora e conteúdo.
 */

#ifndef TEXTO_H
#define TEXTO_H

/* Tipo opaco para Texto */
typedef void* Texto;

/**
 * Cria um texto.
 * @param id Identificador único
 * @param x Coordenada x
 * @param y Coordenada y
 * @param corBorda Cor da borda (formato #RRGGBB)
 * @param corPreenchimento Cor de preenchimento (formato #RRGGBB)
 * @param ancora Ponto de ancoragem do texto: 'i' (início/start), 'm' (meio/middle), 'f' (fim/end)
 * @param conteudo String de texto (pode conter espaços)
 * @param fontFamily Família da fonte (sans-serif, serif, cursive)
 * @param fontWeight Peso da fonte (normal, bold, bolder, lighter)
 * @param fontSize Tamanho da fonte
 * @return Ponteiro para o texto criado, ou NULL em caso de erro
 */
Texto criaTexto(int id, double x, double y, const char *corBorda, 
                const char *corPreenchimento, const char *ancora, const char *conteudo,
                const char *fontFamily, const char *fontWeight, double fontSize);

/**
 * Cria uma cópia de um texto.
 */
Texto clonaTexto(Texto texto);

/**
 * Libera memória de um texto.
 */
void destroiTexto(Texto texto);

/**
 * Getters
 */
int getTextoId(Texto t);
double getTextoX(Texto t);
double getTextoY(Texto t);
const char* getTextoCorBorda(Texto t);
const char* getTextoCorPreenchimento(Texto t);
const char* getTextoAncora(Texto t);
const char* getTextoConteudo(Texto t);
const char* getTextoFontFamily(Texto t);
const char* getTextoFontWeight(Texto t);
double getTextoFontSize(Texto t);

/**
 * Setters
 */
void setTextoPosicao(Texto t, double x, double y);

/**
 * Define as cores do texto.
 * @param t Ponteiro para o texto
 * @param corBorda Nova cor de borda
 * @param corPreenchimento Nova cor de preenchimento
 */
void setTextoCores(Texto t, const char *corBorda, const char *corPreenchimento);

#endif /* TEXTO_H */
