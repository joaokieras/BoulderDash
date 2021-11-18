// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro_font.h>
#include "game.h"
#include "mapa.h"
#include "sprites.h"

ALLEGRO_FONT* font;

int** inicia_mapa(char* nome_mapa, objetos* obj){
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
  
  int cont = 0;
  for(i = 0;i < lin;i++)
  	for(j = 0;j < col;j++){
  	  fscanf(arq, "%d", &mapa[i][j]);
  	  if(mapa[i][j] == PEDRA)
  	  	cont++;
  	}
  obj->qntd_rocks = cont;
  obj->rock = malloc(cont * sizeof(rock));

  cont = 0;
  for(i = 0;i < lin;i++)
  	for(j = 0;j < col;j++){
  	  if(mapa[i][j] == PEDRA){
  	  	obj->rock[cont].x = i;
  	    obj->rock[cont].y = j;
  	    obj->rock[cont].caindo = false;
  	    cont++;
  	  }
  	}

  fclose(arq);
  return mapa;
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

void draw_map(int** mapa, audio* som, objetos* objetos_mapa, long frames){
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
          testa_desmoronamento(mapa, som, objetos_mapa, i, j, frames);
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
          //testa_desmoronamento(mapa, objetos_mapa, i, j, frames);
          al_draw_scaled_bitmap(objetos_mapa->diamante[objetos_mapa->ciclos_diamante], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case EXPLOSAO:
          if(objetos_mapa->ciclos_explosao == 3){
          	//objetos_mapa->ciclos_explosao = 0;
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

void testa_desmoronamento(int** mapa, audio* som, objetos* objetos_mapa, int i, int j, long frames){
  //if(frames % 5 != 0)
  	//return;
  //Testa colisão com o personagem
  /*if(mapa[i + 1][j] == PLAYER){
  	if((mapa_anterior[i - 1][j] == PEDRA || mapa_anterior[i][j] == PEDRA) && mapa_anterior[i + 1][j] == PLAYER){
  	  mapa[i + 1][j] = PEDRA;
  	  mapa[i][j] = VAZIO;
  	}
  }*/
  //Testa rolamento para os lados
  //Testa se está no topo da pilha
  int pos_x, pos_y;

  /*for(int i = 0;i < objetos_mapa->qntd_rocks;i++){
  	pos_x = objetos_mapa->rock[i].x;
  	pos_y = objetos_mapa->rock[i].y;
  	if(objetos_mapa->rock[i].caindo == 1){
  	  if(mapa[pos_x + 1][pos_y] == PLAYER){
  	  mapa[pos_x][pos_y] = EXPLOSAO;
  	  mapa[pos_x][pos_y + 1] = EXPLOSAO;
  	  mapa[pos_x][pos_y - 1] = EXPLOSAO;
  	  mapa[pos_x + 1][pos_y + 1] = EXPLOSAO;
  	  mapa[pos_x + 1][pos_y - 1] = EXPLOSAO;
  	  mapa[pos_x + 2][pos_y - 1] = EXPLOSAO;
  	  mapa[pos_x + 2][pos_y] = EXPLOSAO;
  	  mapa[pos_x + 2][pos_y + 1] = EXPLOSAO;
  	  objetos_mapa->rock[i].y = 0;
  	  objetos_mapa->rock[i].x = 0;
  	  }
  	  else if(mapa[pos_x + 1][pos_y] == TERRA || mapa[pos_x + 1][pos_y] == MURO || mapa[pos_x + 1][pos_y] == METAL)
  	  	objetos_mapa->rock[i].caindo = 0;
  	}
  }*/

  for(int i = 0;i < objetos_mapa->qntd_rocks;i++){
  	pos_x = objetos_mapa->rock[i].x;
  	pos_y = objetos_mapa->rock[i].y;

  	if((mapa[pos_x + 1][pos_y] == PEDRA || mapa[pos_x + 1][pos_y] == DIAMANTE) && 
  	  (mapa[pos_x - 1][pos_y] != PEDRA || mapa[pos_x - 1][pos_y] != DIAMANTE)){
  	  if(mapa[pos_x][pos_y + 1] == VAZIO && mapa[pos_x + 1][pos_y + 1] == VAZIO){
  	    //if(mapa[i][j] == PEDRA)
  	    objetos_mapa->rock[i].y++;
  	    mapa[pos_x][pos_y + 1] = PEDRA;
  	    //else if(mapa[i][j] == DIAMANTE)
  	      //mapa[i][j + 1] = DIAMANTE;	 
  	    mapa[pos_x][pos_y] = VAZIO;
  	  }
  	  if(mapa[pos_x][pos_y - 1] == VAZIO && mapa[pos_x + 1][pos_y - 1] == VAZIO){
  	    //if(mapa[i][j] == PEDRA)
  	    objetos_mapa->rock[i].y--;
  	    mapa[pos_x][pos_y - 1] = PEDRA;
  	    //else if(mapa[i][j] == DIAMANTE)
  	      //mapa[i][j - 1] = DIAMANTE;	 
  	    mapa[pos_x][pos_y] = VAZIO;
  	  }
    }
    //Verifica se chegou no chão
  	if(objetos_mapa->rock[i].caindo == 1){
  	  if(mapa[pos_x + 1][pos_y] != VAZIO && mapa[pos_x + 1][pos_y] != PLAYER && mapa[pos_x + 1][pos_y] != EXPLOSAO){
  	  	play_sound(som->fall);
  	    objetos_mapa->rock[i].caindo = 0;
  	  }
  	}
  	//Desabamento normal
  	if(mapa[pos_x + 1][pos_y] == VAZIO){
  	  objetos_mapa->rock[i].caindo = 1;
  	  objetos_mapa->rock[i].x++;
  	  mapa[pos_x + 1][pos_y] = PEDRA;
  	  mapa[pos_x][pos_y] = VAZIO;
  	}
  }
  /*if((mapa[i + 1][j] == PEDRA || mapa[i + 1][j] == DIAMANTE) && (mapa[i - 1][j] != PEDRA || mapa[i - 1][j] != DIAMANTE)){
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
  	if(mapa[i][j] == PEDRA){
  	  mapa[i + 1][j] = PEDRA;
  	}
  	else if(mapa[i][j] == DIAMANTE)
  	  mapa[i + 1][j] = DIAMANTE;
  	mapa[i][j] = VAZIO;
  }*/
}