#include "score.h"
#include <stdio.h>

void carrega_pontuacao(int* pontuacao, int tam){
  FILE* arq;
  //printf("entrou\n");
  //pontuacao->tam = 0;
  arq = fopen(PATH_SCORE, "r");
  if(!arq){
  	fprintf(stderr, "Não foi possivel inicializar o arquivo score\n");
    exit(1);
  }
  //printf("ola\n");
  for(int i = 0;i < tam && !feof(arq);i++)
  	fscanf(arq, "%d", &pontuacao[i]);

  fclose(arq);
}

void salva_pontuacao(int pontos, int* pontuacao, int tam){
  FILE* arq;

  arq = fopen(PATH_SCORE, "w");
  if(!arq){
  	fprintf(stderr, "Não foi possivel inicializar o arquivo score\n");
    exit(1);
  }
  int i = tam;
  while(pontos > pontuacao[i - 1] && i > 0)
  {
    i--;
    pontuacao[i + 1] = pontuacao[i];
  }
  int pos = i;
  // insere score atual na sua posição devida
  pontuacao[pos] = pontos;

  fprintf(arq, "%d", pontuacao[0]);
    // escreve próximos lugares
  for(int i = 1; i < tam; i++)
    fprintf(arq, "\n%d", pontuacao[i]);

  fclose(arq);
}