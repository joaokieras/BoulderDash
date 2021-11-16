// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mapa.h"
#include "player.h"
#include "sprites.h"

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
  jogador->diamantes = 0;
  jogador->pontuacao = 0;
  inicia_sprites_jogador(sheet, jogador);
  return jogador;
}

void draw_player(player *jogador, int** mapa, long frames){
  int andou = 1;
  if(jogador->flag_left){
  	andou = testa_terreno(jogador, mapa, 0, frames);
  	atualiza_player(jogador, 0, andou);
    al_draw_scaled_bitmap(jogador->player_esquerda[jogador->ciclos_esq], 0, 0, 15, 17, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if(jogador->flag_right){
  	andou = testa_terreno(jogador, mapa, 1, frames);
  	atualiza_player(jogador, 1, andou);
    al_draw_scaled_bitmap(jogador->player_direita[jogador->ciclos_dir], 0, 0, 15, 16, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if(jogador->flag_up){
  	andou = testa_terreno(jogador, mapa, 2, frames);
  	atualiza_player(jogador, 2, andou);
    al_draw_scaled_bitmap(jogador->player_direita[jogador->ciclos_dir], 0, 0, 15, 16, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if(jogador->flag_down){
  	andou = testa_terreno(jogador, mapa, 3, frames);
  	atualiza_player(jogador, 3, andou);
    al_draw_scaled_bitmap(jogador->player_direita[jogador->ciclos_dir], 0, 0, 15, 16, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  } 
  else
    al_draw_scaled_bitmap(jogador->player_parado[jogador->ciclos_parado], 0, 0, 15, 16, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  //De 10 em 10 frames a animação muda
  if(frames % 10 == 0)
    jogador->ciclos_parado++;
  if(jogador->ciclos_parado == 7)
    jogador->ciclos_parado = 0;
}

int testa_terreno(player* jogador, int** mapa, int direcao, long frames){
  int x, y;
  //Coordenadas do personagem dentro do mapa
  x = jogador->pos_x / SIZE_OBJS;
  y = (jogador->pos_y - MARGIN_TOP) /SIZE_OBJS;	
  //0 - Esquerda
  //1 - Direita
  //2 - Cima
  //3 - Baixo
  //Personagem só pode se mover a cada 5 frames
  if(jogador->tired % 5 == 0){
    switch(direcao){
  	case 0:
  	  //Verifica se há diamante
  	  if(mapa[y][x - 1] == DIAMANTE){
  	  	  mapa[y][x - 1] = PLAYER;
  	  	  mapa[y][x] = VAZIO;
  	  	  jogador->pontuacao += 40;
  	  	  jogador->diamantes++;
  	  	  return 1;
  	  }
  	  if(mapa[y][x - 1] == TERRA || mapa[y][x - 1] == VAZIO){
  	  	  mapa[y][x - 1] = PLAYER;
  	  	  mapa[y][x] = VAZIO;
  	  	  return 1;
  	  }
  	  //Empurra pedra à esquerda a cada 10 frames para ser mais demorado e "difícil"
  	  //Se [vazio][pedra][player], então -> [pedra][player][vazio]
  	  if(mapa[y][x - 1] == PEDRA && mapa[y][x - 2] == VAZIO && frames % 10 == 0){
  	  	  mapa[y][x - 1] = PLAYER;
  	  	  mapa[y][x - 2] = PEDRA;
  	  	  mapa[y][x] = VAZIO;
  	  	  return 1;
  	  }
  	  break;
  	case 1:
  	  if(mapa[y][x + 1] == DIAMANTE){
  	  	  mapa[y][x + 1] = PLAYER;
  	  	  mapa[y][x] = VAZIO;
  	  	  jogador->pontuacao += 40;
  	  	  jogador->diamantes++;
  	  	  return 1;
  	  }
  	  if(mapa[y][x + 1] == TERRA || mapa[y][x + 1] == VAZIO){
  	  	  mapa[y][x + 1] = PLAYER;
  	  	  mapa[y][x] = VAZIO;
  	  	  return 1;
  	  }
  	  //Empurra pedra à direita
  	  if(mapa[y][x + 1] == PEDRA && mapa[y][x + 2] == VAZIO && frames % 10 == 0){
  	  	  mapa[y][x + 1] = PLAYER;
  	  	  mapa[y][x + 2] = PEDRA;
  	  	  mapa[y][x] = VAZIO;
  	  	  return 1;
  	  }
  	  break;
  	case 2:
  	  if(mapa[y - 1][x] == DIAMANTE){
  	  	  mapa[y - 1][x] = PLAYER;
  	  	  mapa[y][x] = VAZIO;
  	  	  jogador->pontuacao += 40;
  	  	  jogador->diamantes++;
  	  	  return 1;
  	  }
  	  if(mapa[y - 1][x] == TERRA || mapa[y - 1][x] == VAZIO){
  	  	  mapa[y - 1][x] = PLAYER;
  	  	  mapa[y][x] = VAZIO;
  	  	  return 1;
  	  }
  	  break;
  	case 3:
  	  if(mapa[y + 1][x] == DIAMANTE){
  	  	  mapa[y + 1][x] = PLAYER;
  	  	  mapa[y][x] = VAZIO;
  	  	  jogador->pontuacao += 40;
  	  	  jogador->diamantes++;
  	  	  return 1;
  	  }
  	  if(mapa[y + 1][x] == TERRA || mapa[y + 1][x] == VAZIO){
  	  	  mapa[y + 1][x] = PLAYER;
  	  	  mapa[y][x] = VAZIO;
  	  	  return 1;
  	  }
  	  break;
  }
  
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