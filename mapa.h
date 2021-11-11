#ifndef __MAPA__
#define __MAPA__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "game.h"

#define METAL    0
#define TERRA    1
#define MURO     2
#define PEDRA    3
#define DIAMANTE 4

struct objetos{
  ALLEGRO_BITMAP* terra;
  ALLEGRO_BITMAP* muro;
  ALLEGRO_BITMAP* metal;
  ALLEGRO_BITMAP* pedra;
  ALLEGRO_BITMAP* diamante[8];
  int ciclos_diamante;
};
typedef struct objetos objetos;

void draw_map(int** mapa, objetos* objetos_mapa, long frames);
int** inicia_mapa(char* nome_mapa);
objetos* inicia_objetos(ALLEGRO_BITMAP* sheet);

#endif