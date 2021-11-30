#include "score.h"
#include <stdio.h>

pontos* carrega_pontuacao(){
  FILE* arq;
  pontos* pontuacao;

  pontuacao = malloc(sizeof(pontos));
  pontuacao->tam = SIZE_ARQ_PONTOS;
  //printf("entrou\n");
  //pontuacao->tam = 0;
  arq = fopen(PATH_SCORE, "r");
  if(!arq){
  	fprintf(stderr, "Não foi possivel inicializar o arquivo score\n");
    exit(1);
  }
  //printf("ola\n");
  for(int i = 0;i < SIZE_ARQ_PONTOS && !feof(arq);i++)
  	fscanf(arq, "%d", &pontuacao->score[i]);

  fclose(arq);
  return pontuacao;
}

void salva_pontuacao(int pontos_jogador, pontos* pontuacao){
  FILE* arq;

  arq = fopen(PATH_SCORE, "w");
  if(!arq){
  	fprintf(stderr, "Não foi possivel inicializar o arquivo score\n");
    exit(1);
  }
  int i = pontuacao->tam - 1;
  while(pontos_jogador > pontuacao->score[i - 1] && i > 0)
  {
    i--;
    pontuacao->score[i + 1] = pontuacao->score[i];
  }
  int pos = i;
  // insere score atual na sua posição devida
  pontuacao->score[pos] = pontos_jogador;

  fprintf(arq, "%d", pontuacao->score[0]);
  for(int i = 1; i < pontuacao->tam; i++)
    fprintf(arq, "\n%d", pontuacao->score[i]);

  fclose(arq);
}

void destroi_pontuacao(pontos* pontos){
  free(pontos);
}