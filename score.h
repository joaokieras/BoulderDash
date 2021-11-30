#include "game.h"

#ifndef __SCORE__
#define __SCORE__

struct pontos{
  int score[5];
  int tam;
};
typedef struct pontos pontos;

pontos* carrega_pontuacao(int tam);
void salva_pontuacao(int pontos_jogador, pontos* pontuacao);
void destroi_pontuacao(pontos* pontos);

#endif