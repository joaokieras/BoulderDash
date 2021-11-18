// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR
#ifndef __MAPA__
#define __MAPA__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "sons.h"

#define METAL    0
#define TERRA    1
#define MURO     2
#define PEDRA    3
#define DIAMANTE 4
#define VAZIO    5
#define PLAYER   6
#define EXPLOSAO 7

struct rock{
  int x, y;
  int caindo;
};
typedef struct rock rock;

struct objetos{
  rock *rock;
  rock *diamond;
  ALLEGRO_BITMAP* terra;
  ALLEGRO_BITMAP* muro;
  ALLEGRO_BITMAP* metal;
  ALLEGRO_BITMAP* pedra;
  ALLEGRO_BITMAP* vazio;
  ALLEGRO_BITMAP* diamante[8];
  ALLEGRO_BITMAP* explosao[4];
  int qntd_rocks, qntd_diamonds;
  int ciclos_diamante, ciclos_explosao;
};
typedef struct objetos objetos;

void draw_map(int** mapa, audio* som, objetos* objetos_mapa, long frames);
void testa_desmoronamento_pedra(int** mapa, audio* som, objetos* objetos_mapa, int i, int j, long frames);
void testa_desmoronamento_diamante(int** mapa, audio* som, objetos* objetos_mapa, int i, int j, long frames);
int** inicia_mapa(char* nome_mapa, objetos* obj);
objetos* inicia_objetos(ALLEGRO_BITMAP* sheet);

#endif