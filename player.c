#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mapa.h"
#include "player.h"

player* inicia_jogador(ALLEGRO_BITMAP* sheet){
  player *jogador;
  jogador = malloc(sizeof(player));
  if(jogador == NULL){
  	printf("Erro ao alocar memoria!\n");
  	exit(1);
  }
  jogador->pos_x = SPAWN_X;
  jogador->pos_y = SPAWN_Y;
  jogador->vel_x = SIZE_OBJS;
  jogador->vel_y = SIZE_OBJS;
  jogador->ciclos_parado = 0;
  jogador->ciclos_esq = 0;
  jogador->ciclos_dir = 0;
  jogador->tired = 0;
  jogador->flag_up = 0;
  jogador->flag_down = 0;
  jogador->flag_left = 0;
  jogador->flag_right = 0;

  jogador->player_parado[0] = al_create_sub_bitmap(sheet, 0, 0, 15, 16);
  for(int i = 1;i <= 6;i++)
    jogador->player_parado[i] = al_create_sub_bitmap(sheet, (16 * i), 0, 15, 16);	

  jogador->player_esquerda[0] = al_create_sub_bitmap(sheet, 0, 16, 15, 16);
  for(int i = 1;i <= 6;i++)
    jogador->player_esquerda[i] = al_create_sub_bitmap(sheet, (16 * i), 16, 15, 16);	

  jogador->player_direita[0] = al_create_sub_bitmap(sheet, 0, 32, 15, 16);
  for(int i = 1;i <= 6;i++)
    jogador->player_direita[i] = al_create_sub_bitmap(sheet, (16 * i), 32, 15, 16);	

  return jogador;
}

void draw_player(player *jogador, int** mapa, long frames){
  int x, y, andou = 1;
  x = jogador->pos_x / SIZE_OBJS;
  y = (jogador->pos_y - MARGIN_TOP) /SIZE_OBJS;
  if(jogador->flag_left){
  	andou = testa_terreno(x, y, mapa, 0);
  	atualiza_player(jogador, 0, andou);
    al_draw_scaled_bitmap(jogador->player_esquerda[jogador->ciclos_esq], 0, 0, 15, 17, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if(jogador->flag_right){
  	andou = testa_terreno(x, y, mapa, 1);
  	atualiza_player(jogador, 1, andou);
    al_draw_scaled_bitmap(jogador->player_direita[jogador->ciclos_dir], 0, 0, 15, 16, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if(jogador->flag_up){
  	andou = testa_terreno(x, y, mapa, 2);
  	atualiza_player(jogador, 2, andou);
    al_draw_scaled_bitmap(jogador->player_direita[jogador->ciclos_dir], 0, 0, 15, 16, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if(jogador->flag_down){
  	andou = testa_terreno(x, y, mapa, 3);
  	atualiza_player(jogador, 3, andou);
    al_draw_scaled_bitmap(jogador->player_direita[jogador->ciclos_dir], 0, 0, 15, 16, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  } 
  else
    al_draw_scaled_bitmap(jogador->player_parado[jogador->ciclos_parado], 0, 0, 15, 16, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);

  if(frames % 10 == 0)
    jogador->ciclos_parado++;
  if(jogador->ciclos_parado == 7)
    jogador->ciclos_parado = 0;
}

int testa_terreno(int x, int y, int** mapa, int direcao){
  switch(direcao){
  	case 0:
  	  if(mapa[y][x - 1] == TERRA || mapa[y][x - 1] == VAZIO){
  	  	mapa[y][x - 1] = VAZIO;
  	  	return 1;
  	  }
  	  break;
  	case 1:
  	  if(mapa[y][x + 1] == TERRA || mapa[y][x + 1] == VAZIO){
  	  	mapa[y][x + 1] = VAZIO;
  	  	return 1;
  	  }
  	  break;
  	case 2:
  	  if(mapa[y - 1][x] == TERRA || mapa[y - 1][x] == VAZIO){
  	  	mapa[y - 1][x] = VAZIO;
  	  	return 1;
  	  }
  	  break;
  	case 3:
  	  if(mapa[y + 1][x] == TERRA || mapa[y + 1][x] == VAZIO){
  	  	mapa[y + 1][x] = VAZIO;
  	  	return 1;
  	  }
  	  break;
  }
  return 0;
}

void atualiza_player(player *jogador, int direcao, int andou){
  switch(direcao){
  	case 0:
  	  jogador->flag_left = 0;
      jogador->ciclos_esq++;
      if(jogador->tired % 5 == 0 && andou){
    	jogador->pos_x -= jogador->vel_x;
    	jogador->tired = 0;
      }
      break;
    case 1:
  	  jogador->flag_right = 0;
      jogador->ciclos_dir++;
      if(jogador->tired % 5 == 0 && andou){
    	jogador->pos_x += jogador->vel_x;
    	jogador->tired = 0;
  	  }
      break;
    case 2:
      jogador->flag_up = 0;
      jogador->ciclos_dir++;
      if(jogador->tired % 5 == 0 && andou){
        jogador->pos_y -= jogador->vel_y;
        jogador->tired = 0;
      }
      break;
    case 3:
      jogador->flag_down = 0;
      jogador->ciclos_dir++;
      if(jogador->tired % 5 == 0 && andou){
        jogador->pos_y += jogador->vel_y;
        jogador->tired = 0;
      }
      break;	  
  }
  if(jogador->ciclos_esq == 7)
    jogador->ciclos_esq = 0;
  if(jogador->ciclos_dir == 7)
    jogador->ciclos_dir = 0;
}