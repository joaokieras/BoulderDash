// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro_font.h>
#include "game.h"
#include "mapa.h"
#include "sprites.h"

ALLEGRO_FONT* font;

int** inicia_mapa(char* nome_mapa){
  FILE *arq;
  int **mapa, i, j, lin, col;
  
  arq = fopen(nome_mapa, "r");
  if(!arq){
  	fprintf(stderr, "Erro ao abrir mapa!\n");
  	exit(1);
  }
  fscanf(arq, "%d %d", &lin, &col);
  mapa = malloc(lin * sizeof(int*));
  for(i = 0;i < lin;i++)
  	mapa[i] = malloc(col * sizeof(int));
  
  for(i = 0;i < lin;i++)
  	for(j = 0;j < col;j++)
  	  fscanf(arq, "%d", &mapa[i][j]);
  	  
  fclose(arq);
  return mapa;
}

int** inicia_mapa_anterior(){
  int i, j;
  int** mapa_anterior;

  mapa_anterior = malloc(22 * sizeof(int*));
  for(i = 0;i < 22;i++)
  	mapa_anterior[i] = malloc(40 * sizeof(int));

  for(i = 0;i < 22;i++)
  	for(j = 0;j < 40;j++)
  	  mapa_anterior[i][j] = 5;
 
  return mapa_anterior;
}

objetos* inicia_objetos(ALLEGRO_BITMAP* sheet){
  objetos *obj;
  obj = malloc(sizeof(objetos));
  if(obj == NULL){
    fprintf(stderr, "Erro ao alocar memória!\n");
    exit(1);
  }
  inicia_sprites_objetos(sheet, obj);
  obj->ciclos_diamante = 0;
  obj->ciclos_explosao = 0;
}

void draw_map(int** mapa, int** mapa_anterior, objetos* objetos_mapa, long frames){
  int i, j, i_aux, j_aux;
  for(i = 0;i < 22;i++){
  	for(j = 0;j < 40;j++){
  	  i_aux = i * SIZE_OBJS;
  	  j_aux = j * SIZE_OBJS;
  	  switch(mapa[i][j]){
  	  	case METAL:
  	  	  al_draw_scaled_bitmap(objetos_mapa->metal, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
  	  	  break;
  	  	case TERRA:
  	  	  al_draw_scaled_bitmap(objetos_mapa->terra, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
  	  	  break;
  	  	case MURO:
  	  	  al_draw_scaled_bitmap(objetos_mapa->muro, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
  	  	  break;
        case PEDRA:
          testa_desmoronamento(mapa, mapa_anterior, objetos_mapa, i, j, frames);
          al_draw_scaled_bitmap(objetos_mapa->pedra, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case VAZIO:
          al_draw_scaled_bitmap(objetos_mapa->vazio, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case DIAMANTE:
          if(objetos_mapa->ciclos_diamante == 7)
          	objetos_mapa->ciclos_diamante = 0;
          if(frames % 30 == 0)
            objetos_mapa->ciclos_diamante++;
          testa_desmoronamento(mapa, mapa_anterior,objetos_mapa, i, j, frames);
          al_draw_scaled_bitmap(objetos_mapa->diamante[objetos_mapa->ciclos_diamante], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case EXPLOSAO:
          if(objetos_mapa->ciclos_explosao == 3){
          	objetos_mapa->ciclos_explosao = 0;
          	mapa[i][j] = VAZIO;
          }
          if(frames % 30 == 0)
            objetos_mapa->ciclos_explosao++;
          al_draw_scaled_bitmap(objetos_mapa->explosao[objetos_mapa->ciclos_explosao], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
  	  }
  	}
  } 
}

void atualiza_mapa_anterior(int** mapa_original, int** mapa_anterior){
  int i, j;

  for(i = 0;i < 22;i++)
    for(j = 0;j < 40;j++)
      mapa_anterior[i][j] = mapa_original[i][j];
}

void testa_desmoronamento(int** mapa, int** mapa_anterior, objetos* objetos_mapa, int i, int j, long frames){
  if(frames % 5 != 0)
  	return;
  //Testa colisão com o personagem
  /*if(mapa[i + 1][j] == PLAYER){
  	if((mapa_anterior[i - 1][j] == PEDRA || mapa_anterior[i][j] == PEDRA) && mapa_anterior[i + 1][j] == PLAYER){
  	  mapa[i + 1][j] = PEDRA;
  	  mapa[i][j] = VAZIO;
  	}
  }*/
  //Testa rolamento para os lados
  //Testa se está no topo da pilha
  if((mapa[i + 1][j] == PEDRA || mapa[i + 1][j] == DIAMANTE) && (mapa[i - 1][j] != PEDRA || mapa[i - 1][j] != DIAMANTE)){
  	if(mapa[i][j + 1] == VAZIO && mapa[i + 1][j + 1] == VAZIO){
  	  if(mapa[i][j] == PEDRA)
  	    mapa[i][j + 1] = PEDRA;
  	  else if(mapa[i][j] == DIAMANTE)
  	    mapa[i][j + 1] = DIAMANTE;	 
  	  mapa[i][j] = VAZIO;
  	}
  	if(mapa[i][j - 1] == VAZIO && mapa[i + 1][j - 1] == VAZIO){
  	  if(mapa[i][j] == PEDRA)
  	    mapa[i][j - 1] = PEDRA;
  	  else if(mapa[i][j] == DIAMANTE)
  	    mapa[i][j - 1] = DIAMANTE;	 
  	  mapa[i][j] = VAZIO;
  	}
  }
  //Testa desabamento normal
  if(mapa[i + 1][j] == VAZIO){
  	if(mapa[i][j] == PEDRA)
  	  mapa[i + 1][j] = PEDRA;
  	else if(mapa[i][j] == DIAMANTE)
  	  mapa[i + 1][j] = DIAMANTE;
  	mapa[i][j] = VAZIO;
  }
}