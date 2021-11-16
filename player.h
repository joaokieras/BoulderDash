#ifndef __PLAYER__
#define __PLAYER__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "game.h"

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
  int pos_x, pos_y;
  int vel_x, vel_y;
};
typedef struct player player;

player* inicia_jogador(ALLEGRO_BITMAP* sheet);
int testa_terreno(player *jogador, int** mapa, int direcao);
void atualiza_player(player *jogador, int direcao, int andou);
void draw_player(player *jogador, int** mapa, long frames);
#endif