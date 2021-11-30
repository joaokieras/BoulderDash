// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  jogador->invencivel = 0;
  jogador->vivo = 1;
  jogador->vidas = 3;
  for(int i = 0;i < 4;i++)
  	jogador->code[i] = 'A';
  jogador->code[4] = '\0';
  inicia_sprites_jogador(sheet, jogador);
  return jogador;
}

void draw_player(player *jogador, audio* som, int** mapa, objetos *obj, long frames){
  int andou = 1;
  if(!jogador->vivo){
  	if(frames % TEMPO_RESET == 0)
  	  jogador->vivo = 1;
  	else
  	  return;
  }
  if(jogador->flag_left){
  	andou = testa_terreno(jogador, som, mapa, 0, obj, frames);
  	atualiza_player(jogador, 0, andou);
    al_draw_scaled_bitmap(jogador->player_esquerda[jogador->ciclos_esq], 0, 0, 15, 17, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if(jogador->flag_right){
  	andou = testa_terreno(jogador, som, mapa, 1, obj, frames);
  	atualiza_player(jogador, 1, andou);
    al_draw_scaled_bitmap(jogador->player_direita[jogador->ciclos_dir], 0, 0, 15, 16, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if(jogador->flag_up){
  	andou = testa_terreno(jogador, som, mapa, 2, obj, frames);
  	atualiza_player(jogador, 2, andou);
    al_draw_scaled_bitmap(jogador->player_direita[jogador->ciclos_dir], 0, 0, 15, 16, jogador->pos_x, jogador->pos_y, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if(jogador->flag_down){
  	andou = testa_terreno(jogador, som, mapa, 3, obj, frames);
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

int testa_terreno(player* jogador, audio* som, int** mapa, int direcao, objetos *obj, long frames){
  int x, y;
  //Coordenadas do personagem dentro do mapa
  x = jogador->pos_x / SIZE_OBJS;
  y = (jogador->pos_y - MARGIN_TOP) /SIZE_OBJS;	
  //0 - Esquerda - 1 - Direita - 2 - Cima - 3 - Baixo
  //Personagem só pode se mover a cada 5 frames
  if(jogador->tired % 5 == 0){
    switch(direcao){
  	case 0:
  	  if(verifica_diamantes_horizontal(jogador, som, mapa, obj, x, y, -1))
  	  	return 1;
  	  if(verifica_terra_horizontal(jogador, som, mapa, obj, x, y, -1))
  	  	return 1;
  	  //Empurra pedra à esquerda a cada 10 frames para ser mais demorado e "difícil"
  	  //Se [vazio][pedra][player], então -> [pedra][player][vazio]
  	  if(empurra_pedra_esq(jogador, som, mapa, obj, frames, x, y))
  	  	return 1;
  	  break;
  	case 1:
  	  if(verifica_diamantes_horizontal(jogador, som, mapa, obj, x, y, 1))
  	  	return 1;
  	  if(verifica_terra_horizontal(jogador, som, mapa, obj, x, y, 1))
  	  	return 1;
  	  if(empurra_pedra_dir(jogador, som, mapa, obj, frames, x, y))
  	  	return 1;
  	  break;
  	case 2:
  	  if(verifica_diamantes_vertical(jogador, som, mapa, obj, x, y, -1))
  	  	return 1;
  	  if(verifica_terra_vertical(jogador, som, mapa, obj, x, y, -1))
  	  	return 1;
  	  break;
  	case 3:
  	  if(verifica_diamantes_vertical(jogador, som, mapa, obj, x, y, 1))
  	  	return 1;
  	  if(verifica_terra_vertical(jogador, som, mapa, obj, x, y, 1))
  	  	return 1;
  	  break;
    } 
  }
  return 0;
}

int verifica_diamantes_horizontal(player* jogador, audio* som, int** mapa, objetos* obj, int x, int y, int lado){
  if(mapa[y][x + lado] == DIAMANTE){
  	busca_e_destroi_diamante(obj, y, x + lado);
  	mapa[y][x + lado] = PLAYER;
  	mapa[y][x] = VAZIO;
  	play_sound(som->diamante);
  	jogador->pontuacao += 10;
  	jogador->diamantes++;
  	return 1;
  }
  return 0;
}

int verifica_diamantes_vertical(player* jogador, audio* som, int** mapa, objetos* obj, int x, int y, int lado){
  if(mapa[y + lado][x] == DIAMANTE){
  	busca_e_destroi_diamante(obj, y + lado, x);
  	mapa[y + lado][x] = PLAYER;
  	mapa[y][x] = VAZIO;
  	play_sound(som->diamante);
  	jogador->pontuacao += 10;
  	jogador->diamantes++;
  	return 1;
  }
  return 0;
}

int verifica_terra_horizontal(player* jogador, audio* som, int** mapa, objetos* obj, int x, int y, int lado){
  if(mapa[y][x + lado] == TERRA || mapa[y][x + lado] == VAZIO){
  	if(mapa[y][x + lado] == TERRA)
  	  play_sound(som->terra);
  	mapa[y][x + lado] = PLAYER;
  	mapa[y][x] = VAZIO;
  	return 1;
  }
  return 0;
}

int verifica_terra_vertical(player* jogador, audio* som, int** mapa, objetos* obj, int x, int y, int lado){
  if(mapa[y + lado][x] == TERRA || mapa[y + lado][x] == VAZIO){
  	if(mapa[y + lado][x] == TERRA)
  	  play_sound(som->terra);
  	mapa[y + lado][x] = PLAYER;
  	mapa[y][x] = VAZIO;
  	return 1;
  }
  return 0;
}

int empurra_pedra_esq(player* jogador, audio* som, int** mapa, objetos* obj, long frames, int x, int y){
if(mapa[y][x - 1] == PEDRA && mapa[y][x - 2] == VAZIO && frames % 10 == 0){
  for(int i = 0;i < obj->qntd_rocks;i++){
  	if(obj->rock[i].x == y && obj->rock[i].y == x - 1)
  	  obj->rock[i].y--;
  }
  mapa[y][x - 1] = PLAYER;
  mapa[y][x - 2] = PEDRA;
  mapa[y][x] = VAZIO;
  return 1;
  }
  return 0;
}

int empurra_pedra_dir(player* jogador, audio* som, int** mapa, objetos* obj, long frames, int x, int y){
  if(mapa[y][x + 1] == PEDRA && mapa[y][x + 2] == VAZIO && frames % 10 == 0){
    for(int i = 0;i < obj->qntd_rocks;i++){
  	  if(obj->rock[i].x == y && obj->rock[i].y == x + 1)
  	  	obj->rock[i].y++;
  	}
  	mapa[y][x + 1] = PLAYER;
  	mapa[y][x + 2] = PEDRA;
  	mapa[y][x] = VAZIO;
  	return 1;
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

void verifica_min_diamantes(int** mapa, player* jogador){
  if(jogador->diamantes >= MIN_DIAMANTES)
  	mapa[16][38] = SAIDA;
}

void reseta_player(player* jogador){
  jogador->vivo = 0;
  jogador->vidas--;
  jogador->diamantes = 0;
  jogador->pos_x = SPAWN_X;
  jogador->pos_y = SPAWN_Y;
}

int testa_game_win(int** mapa, player* jogador){
  int x, y;

  x = jogador->pos_x / SIZE_OBJS;
  y = (jogador->pos_y - MARGIN_TOP) /SIZE_OBJS;
  if(mapa[16][38] == PLAYER)
  	return 1;
  else if(jogador->flag_left){
  	if(mapa[y][x - 1] == SAIDA){
  	  mapa[y][x - 1] = PLAYER;
  	  mapa[y][x] = VAZIO;
  	  atualiza_player(jogador, 0, 1);
  	  return 1;
  	}
  }
  else if(jogador->flag_right){
  	if(mapa[y][x + 1] == SAIDA){
  	  mapa[y][x + 1] = PLAYER;
  	  mapa[y][x] = VAZIO;
  	  atualiza_player(jogador, 1, 1);
  	  return 1;
  	}
  }
  else if(jogador->flag_up){
  	if(mapa[y - 1][x] == SAIDA){
  	  mapa[y - 1][x] = PLAYER;
  	  mapa[y][x] = VAZIO;
  	  atualiza_player(jogador, 2, 1);
  	  return 1;
  	}
  }
  else if(jogador->flag_down){
  	if(mapa[y + 1][x] == SAIDA){
  	  mapa[y + 1][x] = PLAYER;
  	  mapa[y][x] = VAZIO;
  	  atualiza_player(jogador, 3, 1);
  	  return 1;
  	}
  }
  return 0;
}

void destroi_player(player* jogador){
  free(jogador);
}