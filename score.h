// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR 
#include "game.h"

#ifndef __SCORE__
#define __SCORE__

struct pontos{
  int score[SIZE_ARQ_PONTOS];
  int tam;
};
typedef struct pontos pontos;

pontos* carrega_pontuacao();
void salva_pontuacao(int pontos_jogador, pontos* pontuacao);
void destroi_pontuacao(pontos* pontos);

#endif