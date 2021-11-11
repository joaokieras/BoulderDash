#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mapa.h"

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
  fprintf(stderr, "Mapa lido com sucesso!\n");
  return mapa;
}

objetos* inicia_objetos(ALLEGRO_BITMAP* sheet){
  objetos *obj;
  obj = malloc(sizeof(objetos));
  if(obj == NULL){
    fprintf(stderr, "Erro ao alocar memória!\n");
    exit(1);
  }
  obj->ciclos_diamante = 0;
  obj->metal = al_create_sub_bitmap(sheet, 0, 48, 15, 16);
  obj->terra = al_create_sub_bitmap(sheet, 48, 48, 15, 16);
  obj->muro  = al_create_sub_bitmap(sheet, 32, 48, 15, 16);
  obj->pedra = al_create_sub_bitmap(sheet, 80, 48, 15, 16);
  obj->diamante[0] = al_create_sub_bitmap(sheet, 0, 64, 15, 16);
  obj->diamante[1] = al_create_sub_bitmap(sheet, 16, 64, 15, 16);
  obj->diamante[2] = al_create_sub_bitmap(sheet, 0, 80, 15, 16);
  obj->diamante[3] = al_create_sub_bitmap(sheet, 16, 80, 15, 16);
  obj->diamante[4] = al_create_sub_bitmap(sheet, 0, 96, 15, 16);
  obj->diamante[5] = al_create_sub_bitmap(sheet, 16, 96, 15, 16);
  obj->diamante[6] = al_create_sub_bitmap(sheet, 0, 112, 15, 16);
  obj->diamante[7] = al_create_sub_bitmap(sheet, 16, 112, 15, 16);
}

void draw_map(int** mapa, objetos* objetos_mapa, long frames){
  int i, j, i_aux, j_aux;
  for(i = 0;i < 22;i++)
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
        case DIAMANTE:
          if(objetos_mapa->ciclos_diamante == 7)
          	objetos_mapa->ciclos_diamante = 0;
          if(frames % 30 == 0)
            objetos_mapa->ciclos_diamante++;
          al_draw_scaled_bitmap(objetos_mapa->diamante[objetos_mapa->ciclos_diamante], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
  	  }
  	}
}