// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR 
#ifndef __PLAYER__
#define __PLAYER__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "mapa.h"
#include "sons.h"

struct player{
  // Sprites
  ALLEGRO_BITMAP* player_parado[7];
  ALLEGRO_BITMAP* player_esquerda[7];
  ALLEGRO_BITMAP* player_direita[7];
  // Flags de movimento
  int flag_up, flag_down, flag_right, flag_left;
  // Ciclos de animação (percorre vetor de sprites)
  int ciclos_parado, ciclos_esq, ciclos_dir;
  int tired;
  int vivo, vidas;
  int pos_x, pos_y;
  int vel_x, vel_y;
  int invencivel;
  int diamantes;
  int pontuacao;
  char code[5];
};
typedef struct player player;

player* inicia_jogador(ALLEGRO_BITMAP* sheet);
int testa_terreno(player *jogador, audio* som, int** mapa, int direcao, objetos *obj,long frames);
void atualiza_player(player *jogador, int direcao, int andou);
void atualiza_player_esq(player* joagdor, int andou);
void atualiza_player_dir(player* jogador, int andou);
void atualiza_player_cima(player* jogador, int andou);
void atualiza_player_baixo(player* jogador, int andou);
void draw_player(player *jogador, audio* som, int** mapa, objetos *obj, long frames);
void verifica_min_diamantes(int** mapa, player* jogador);
void reseta_player(player* jogador);
int testa_game_win(int** mapa, player* jogador);
void destroi_player(player* jogador);
int verifica_diamantes_horizontal(player* jogador, audio* som, int** mapa, objetos* obj, int x, int y, int lado);
int verifica_diamantes_vertical(player* jogador, audio* som, int** mapa, objetos* obj, int x, int y, int lado);
int verifica_terra_horizontal(player* jogador, audio* som, int** mapa, int x, int y, int lado);
int verifica_terra_vertical(player* jogador, audio* som, int** mapa, int x, int y, int lado);
int verifica_saida_horizontal(player* jogador, int** mapa, int x, int y, int lado, int direcao);
int verifica_saida_vertical(player* jogador, int** mapa, int x, int y, int lado, int direcao);
int empurra_pedra_esq(player* jogador, audio* som, int** mapa, objetos* obj, long frames, int x, int y);
int empurra_pedra_dir(player* jogador, audio* som, int** mapa, objetos* obj, long frames, int x, int y);
#endif