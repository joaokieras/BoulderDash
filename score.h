#include "game.h"

#ifndef __SCORE__
#define __SCORE__

struct pontos{
  int score[5];
  int tam;
};
typedef struct pontos pontos;

void carrega_pontuacao(int* pontuacao, int tam);
void salva_pontuacao(int pontos, int* pontuacao, int tam);

#endif