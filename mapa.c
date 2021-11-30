// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR 
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mapa.h"
#include "sprites.h"

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
  
  int cont_pedras = 0, cont_diamantes = 0;
  for(i = 0;i < lin;i++)
  	for(j = 0;j < col;j++){
  	  fscanf(arq, "%d", &mapa[i][j]);
  	  if(mapa[i][j] == PEDRA)
  	  	cont_pedras++;
  	  if(mapa[i][j] == DIAMANTE)
  	  	cont_diamantes++;
  	}
  obj->qntd_rocks = cont_pedras;
  obj->qntd_diamonds = cont_diamantes;
  obj->rock = malloc(cont_pedras * sizeof(rock));
  obj->diamond = malloc(cont_diamantes * sizeof(rock));
  inicia_pedras_e_diamantes(mapa, obj);
  fclose(arq);
  return mapa;
}

void inicia_pedras_e_diamantes(int** mapa, objetos* obj){
  int cont_pedras = 0, cont_diamantes = 0, i, j;

  for(i = 0;i < 22;i++){
  	for(j = 0;j < 40;j++){
  	  if(mapa[i][j] == PEDRA){
  	  	obj->rock[cont_pedras].x = i;
  	    obj->rock[cont_pedras].y = j;
  	    obj->rock[cont_pedras].caindo = 0;
  	    cont_pedras++;
  	  }
  	  if(mapa[i][j] == DIAMANTE){
  	  	obj->diamond[cont_diamantes].x = i;
  	    obj->diamond[cont_diamantes].y = j;
  	    obj->diamond[cont_diamantes].caindo = 0;
  	    cont_diamantes++;
  	  }
  	}
  }
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
          al_draw_scaled_bitmap(objetos_mapa->pedra, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case VAZIO:
          al_draw_scaled_bitmap(objetos_mapa->vazio, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case SAIDA:
          al_draw_scaled_bitmap(objetos_mapa->saida, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case DIAMANTE:
          //Reseta frames do diamante se necessário e faz animação
          if(objetos_mapa->ciclos_diamante == 7)
          	objetos_mapa->ciclos_diamante = 0;
          if(frames % 30 == 0)
            objetos_mapa->ciclos_diamante++;
          al_draw_scaled_bitmap(objetos_mapa->diamante[objetos_mapa->ciclos_diamante], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case EXPLOSAO:
          al_draw_scaled_bitmap(objetos_mapa->explosao[1], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          if(frames % 15 == 0)
            mapa[i][j] = EXPLOSAO2;
          break;
        case EXPLOSAO2:
          al_draw_scaled_bitmap(objetos_mapa->explosao[2], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          if(frames % 15 == 0)
            mapa[i][j] = EXPLOSAO3;
          break;
        case EXPLOSAO3:
          al_draw_scaled_bitmap(objetos_mapa->explosao[3], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          if(frames % 15 == 0)
            mapa[i][j] = VAZIO;
          break;
  	  }
  	}
  } 
}

void testa_desmoronamento_pedra(int** mapa, audio* som, objetos* objetos_mapa, long frames){
  //Só ocorre desmoronamento a cada 15 frames para não ser tão rápido
  if(frames % 15 != 0)
	return;

  int pos_x, pos_y;
  for(int i = 0;i < objetos_mapa->qntd_rocks;i++){
  	pos_x = objetos_mapa->rock[i].x;
  	pos_y = objetos_mapa->rock[i].y;
  	verifica_rolamento_pedras(mapa, objetos_mapa, pos_x, pos_y, i);
    //Verifica se chegou no chão
  	if(objetos_mapa->rock[i].caindo == 1){
  	  if(mapa[pos_x + 1][pos_y] != VAZIO && mapa[pos_x + 1][pos_y] != PLAYER && mapa[pos_x + 1][pos_y]){
  	  	play_sound(som->fall);
  	    objetos_mapa->rock[i].caindo = 0;
  	  }
  	}
  	//Desabamento normal
  	if(mapa[pos_x + 1][pos_y] == VAZIO && (pos_x + 1 < 21)){
  	  objetos_mapa->rock[i].caindo = 1;
  	  objetos_mapa->rock[i].x++;
  	  mapa[pos_x + 1][pos_y] = PEDRA;
  	  mapa[pos_x][pos_y] = VAZIO;
  	}
  }
}

void testa_desmoronamento_diamante(int** mapa, audio* som, objetos* objetos_mapa, long frames){
  if(frames % 10 != 0)
  	return;	
  int pos_x, pos_y;
  for(int i = 0;i < objetos_mapa->qntd_diamonds;i++){
    pos_x = objetos_mapa->diamond[i].x;
  	pos_y = objetos_mapa->diamond[i].y;
  	verifica_rolamento_diamantes(mapa, objetos_mapa, pos_x, pos_y, i);
  	if(objetos_mapa->diamond[i].caindo == 1){
  	  if(mapa[pos_x + 1][pos_y] != VAZIO && mapa[pos_x + 1][pos_y] != PLAYER && mapa[pos_x + 1][pos_y] != EXPLOSAO){
  	  	play_sound(som->fall);
  	    objetos_mapa->diamond[i].caindo = 0;
  	  }
  	}
  	if(mapa[pos_x + 1][pos_y] == VAZIO && (pos_x + 1 < 21)){
  	  objetos_mapa->diamond[i].caindo = 1;
  	  objetos_mapa->diamond[i].x++;
  	  mapa[pos_x + 1][pos_y] = DIAMANTE;
  	  mapa[pos_x][pos_y] = VAZIO;
  	}
  }
}

void verifica_rolamento_pedras(int** mapa, objetos* objetos_mapa, int pos_x, int pos_y, int i){
  //Verifica se está no topo da pilha
  if((mapa[pos_x + 1][pos_y] == PEDRA || mapa[pos_x + 1][pos_y] == DIAMANTE) && 
  	  (mapa[pos_x - 1][pos_y] != PEDRA || mapa[pos_x - 1][pos_y] != DIAMANTE)){
  	if(mapa[pos_x][pos_y + 1] == VAZIO && mapa[pos_x + 1][pos_y + 1] == VAZIO){
  	  objetos_mapa->rock[i].y++;
  	  mapa[pos_x][pos_y + 1] = PEDRA;	 
  	  mapa[pos_x][pos_y] = VAZIO;
  	}
  	else if(mapa[pos_x][pos_y - 1] == VAZIO && mapa[pos_x + 1][pos_y - 1] == VAZIO){
  	  objetos_mapa->rock[i].y--;
  	  mapa[pos_x][pos_y - 1] = PEDRA;	 
  	  mapa[pos_x][pos_y] = VAZIO;
  	}
  }
}

void verifica_rolamento_diamantes(int** mapa, objetos* objetos_mapa, int pos_x, int pos_y, int i){
  if((mapa[pos_x + 1][pos_y] == PEDRA || mapa[pos_x + 1][pos_y] == DIAMANTE) && 
  	  (mapa[pos_x - 1][pos_y] != PEDRA || mapa[pos_x - 1][pos_y] != DIAMANTE)){
  	if(mapa[pos_x][pos_y + 1] == VAZIO && mapa[pos_x + 1][pos_y + 1] == VAZIO){
  	  objetos_mapa->diamond[i].y++;
  	  mapa[pos_x][pos_y + 1] = DIAMANTE; 
  	  mapa[pos_x][pos_y] = VAZIO;
  	}
  	else if(mapa[pos_x][pos_y - 1] == VAZIO && mapa[pos_x + 1][pos_y - 1] == VAZIO){
  	  objetos_mapa->diamond[i].y--;
  	  mapa[pos_x][pos_y - 1] = DIAMANTE;	 
  	  mapa[pos_x][pos_y] = VAZIO;
  	}
  }
}

int testa_game_over(int** mapa, audio* som, objetos* objetos_mapa, long frames, int tempo){
  if(frames % 10 != 0)
  	return 0;

  int pos_x, pos_y;
  //Percorre vetor de pedras verificando se acerta o player
  for(int i = 0;i < objetos_mapa->qntd_rocks;i++){
  	pos_x = objetos_mapa->rock[i].x;
  	pos_y = objetos_mapa->rock[i].y;
  	if(objetos_mapa->rock[i].caindo == 1){
  	  if(mapa[pos_x + 1][pos_y] == PLAYER){
  	  play_sound(som->fall);
  	  busca_pedras_explosao(objetos_mapa, pos_x + 1, pos_y);
  	  explode_em_volta(mapa, pos_x, pos_y);
  	  //"Deleta" pedra que explodiu o player
  	  objetos_mapa->rock[i].y = 0;
  	  objetos_mapa->rock[i].x = 0;
  	  objetos_mapa->rock[i].caindo = 0;
  	  return 1;
  	  }
  	}
  }
  //O mesmo com vetor de diamantes
  for(int i = 0;i < objetos_mapa->qntd_diamonds;i++){
  	pos_x = objetos_mapa->diamond[i].x;
  	pos_y = objetos_mapa->diamond[i].y;
  	if(objetos_mapa->diamond[i].caindo == 1){
  	  if(mapa[pos_x + 1][pos_y] == PLAYER){
  	  play_sound(som->fall);
  	  busca_diamantes_explosao(objetos_mapa, pos_x + 1, pos_y);
  	  explode_em_volta(mapa, pos_x, pos_y);
  	  objetos_mapa->diamond[i].y = 0;
  	  objetos_mapa->diamond[i].x = 0;
  	  objetos_mapa->diamond[i].caindo = 0;
  	  return 1;
  	  }
  	}
  }
  return 0;
}

void explode_em_volta(int** mapa, int pos_x, int pos_y){
  mapa[pos_x + 1][pos_y] = EXPLOSAO;
  mapa[pos_x][pos_y] = EXPLOSAO;
  mapa[pos_x][pos_y + 1] = EXPLOSAO;
  mapa[pos_x][pos_y - 1] = EXPLOSAO;
  mapa[pos_x + 1][pos_y + 1] = EXPLOSAO;
  mapa[pos_x + 1][pos_y - 1] = EXPLOSAO;
  mapa[pos_x + 2][pos_y - 1] = EXPLOSAO;
  mapa[pos_x + 2][pos_y] = EXPLOSAO;
  mapa[pos_x + 2][pos_y + 1] = EXPLOSAO;
}

//Destroi qualquer pedra em volta da explosão
void busca_pedras_explosao(objetos* obj, int x, int y){
  busca_e_destroi_pedra(obj, x - 1, y - 1);
  busca_e_destroi_pedra(obj, x - 1, y);
  busca_e_destroi_pedra(obj, x - 1, y + 1);
  busca_e_destroi_pedra(obj, x, y - 1);
  busca_e_destroi_pedra(obj, x, y + 1);
  busca_e_destroi_pedra(obj, x + 1, y - 1);
  busca_e_destroi_pedra(obj, x + 1, y);
  busca_e_destroi_pedra(obj, x + 1, y + 1);
}

//Destroi qualquer diamante em volta da explosão
void busca_diamantes_explosao(objetos* obj, int x, int y){
  busca_e_destroi_diamante(obj, x - 1, y - 1);
  busca_e_destroi_diamante(obj, x - 1, y);
  busca_e_destroi_diamante(obj, x - 1, y + 1);
  busca_e_destroi_diamante(obj, x, y - 1);
  busca_e_destroi_diamante(obj, x, y + 1);
  busca_e_destroi_diamante(obj, x + 1, y - 1);
  busca_e_destroi_diamante(obj, x + 1, y);
  busca_e_destroi_diamante(obj, x + 1, y + 1);
}

void busca_e_destroi_pedra(objetos* obj, int x, int y){
  int i;
  for(int i = 0;i < obj->qntd_rocks;i++){
  	if(obj->rock[i].x == x && obj->rock[i].y == y){
  	  obj->rock[i].x = 0;
  	  obj->rock[i].y = 0;
  	  obj->rock[i].caindo = 0;
  	}
  }
}

void busca_e_destroi_diamante(objetos* obj, int x, int y){
  int i;
  for(int i = 0;i < obj->qntd_diamonds;i++){
  	if(obj->diamond[i].x == x && obj->diamond[i].y == y){
  	  obj->diamond[i].x = 0;
  	  obj->diamond[i].y = 0;
  	  obj->diamond[i].caindo = 0;
  	}
  }
}

void destroi_mapa(int** mapa){
  int i;
  for(i = 0;i < 22;i++)
  	free(mapa[i]);
  free(mapa);
}

void destroi_objetos(objetos* obj){
  free(obj->rock);
  free(obj->diamond);
  free(obj);
}